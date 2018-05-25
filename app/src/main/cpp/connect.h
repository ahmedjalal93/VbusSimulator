//
// Created by ajalal on 3/29/18.
//

#ifndef VBUSSIMULATOR_CONNECT_H
#define VBUSSIMULATOR_CONNECT_H

#endif //VBUSSIMULATOR_CONNECT_H

extern int tty_fd;
int sock_open(char *dev);
void sock_close(int fd);
int sock_read(int fd);
int sock_write(int fd, char *data, size_t len);
