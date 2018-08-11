/*
 * Copyright (c) 2018 Tomoyuki Sakurai <y@trombik.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(TRB_MCP23017_ESP_IDF)
/* required for i2c_port_t */
#include <driver/i2c.h>
#endif // defined(TRB_MCP23017_ESP_IDF)

#if defined(ARDUINO)
#include <Arduino.h>
#endif // defined(ARDUINO)

#include "TRB_MCP23017.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
	mcp23017_i2c_config_t *i2c_config;
} mcp23017_config_t;

static mcp23017_config_t *config;

int8_t
mcp23017_init()
{
	int8_t r;
	config = (mcp23017_config_t *)calloc(1, sizeof(mcp23017_config_t));
	if (config == NULL) {
		r = ENOMEM;
		goto calloc_fail;
	}
	config->i2c_config = (mcp23017_i2c_config_t *)calloc(1, sizeof(mcp23017_i2c_config_t));
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
	if (config != NULL) {
		free(config->i2c_config);
	}
	free(config);
	config = NULL;
	return;
}

int8_t
mcp23017_set_i2c_config(const mcp23017_i2c_config_t *new_config)
{
	int8_t r;
	if (config == NULL || new_config == NULL) {
		r = EINVAL;
		goto fail;
	}
	*(config->i2c_config) = *new_config;
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
	uint8_t reg, value, pos, old, new_value, enable_pullup;

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
	new_value = bit_write8(old, pos, value);
	if (new_value != old) {
		if ((r = mcp23017_write8(reg, new_value)) != 0) {
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
	uint8_t reg, pos, old, new_value;
	if (level != LOW && level != HIGH) {
		r = EINVAL;
		goto fail;
	}
	reg = pin_num < 8 ? MCP23x17_OLAT : MCP23x17_OLAT + 1;
	pos = pin_num < 8 ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(reg, &old)) != 0) {
		goto fail;
	}
	new_value = bit_write8(old, pos, level);
	if (new_value != old) {
		if ((r = mcp23017_write8(reg, new_value)) != 0)
			goto fail;
	}
fail:
	return r;
}

static int32_t
mcp23017_set_pullup_value(const uint8_t pin_num, const uint8_t value)
{
	int32_t r;
	uint8_t reg, pos, old, new_value;
	reg = (pin_num < 8) ? MCP23x17_GPPU : MCP23x17_GPPU + 1;
	pos = (pin_num < 8) ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(reg, &old)) != 0) {
		goto fail;
	}
	new_value = bit_write8(old, pos, value & 1);
	if (new_value != old) {
		if ((r = mcp23017_write8(reg, new_value)) != 0) {
			goto fail;
		}
	}
	r = 0;
fail:
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

/*
 * \brief Get configured I2C clock frequency
 */

uint16_t
mcp23017_get_i2c_freq()
{
	return config->i2c_config->freq;
}

#if defined(TRB_MCP23017_ESP_IDF)
i2c_port_t
mcp23017_get_i2c_port()
{
	return config->i2c_config->i2c_port;
}
#endif // defined(TRB_MCP23017_ESP_IDF)

#if defined(__cplusplus)
}
#endif
