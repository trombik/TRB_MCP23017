#include <Arduino.h>
#include <brzo_i2c.h>
#include <errno.h>
#include <brzo_i2c.h>
#include "TRB_MCP23017.h"

#define WITH_REPEATED_START true
#define WITHOUT_REPEATED_START false

int32_t
mcp23017_read8(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t *value)
{
	int32_t err;
	uint8_t reg_addr_copy;

	/* avoid const violation */
	reg_addr_copy = dev->address;

	/* start transaction */
	brzo_i2c_start_transaction(reg_addr_copy, dev->i2c_config->freq);

	/* write register address with repeated start */
	brzo_i2c_write(&reg_addr_copy, 1, WITH_REPEATED_START);

	/* read the values */
	brzo_i2c_read(value, 1, WITHOUT_REPEATED_START);

	/* send the I2C transaction */
	err = brzo_i2c_end_transaction();
	return err;
}

int32_t
mcp23017_write8(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t value)
{
	int32_t err;
	uint8_t buffer[2];
	buffer[0] = reg;
	buffer[1] = value;

	/* start transaction */
	brzo_i2c_start_transaction(dev->address, dev->i2c_config->freq);

	/* write register address + the value */
	brzo_i2c_write(buffer, 2, WITHOUT_REPEATED_START);

	/* send the I2C transaction */
	err = brzo_i2c_end_transaction();
	return err;
}
