#include <esp_err.h>
#include <driver/gpio.h>

esp_err_t
init_reset_pin(gpio_num_t pin);

esp_err_t
reset_ic(gpio_num_t pin);
