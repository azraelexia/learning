#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "gpio.h"


int main(void) {
    // P4-1
    cepton_gpio_t pio = {-1, 388, true};
    int erro = gpio_open(&pio);
    if (erro != 0) {
        fprintf(stderr, "Failed to open GPIO\n");
        fprintf(stderr, "erro num: %d\n", erro);
        return -1;
    }
    gpio_write(pio, 1);
    printf("setting P4-1 to HIGH\n");
    getchar();
    return 0;
}