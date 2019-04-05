#ifndef CEPTON_SERIAL_H
#define CEPTON_SERIAL_H

#include <stdint.h>

int serial_open();
int serial_close(int fd);
int serial_send(int fd, void *buff, int size);


#endif