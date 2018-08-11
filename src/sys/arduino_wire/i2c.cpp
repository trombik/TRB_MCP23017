#include <Arduino.h>
#include <Wire.h>
#include <errno.h>
#include <stdint.h>
#include "TRB_MCP23017.h"

/* arduino-esp8266 has rather old Wire library and lacks Wire.getClock(). */
#if defined(ARDUINO_ARCH_ESP32)
#define HAS_WIRE_GETCLOCK
#endif

#define WITH_REPEATED_START	false

/* Optionally support optimized SCL frequency for this particular IC */
#if defined(WITH_DYNAMIC_I2C_FREQ)
static size_t saved_freq = 0;

/*
 * \brief Set configured frequency in i2c_config, restore the old value if
 * possible.
 */
static uint8_t
set_freq()
{
	uint8_t err;
	uint16_t freq;

	freq = mcp23017_get_i2c_freq();
	if (freq == 0) {
		err = EINVAL;
		goto fail;
	}
#if defined(HAS_WIRE_GETCLOCK)
	saved_freq = Wire.getClock();
#endif
	Wire.setClock(freq * 1000);
	err = 0;
fail:
	return err;
}

static void
restore_freq()
{
	if (saved_freq != 0) {
		Wire.setClock(saved_freq);
	}
}
#endif // defined(WITH_DYNAMIC_I2C_FREQ)

int32_t
mcp23017_read8(const uint8_t reg, uint8_t *value)
{
	int32_t err = 0;
	const static uint8_t WITH_STOP_BIT = 1;
	uint8_t length = 1;
	uint8_t addr;

	addr = mcp23017_get_i2c_address();
#if defined(WITH_DYNAMIC_I2C_FREQ)
	if ((err = set_freq()) != 0) {
		goto fail;
	}
#endif
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
#if defined(WITH_DYNAMIC_I2C_FREQ)
	restore_freq();
#endif
	return err;
}

int32_t
mcp23017_write8(const uint8_t reg, uint8_t value)
{
	int32_t err = 0;
	uint8_t length = 1;
	uint8_t addr;

	addr = mcp23017_get_i2c_address();
#if defined(WITH_DYNAMIC_I2C_FREQ)
	if ((err = set_freq()) != 0) {
		goto fail;
	}
#endif
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
#if defined(WITH_DYNAMIC_I2C_FREQ)
	restore_freq();
#endif
	return err;
}
