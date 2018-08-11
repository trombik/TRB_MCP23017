#include <Arduino.h>
#include <Wire.h>
#include <errno.h>

#define WITH_REPEATED_START	false

int32_t
mcp23017_read8(const uint8_t reg, uint8_t *value)
{
	int32_t err = 0;
	const static uint8_t WITH_STOP_BIT = 1;
	uint8_t length = 1;
	uint16_t addr;

	addr = mcp23017_get_i2c_address();
	/* start transaction */
	Wire.beginTransmission(addr);
	/* write register address */
	if ((err = Wire.write(reg)) != 1) {
		err = EIO;
		goto fail;
	}
	/* read the values with repeated start, and end the transaction with stop
	* bit */
	if ((err = Wire.endTransmission(WITH_REPEATED_START)) != 0) {
		goto fail;
	}
	if (Wire.requestFrom(addr, length, WITH_STOP_BIT) != 1) {
		err = EIO;
		goto fail;
	}
	*value = Wire.read();
fail:
	return err;
}

int32_t
mcp23017_write8(const uint8_t reg, uint8_t value)
{
	int32_t err = 0;
	uint8_t length = 1;
	uint16_t addr;

	addr = mcp23017_get_i2c_address();
	/* start transaction */
	Wire.beginTransmission(addr);
	/* write register address */
	if ((err = Wire.write(reg)) != 1) {
		err = EIO;
		goto fail;
	}
	/* write values */
	if ((err = Wire.write(&value, length)) != 1) {
		err = EIO;
		goto fail;
	}
	/* end end transaction */
	err = Wire.endTransmission();
fail:
	return err;
}
