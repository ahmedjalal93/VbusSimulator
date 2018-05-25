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


int tty_fd;

int sock_open(char *dev) {

    if(tty_fd == -1){
        sock_close(tty_fd);
    }

    int fd;
    char device[32];
    errno = 0;

    struct termios options;

    snprintf(device, sizeof(device), "%s", dev);
    fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (-1 == fd) {
        LOGI("Socket open failed for %s with error [%s]", device, strerror(errno));
        return 1;
    } else {
        LOGI("Socket opened successfully");
    }
    tty_fd = fd;

    if (isatty(tty_fd)) {
        struct termios  ios;

        tcgetattr(fd, &ios);

        bzero(&ios, sizeof(ios));

        cfmakeraw(&ios);

        cfsetospeed(&ios, B115200);
        cfsetispeed(&ios, B115200);
        ios.c_cflag =  (B115200 | CS8 | CLOCAL | CREAD) & ~(CRTSCTS | CSTOPB | PARENB);
        ios.c_iflag = 0;
        ios.c_oflag = 0;
        ios.c_lflag = 0;        /* disable ECHO, ICANON, etc... */
        ios.c_cc[VTIME] = 10;   /* unit: 1/10 second. */
        ios.c_cc[VMIN] = 1;     /* minimal characters for reading */

        tcsetattr( fd, TCSANOW, &ios );
        tcflush(fd, TCIOFLUSH);
    } else {
        LOGI("Error isatty(fd): invalid terminal %s", strerror(errno));
        close(fd);
        return 1;
    }
    return fd;
}

void sock_close(int fd) {
    close(fd);
    tcflush(fd,TCIOFLUSH);
    LOGI("Socket closed successfully");
}

int sock_read(int fd) {
    int rd;
    __u8 buf[4096];

    memset(buf, 0, sizeof buf);
//    for(int i = 0; i < sizeof buf; i++){
//        memset(&buf[i], 0, sizeof buf);
//    }

    rd = read (fd, buf, sizeof(buf));

    LOGI("Read %d: %s", rd, buf);
    return (int) buf;
}

int sock_write(int fd, char *data, size_t len) {
    int wr;

    LOGI("started writing %s ", data);
    wr = write (fd, data, strlen(data));
    if (wr < 0) {
        LOGI("writing to port failed");
    }
    tcflush(fd, TCIOFLUSH);
    LOGI("writing %s | %d", data, strlen(data));
    sleep(1);
//    sock_close(fd);
    return 0;
}
