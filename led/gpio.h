#ifndef CEPTON_GPIO_H
#define CEPTON_GPIO_H

#include <stdint.h>

typedef uint32_t jetson_gpio_t;
typedef int jetson_gpio_f;

typedef struct cepton_gpio_s {
    jetson_gpio_f fd;
    const jetson_gpio_t gpio;
    const uint8_t output;
} cepton_gpio_t ;

int gpio_open(cepton_gpio_t* const gpio_s);
int gpio_close(const cepton_gpio_t gpio_s);
int gpio_read(const cepton_gpio_t gpio_s, uint8_t* const buff);
int gpio_write(const cepton_gpio_t gpio_s, const uint8_t value);


#endif