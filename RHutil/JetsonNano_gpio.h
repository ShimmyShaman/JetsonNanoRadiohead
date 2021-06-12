#ifndef JETSON_NANO_GPIO_H
#define JETSON_NANO_GPIO_H

// #define RH_HAVE_HARDWARE_SPI

int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_fd_open(unsigned int gpio);
int gpio_fd_close(int fd);

void ensure_export(unsigned int gpio);
void ensure_set_dir(unsigned int gpio, unsigned int out_flag);
void ensure_set_value(unsigned int gpio, unsigned int value);

#endif // JETSON_NANO_GPIO_H