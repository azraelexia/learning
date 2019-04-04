#include "gpio.h"

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/capability.h>
#include <sys/types.h>

#define GPIO_SYS_DIR "sys/class/gpio"


// 248, 256, 320 are gpiochip on TX2
#define GPIO_MIN 249


static inline int set_gpio_export(jetson_gpio_t pio, bool is_export) {
    int fd;
    if (is_export) {
        fd = open(GPIO_SYS_DIR "/export", O_WRONLY);
    } else {
        fd = open(GPIO_SYS_DIR "/unexport", O_WRONLY);
    }    
    if (fd < 0) {
        fprintf(stderr, "GPIO export file open failed\n");
        return fd;
    }

    char cmd[32] = {0};
    sprintf(cmd, "%d", pio);

    if (write(fd, cmd, 32) != 32) {
        fprintf(stderr, "˜Failed to write to export file on pin %d\n", pio);
        close(fd);
        return fd;
    }
    close(fd);
    return 0;
}

static inline int set_gpio_dir(const jetson_gpio_t pio, bool output) {
    int fd;
    char dir_file_name[256] = {0};
    sprintf(dir_file_name, GPIO_SYS_DIR "/gpio%d/direction", pio);

    fd = open(dir_file_name, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open direction setting file on pin %d\n", pio);
        return fd;
    }

    char cmd[32] = {0};
    if (output) {
        sprintf(cmd, "out");
    } else {
        sprintf(cmd, "in");
    }
    if (write(fd, cmd, 32) != 32) {
        fprintf(stderr, "Failed to write to direction file on pin %d\n", pio);
        close(fd);
        return fd;
    }
    close(fd);
    return 0;
}

static int rasie_priviledge() {
    cap_t caps;
    cap_value_t cap_value;
    if (!CAP_IS_SUPPORTED(CAP_DAC_OVERRIDE)) {
        fprintf(stderr, "Linux CAP is not supported\n");
        return -1;
    }
    caps = cap_get_proc();
    if (caps == NULL) {
        fprintf(stderr, "cap_get_proc failed\n");
        return -1;
    }
    cap_value = CAP_DAC_OVERRIDE;
    if (cap_set_flag(caps, CAP_EFFECTIVE, 1, &cap_value, CAP_SET) == -1) {
        fprintf(stderr, "cap_set_flag failed\n");
        return -2;
    }
    if (cap_set_proc(caps) == -1) {
        fprintf(stderr, "cep_set_proc failed\n");
        return -3;
    }
    if (cap_free(caps) == -1) {
        fprintf(stderr, "cep_free failed\n");
        return -4;
    }
    return 0;
}

static int drop_priviledge() {

}

int gpio_open(cepton_gpio_t* const gpio_s) {
    if (gpio_s->gpio < GPIO_MIN) {
        fprintf(stderr, "Selected gpio pin not valid\n");
        return -1;
    }
    int erro;
    // if (erro = rasie_priviledge() != 0) {
    //     fprintf(stderr, "FAiled to raise priviledge\n");
    //     fprintf(stderr, "erro number: %d\n", erro);
    // }

    
    erro = set_gpio_export(gpio_s->gpio, true);
    if (erro != 0) {
        fprintf(stderr, "Erro exporting GPIO pin %d\n", gpio_s->gpio);
        return erro;
    }

    erro = set_gpio_dir(gpio_s->gpio, gpio_s->output);
    if (erro != 0) {
        fprintf(stderr, "Erro setting GPIO output mode on pin %d\n", gpio_s->gpio);
        set_gpio_export(gpio_s->gpio, false);
        return erro;
    }

    char value_file_name[256] = {0};
    sprintf(value_file_name, GPIO_SYS_DIR "/gpio%d/value", gpio_s->gpio);
    int fd;
    if (gpio_s->output) {
        fd = open(value_file_name, O_WRONLY);
    } else {
        fd = open(value_file_name, O_RDONLY);
    }
    if (fd < 0) {
        fprintf(stderr, "Erro opening value file on pin %d\n", gpio_s->gpio);
        return fd;
    }
    gpio_s->fd = fd;
    return 0;
}

int gpio_close(const cepton_gpio_t gpio_s) {
    if (gpio_s.fd != -1) {
        close(gpio_s.fd);
    }    
    return set_gpio_export(gpio_s.gpio, false);
}

int gpio_read(const cepton_gpio_t gpio_s, uint8_t* const buff) {
    return read(gpio_s.fd, (void *) buff, 1);
}

int gpio_write(const cepton_gpio_t gpio_s, const uint8_t value) {
    uint8_t cmd = value ? '1' : '0';
    return write(gpio_s.fd, (void *) (&cmd), 1);
}