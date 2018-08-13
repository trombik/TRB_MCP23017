#include <stdint.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "sdkconfig.h"

#include <TRB_MCP23017.h>

#define DONTCARE_ZERO   (0)

int32_t
i2c_init(const mcp23017_i2c_config_t config)
{
	int32_t r;
	i2c_config_t i2c_config;

	i2c_config.mode = I2C_MODE_MASTER;
	i2c_config.sda_io_num = config.sda;
	i2c_config.scl_io_num = config.scl;
	i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
	i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
	i2c_config.master.clk_speed = 100000L;

	if ((r = i2c_param_config(config.i2c_port, &i2c_config)) != 0) {
		ESP_LOGE(__func__, "i2c_param_config(): %d", r);
		goto fail;
	}
	if ((r = i2c_driver_install(
	    config.i2c_port, i2c_config.mode,
	    DONTCARE_ZERO, DONTCARE_ZERO, DONTCARE_ZERO)) != 0) {
		ESP_LOGE(__func__, "i2c_driver_install(): %d", r);
		goto fail;
	}
fail:
	return r;
}

void
task_blink(void *pvParameters)
{
	uint8_t reg_value;
	uint8_t level = 0;
	int32_t err;
	mcp23017_i2c_config_t config;
	mcp23017_dev_t *dev;

	config.scl = GPIO_SCL;
	config.sda = GPIO_SDA;
	config.i2c_port = I2C_NUM_0;
	dev = calloc(1, sizeof(mcp23017_dev_t));
	if (dev == NULL) {
		ESP_LOGE(__func__, "Out of memory");
		goto fail;
	}
	dev->i2c_config = &config;
	dev->address = MCP23017_I2C_ADDRESS_DEFAULT;

	ESP_LOGI(__func__, "Initializing I2C.");
	if ((err = i2c_init(config)) != 0) {
		ESP_LOGE(__func__, "i2c_init(): %d", err);
		goto fail;
	}

	ESP_LOGI(__func__, "Read IODIRA.");
	if ((err = mcp23017_read8(dev, MCP23x17_IODIRA, &reg_value)) != 0) {
		ESP_LOGE(__func__, "mcp23017_read8(): %d", err);
		goto fail;
	}
	if (reg_value != 0xff) {
		err = -1;
		ESP_LOGE(__func__, "IODIRA: %d", reg_value);
		goto fail;
	}
	for (uint8_t pin = 0; pin <= 7; pin++) {
		ESP_LOGI(__func__, "Set pin %d to OUTPUT", pin);
		err = mcp23017_set_pin_direction(dev, pin, OUTPUT);
		if (err != 0) {
			ESP_LOGE(__func__, "mcp23017_set_pin_direction(): %d", err);
			goto fail;
		}
	}
	ESP_LOGI(__func__, "Starting loop");
	while (1) {
		level ^= 1;
		for (uint8_t pin = 0; pin <= 7; pin++) {
			vTaskDelay(500 / portTICK_PERIOD_MS);
			err = mcp23017_set_pin_level(dev, pin, level);
			if (err != 0) {
				ESP_LOGE(__func__, "mcp23017_set_pin_level(): %d", err);
				goto fail;
			}
		}
	}
fail:
	free(dev);
	ESP_LOGE(__func__, "Test failed.");
	vTaskDelete(NULL);
}

void
app_main(void)
{
	TaskHandle_t task_h;
	gpio_config_t io_conf;

	vTaskDelay(3000 / portTICK_PERIOD_MS);

	/* reset the IC */
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = (1 << RESET_PIN);
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf);
	gpio_set_level(RESET_PIN, LOW);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	gpio_set_level(RESET_PIN, HIGH);

	/* create the task */
	xTaskCreate(
	    task_blink,
	    "task_blink",
	    configMINIMAL_STACK_SIZE * 3,
	    NULL,
	    1,
	    &task_h);
}
