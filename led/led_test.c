#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "serial.h"


const static char CEPTON_LED_SIGNATURE = '\x55';

typedef struct cepton_led_s {
    int gpio;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} cepton_led_t ;

int led_send(cepton_led_t led) {
    serial_send(led.gpio, (void *) &CEPTON_LED_SIGNATURE, 1);
    serial_send(led.gpio, (void *) &led.red, 2);
    serial_send(led.gpio, (void *) &led.green, 2);
    serial_send(led.gpio, (void *) &led.blue, 2);
    return 0;
}

int main(void) {

    cepton_led_t led = {-1, 0, 0, 0};
    led.gpio = serial_open();
    if (led.gpio < 0) {
        fprintf(stderr, "Erro opening gpio port\n");
        return -1;
    }
    int r, g, b;
    while (true) {
        printf("Input RGB: ");
        scanf("%d %d %d", &r, &g, &b);
        if (r == -1) {
            break;
        }
        led.red = r;
        led.green = g;
        led.blue = b;
        led_send(led);
    }
    serial_close(led.gpio);
    return 0;
}