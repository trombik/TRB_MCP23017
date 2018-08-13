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

static uint8_t
bit_write8(const uint8_t old, const uint8_t pos, const uint8_t value)
{
	return (old & ~(1 << pos)) | ((value & 1) << pos);
}

int32_t
mcp23017_set_pin_direction(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t direction)
{
	int32_t r;
	uint8_t reg, value, enable_pullup;
	if (port > 1) {
		r = EINVAL;
		goto fail;
	}

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

	reg = MCP23x17_IODIR + port;
	if ((r = mcp23017_set_bit(dev, reg, value, pin_num)) != 0) {
		goto fail;
	}
	if (enable_pullup == 1) {
		if ((r = mcp23017_enable_pin_pullup(dev, port, pin_num)) != 0) {
			goto fail;
		}
	}
fail:
	return r;
}

int32_t
mcp23017_set_pin_level(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t level)
{
	int32_t r;
	uint8_t reg, old, new_value;
	if (level != LOW && level != HIGH) {
		r = EINVAL;
		goto fail;
	}
	if (port > 1) {
		r = EINVAL;
		goto fail;
	}
	reg = MCP23x17_OLAT + port;
	if ((r = mcp23017_read8(dev, reg, &old)) != 0) {
		goto fail;
	}
	new_value = bit_write8(old, pin_num, level);
	if (new_value != old) {
		if ((r = mcp23017_write8(dev, reg, new_value)) != 0)
			goto fail;
	}
fail:
	return r;
}

int32_t
mcp23017_enable_pin_intrrupt(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t default_value, const mcp_int_condition_t condition)
{
	uint8_t reg;
	int32_t r;
	if (pin_num >= 8 || default_value > 1 || port > 1) {
		r = EINVAL;
		goto fail;
	}

	/* set default_value */
	reg = MCP23x17_DEFVAL + port;
	if ((r = mcp23017_set_bit(dev, reg, default_value, pin_num)) != 0) {
		goto fail;
	}

	/* set condition */
	reg = MCP23x17_INTCON + port;
	if ((r = mcp23017_set_bit(dev, reg, condition, pin_num)) != 0) {
		goto fail;
	}

	/* enable intrrupt */
	reg = MCP23x17_GPINTEN + port;
	if ((r = mcp23017_set_bit(dev, reg, condition, pin_num)) != 0) {
		goto fail;
	}
fail:
	return r;
}

int32_t
mcp23017_set_bit(const mcp23017_dev_t *dev, const uint8_t reg, const uint8_t value, const uint8_t pos)
{
	int32_t r;
	uint8_t old_value, new_value;

	if (pos > 8 || value > 1) {
		r = EINVAL;
		goto fail;
	}

	if ((r = mcp23017_read8(dev, reg, &old_value)) != 0) {
		goto fail;
	}
	new_value = bit_write8(old_value, pos, value & 1);
	if (new_value != old_value) {
		if ((r = mcp23017_write8(dev, reg, new_value)) != 0) {
			goto fail;
		}
	}
fail:
	return r;

}

int32_t
mcp23017_get_bit(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t *value, const uint8_t pos)
{
	int32_t r;
	uint8_t reg_value;

	if (pos > 8) {
		r = EINVAL;
		goto fail;
	}

	if ((r = mcp23017_read8(dev, reg, &reg_value)) != 0) {
		goto fail;
	}
	*value = (reg_value >> pos ) & 1;
fail:
	return r;

}

static int32_t
mcp23017_set_pullup_value(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t value)
{
	int32_t r;
	uint8_t reg;
	reg = MCP23x17_GPPU + port;

	if ((r = mcp23017_set_bit(dev, reg, value & 1, pin_num)) != 0) {
		goto fail;
	}
fail:
	return r;
}

/*
 * \brief Disable pullup on a pin
 *
 * \param pin_num : Pin number
 */
int32_t
mcp23017_disable_pin_pullup(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num)
{
	return mcp23017_set_pullup_value(dev, port, pin_num, 0);
}

/*
 * \brief Enable pullup on a pin
 *
 * \param pin_num : Pin number
 */
int32_t
mcp23017_enable_pin_pullup(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num)
{
	return mcp23017_set_pullup_value(dev, port, pin_num, 1);
}

int32_t
mcp23017_flip_pin(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num)
{
	int32_t r;
	uint8_t reg_value;
	if (port > 1 || pin_num >= 8) {
		r = EINVAL;
		goto fail;
	}
	if ((r = mcp23017_get_bit(dev, MCP23x17_OLAT + port, &reg_value, pin_num)) != 0) {
		goto fail;
	}
	if ((r = mcp23017_set_bit(dev, MCP23x17_OLAT + port, !reg_value, pin_num)) != 0) {
		goto fail;
	}
fail:
	return r;
}

#if defined(__cplusplus)
}
#endif
