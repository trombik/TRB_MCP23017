#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdint.h>
#include "unity.h"
#include "../reset.h"

#include "TRB_MCP23017.h"

#define GPA0	0 // physical pin 21 in DIP package, 8th from top on right side

static i2c_port_t i2c_port = I2C_NUM_0;
static char component[] = "[TRB_MCP23017]";
uint8_t reg_value;
uint32_t r;
struct mcp23017_cxt_t *mcp;

uint32_t
i2c_init()
{
	i2c_config_t i2c_config;

	i2c_config.mode = I2C_MODE_MASTER;
	i2c_config.sda_io_num = GPIO_SDA;
	i2c_config.scl_io_num = GPIO_SCL;
	i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
	i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
	i2c_config.master.clk_speed = 100000L; // 100KHz

	TEST_ASSERT_EQUAL_INT8(0, i2c_param_config(i2c_port, &i2c_config));
	TEST_ASSERT_EQUAL_INT8(0, i2c_driver_install(i2c_port, i2c_config.mode, 0, 0, 0));
	TEST_ASSERT_EQUAL(I2C_NUM_0, mcp23017_set_i2c_port(i2c_port));
	return 0;
}

TEST_CASE("i2c_init", component)
{
	TEST_ASSERT_EQUAL_INT32(0, i2c_init());
	TEST_ASSERT_EQUAL_INT32(0, i2c_driver_delete(i2c_port));
}

TEST_CASE("mcp23017_new", component)
{
	TEST_ASSERT_NOT_NULL(mcp = mcp23017_new());
	mcp23017_free(mcp);
}

TEST_CASE("mcp23017_get_i2c_port", component)
{
	TEST_ASSERT_NOT_NULL(mcp = mcp23017_new());
	TEST_ASSERT(mcp23017_get_i2c_port() == I2C_NUM_0);
	mcp23017_set_i2c_port(I2C_NUM_1);
	TEST_ASSERT(mcp23017_get_i2c_port() == I2C_NUM_1);
	TEST_ASSERT(mcp23017_set_i2c_port(I2C_NUM_0) == I2C_NUM_0);
	mcp23017_free(mcp);
}

TEST_CASE("mcp23017_read8", component)
{
	TEST_ASSERT_EQUAL_INT32(0, init_reset_pin(RESET_PIN));
	TEST_ASSERT_EQUAL_INT32(0, reset_ic(RESET_PIN));
	TEST_ASSERT_NOT_NULL(mcp = mcp23017_new());
	mcp->i2c_config->address = MCP23017_I2C_ADDRESS_DEFAULT;
	TEST_ASSERT_EQUAL_INT32(0, i2c_init());
	TEST_ASSERT_EQUAL_INT32(0, mcp23017_read8(mcp, MCP23x17_IODIRA, &reg_value));
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(0xff, reg_value, "MCP23x17_IODIRA has default value");
	mcp23017_free(mcp);
	TEST_ASSERT_EQUAL_INT32(0, i2c_driver_delete(i2c_port));
}
TEST_CASE("output", component)
{
	TEST_ASSERT_EQUAL_INT32(0, init_reset_pin(RESET_PIN));
	TEST_ASSERT_EQUAL_INT32(0, reset_ic(RESET_PIN));
	TEST_ASSERT_NOT_NULL(mcp = mcp23017_new());
	mcp->i2c_config->address = MCP23017_I2C_ADDRESS_DEFAULT;
	mcp->i2c_config->scl = GPIO_SCL;
	mcp->i2c_config->sda = GPIO_SDA;
	mcp->i2c_config->i2c_port = I2C_NUM_0;
	TEST_ASSERT_EQUAL_INT32(0, i2c_init());
	TEST_ASSERT_EQUAL_INT32(0, mcp23017_set_pin_direction(mcp, 0, INPUT_PULLUP));
	TEST_ASSERT_EQUAL_INT32(0, mcp23017_set_pin_direction(mcp, 0, OUTPUT));
	for (uint8_t i = 1; i <= 10; i++) {
		mcp23017_set_pin_level(mcp, 0, (i % 2) == 0 ? LOW : HIGH);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	mcp23017_free(mcp);
	TEST_ASSERT_EQUAL_INT32(0, i2c_driver_delete(i2c_port));
}

