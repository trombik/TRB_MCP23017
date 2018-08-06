#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(TRB_MCP23017_ESP_IDF)
/* required for i2c_port_t */
#include <driver/i2c.h>
#endif

#include "TRB_MCP23017.h"

typedef struct
{
	mcp23017_i2c_config_t *i2c_config;
} mcp23017_config_t;

static mcp23017_config_t *config;

int8_t
mcp23017_init()
{
	int8_t r;
	config = calloc(1, sizeof(mcp23017_config_t));
	if (config == NULL) {
		r = ENOMEM;
		goto calloc_fail;
	}
	config->i2c_config = calloc(1, sizeof(mcp23017_i2c_config_t));
	if (config->i2c_config == NULL) {
		free(config);
		config = NULL;
		r = ENOMEM;
		goto calloc_fail;
	}
	r = 0;
calloc_fail:
	return r;
}

void
mcp23017_free()
{
	free(config->i2c_config);
	free(config);
	config = NULL;
	return;
}

int8_t
mcp23017_set_i2c_config(const mcp23017_i2c_config_t *new)
{
	int8_t r;
	if (new == NULL) {
		r = EINVAL;
		goto fail;
	}
	*(config->i2c_config) = *new;
	r = 0;
fail:
	return r;
}

static uint8_t
bit_write8(const uint8_t old, const uint8_t pos, const uint8_t value)
{
	return (old & ~(1 << pos)) | ((value & 1) << pos);
}

int32_t
mcp23017_set_pin_direction(const uint8_t pin_num, const uint8_t direction)
{
	int32_t r;
	uint8_t reg, value, pos, old, new, enable_pullup;

	enable_pullup = 0;
	switch (direction) {
	case INPUT_PULLUP:
		enable_pullup = 1;
		/* FALL THROUGH */
	case INPUT:
		value = 1;
		break;
	case OUTPUT:
		value = 0;
		break;
	default:
		r = EINVAL;
		goto fail;
	}

	reg = pin_num < 8 ? MCP23x17_IODIR : MCP23x17_IODIR + 1;
	pos = pin_num < 8 ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(reg, &old)) != 0) {
		goto fail;
	}
	new = bit_write8(old, pos, value);
	if (new != old) {
		if ((r = mcp23017_write8(reg, new)) != 0) {
			goto fail;
		}
	}
	if (enable_pullup == 1) {
		if ((r = mcp23017_enable_pullup(pin_num)) != 0) {
			goto fail;
		}
	}
fail:
	return r;
}

int32_t
mcp23017_set_pin_level(const uint8_t pin_num, const uint8_t level)
{
	int32_t r;
	uint8_t reg, pos, old, new;
	if (level != LOW && level != HIGH) {
		r = EINVAL;
		goto fail;
	}
	reg = pin_num < 8 ? MCP23x17_OLAT : MCP23x17_OLAT + 1;
	pos = pin_num < 8 ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(reg, &old)) != 0) {
		goto fail;
	}
	new = bit_write8(old, pos, level);
	if (new != old) {
		if ((r = mcp23017_write8(reg, new)) != 0)
			goto fail;
	}
fail:
	return r;
}

static int32_t
mcp23017_set_pullup_value(const uint8_t pin_num, const uint8_t value)
{
	int32_t r;
	uint8_t reg, pos, old, new;
	reg = (pin_num < 8) ? MCP23x17_GPPU : MCP23x17_GPPU + 1;
	pos = (pin_num < 8) ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(reg, &old)) != 0) {
		goto fail;
	}
	new = bit_write8(old, pos, value & 1);
	if (new != old) {
		if ((r = mcp23017_write8(reg, new)) != 0) {
			goto fail;
		}
	}
fail:
	r = 0;
	return r;
}

/*
 * \brief Disable pullup on a pin
 *
 * \param pin_num : Pin number
 */
int32_t
mcp23017_disable_pullup(const uint8_t pin_num)
{
	return mcp23017_set_pullup_value(pin_num, 0);
}

/*
 * \brief Enable pullup on a pin
 *
 * \param pin_num : Pin number
 */
int32_t
mcp23017_enable_pullup(const uint8_t pin_num)
{
	return mcp23017_set_pullup_value(pin_num, 1);
}

uint16_t
mcp23017_get_i2c_address()
{
	return config->i2c_config->address;
}

#if defined(TRB_MCP23017_ESP_IDF)
i2c_port_t
mcp23017_get_i2c_port()
{
	return config->i2c_config->i2c_port;
}
#endif
