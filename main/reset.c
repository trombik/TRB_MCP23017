#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_err.h>
#include <driver/gpio.h>

static gpio_config_t reset_pin_config;

esp_err_t
init_reset_pin(gpio_num_t pin)
{
	esp_err_t r;
	reset_pin_config.mode = GPIO_MODE_OUTPUT;
	reset_pin_config.intr_type = GPIO_PIN_INTR_DISABLE;
	reset_pin_config.pin_bit_mask = (1 << pin);
	reset_pin_config.pull_down_en = 0;
	reset_pin_config.pull_up_en = 0;
	r = gpio_config(&reset_pin_config);
	if (r != ESP_OK) {
	goto fail;
	}
	r = gpio_set_level(pin, 1);
fail:
	return r;
}

esp_err_t
reset_ic(gpio_num_t pin)
{
	esp_err_t r;

	r = gpio_set_level(pin, 0);
	if (r != ESP_OK)
	goto fail;
	vTaskDelay(100 / portTICK_PERIOD_MS);
	r = gpio_set_level(pin, 1);
fail:
	return r;
}
