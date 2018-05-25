//
// Created by ajalal on 3/29/18.
//

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include "connect.h"
#include "hwlog.h"
#include <pthread.h>

extern "C" {

JNIEXPORT jint JNICALL
Java_com_inthinc_vbussimulator_home_init(JNIEnv *env, jclass clazz, jint handle) {
    LOGI("JNI initialization");

    sock_open("/dev/ttyACM2");

    char buf[64];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "Cu%04x\r", 1000);
    sock_write(tty_fd, buf, sizeof(buf));
//
    LOGI("mask setup");
    sleep(1);
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "m%c%08xu%04x\r", 't', 0x7e8, 1000);
    sock_write(tty_fd, buf, sizeof(buf));


    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "M%c%08xu%04x\r", 't', 0x7e8, 1000);
    sock_write(tty_fd, buf, sizeof(buf));

    LOGI("subscribe");
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "Mf%03x%03x8%02x%02x%02x%02x%02x%02x%02x%02xu%04x\r",
             (0x7e8 & 0x1fffffff), (0x7e0 & 0x1fffffff),
             0x02, 0x00, 0x00, 0x00,
             0x00, 0x00, 0x00, 0x00,
             1000);
    sock_write(tty_fd, buf, sizeof(buf));

    LOGI("tranceiver closed waiting 1 sec");
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "S%du%04x\r", 7, 1000);
    sock_write(tty_fd, buf, sizeof(buf));


    LOGI("configuring port");
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "O%du%04x\r", 1, 1000);
    sock_write(tty_fd, buf, sizeof(buf));

    sleep(1);

    while (true){

        int buff = sock_read(tty_fd);
        char raw[sizeof buff];
        snprintf(raw, 26, "%s", buff);

//
        LOGI("first bytes %02x %02x %02x %02x %02x %02x", raw[0], raw[1], raw[2], raw[3], raw[4], raw[5]);
        LOGI("raw: %s", raw);


        if(raw[0] == 0x74){
            __u8 frame[26];
            memset(frame, 0, sizeof(buff));

            LOGI("my frame %s", frame);
        }
        usleep(10000);
    }
    //LOGI("reading, %s", rf);

    sleep(1);


    LOGI("closing port");
    //sleep(10);
    sock_close(tty_fd);
    return handle;

}
}