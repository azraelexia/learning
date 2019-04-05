#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "serial.h"


int main(void) {
    int fd = serial_open();
    if (fd < 0) {
        fprintf(stderr, "Erro opening serial port\n");
        return -1;
    }
    char input_buff[256] = { 0 };
    int input_size = 0;
    uint8_t num;
    while (true) {
        fgets(input_buff, 256, stdin);
        input_size = strlen(input_buff);
        num = atoi(input_buff);
        if (input_size == 1) {
            printf("Quiting\n");
            break;
        } else {
            serial_send(fd, &num, 1);
            printf("Sending: %d\n", num);
        }
    }
    serial_close(fd);
    return 0;
}
