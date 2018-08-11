#include <Arduino.h>
#include <brzo_i2c.h>
#include <errno.h>
#include <brzo_i2c.h>
#include "TRB_MCP23017.h"

#define WITH_REPEATED_START true
#define WITHOUT_REPEATED_START false

int32_t
mcp23017_read8(const uint8_t reg, uint8_t *value)
{
	int32_t err;
	uint8_t reg_addr_copy;

	/* avoid const violation */
	reg_addr_copy = reg;
	if (mcp23017_get_i2c_freq() == 0) {
		err = EINVAL;
		goto fail;
	}

	/* start transaction */
	brzo_i2c_start_transaction(mcp23017_get_i2c_address(), mcp23017_get_i2c_freq());

	/* write register address with repeated start */
	brzo_i2c_write(&reg_addr_copy, 1, WITH_REPEATED_START);

	/* read the values */
	brzo_i2c_read(value, 1, WITHOUT_REPEATED_START);

	/* send the I2C transaction */
	err = brzo_i2c_end_transaction();
fail:
	return err;
}

int32_t
mcp23017_write8(const uint8_t reg, uint8_t value)
{
	int32_t err;
	uint8_t buffer[2];
	if (mcp23017_get_i2c_freq() == 0) {
		err = EINVAL;
		goto fail;
	}

	buffer[0] = reg;
	buffer[1] = value;

	/* start transaction */
	brzo_i2c_start_transaction(mcp23017_get_i2c_address(), mcp23017_get_i2c_freq());

	/* write register address + the value */
	brzo_i2c_write(buffer, 2, WITHOUT_REPEATED_START);

	/* send the I2C transaction */
	err = brzo_i2c_end_transaction();
fail:
	return err;
}
