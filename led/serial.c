#include "serial.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define _LED_SERIAL_FILE "/dev/ttyS0"
#define _LED_SERIAL_SPEED B19200

static int set_interface_attr(int fd) {
    struct termios tty = { 0 };
    if (tcgetattr(fd, &tty) != 0) {
        fprintf(stderr, "Erro %d from tcgetattr: %s", errno, strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, _LED_SERIAL_SPEED);
    // ignoring the input
    // cfsetispeed
    
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= 0;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        fprintf(stderr, "Erro %d setting term attributes: %s", errno, strerror(errno));
        return -1;
    }

    return 0;
}

static int set_nonblocking(int fd) {
    struct termios tty = { 0 };
    if (tcgetattr(fd, &tty) != 0) {
        fprintf(stderr, "Erro %d from tcgetattr: %s", errno, strerror(errno));
        return -1;
    }

    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        fprintf(stderr, "Erro %d setting term attributes: %s", errno, strerror(errno));
        return -1;
    }

    return 0;
}

int serial_open() {
    int fd = open(_LED_SERIAL_FILE, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "Erro %d opening %s: %s", errno, _LED_SERIAL_FILE, strerror(errno));
        return fd;
    }
    
    if (set_interface_attr(fd) != 0) {
        return -1;
    }
    if (set_nonblocking(fd) != 0) {
        return -1;
    }

    return fd;
}

int serial_close(int fd) {
    return close(fd);
}

int serial_send(int fd, void *buff, int size) {
    for (int i = size - 1; i >= 0; ++i) {
        write(fd, buff[i], 1);
    }
}