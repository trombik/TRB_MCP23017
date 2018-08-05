#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "TRB_MCP23017.h"

#if defined(TRB_MCP23017_ESP_IDF)
#include "sys/esp_idf/i2c.h"
#endif

int8_t
mcp23017_init(struct mcp23017_cxt_t *ctx)
{
	int8_t r;
	if (ctx == NULL) {
		r = EINVAL;
		goto fail;
	}
	struct mcp23017_i2c_t *i2c_config = (struct mcp23017_i2c_t *)malloc(sizeof(struct mcp23017_i2c_t));
	if (i2c_config == NULL) {
		r = ENOMEM;
		goto fail;
	}
	memset(i2c_config, 0, sizeof(struct mcp23017_i2c_t));
	ctx->i2c_config = i2c_config;
	ctx->err_str = NULL;
	ctx->set_pin_direction = mcp23017_set_pin_direction;
	ctx->set_pin_level = mcp23017_set_pin_level;
	r = 0;
fail:
	return r;
}

struct mcp23017_cxt_t *
mcp23017_new(void)
{
	struct mcp23017_cxt_t *ctx = (struct mcp23017_cxt_t *)malloc(sizeof(struct mcp23017_cxt_t));
	if (ctx == NULL)
		goto fail;
	if (mcp23017_init(ctx) != 0) {
		free(ctx);
		ctx = NULL;
		goto fail;
	}
fail:
	return ctx;
}

void
mcp23017_free(struct mcp23017_cxt_t *ctx)
{
	if (ctx == NULL)
		return;
	if (ctx->i2c_config != NULL)
		free(ctx->i2c_config);
	free(ctx);
	return;
}

static uint8_t
bit_write8(const uint8_t old, const uint8_t pos, const uint8_t value)
{
	return (old & ~(1 << pos)) | ((value & 1) << pos);
}

int32_t
mcp23017_set_pin_direction(struct mcp23017_cxt_t *ctx, const uint8_t pin_num, const uint8_t direction)
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
		ctx->err_str = (char *)"mcp23017_set_pin_direction(): value should be INPUT_PULLUP, INPUT, or OUTPUT";
		r = EINVAL;
		goto fail;
	}

	reg = pin_num < 8 ? MCP23x17_IODIR : MCP23x17_IODIR + 1;
	pos = pin_num < 8 ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(ctx, reg, &old)) != 0) {
		goto fail;
	}
	new = bit_write8(old, pos, value);
	if (new != old) {
		if ((r = mcp23017_write8(ctx, reg, new)) != 0) {
			goto fail;
		}
	}
	if (enable_pullup == 1) {
		if ((r = mcp23017_enable_pullup(ctx, pin_num)) != 0) {
			goto fail;
		}
	}
fail:
	return r;
}

int32_t
mcp23017_set_pin_level(struct mcp23017_cxt_t *ctx, const uint8_t pin_num, const uint8_t level)
{
	int32_t r;
	uint8_t reg, pos, old, new;
	if (level != LOW && level != HIGH) {
		r = EINVAL;
		ctx->err_str = (char *)"level must be either HIGH or LOW";
		goto fail;
	}
	reg = pin_num < 8 ? MCP23x17_OLAT : MCP23x17_OLAT + 1;
	pos = pin_num < 8 ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(ctx, reg, &old)) != 0) {
		goto fail;
	}
	new = bit_write8(old, pos, level);
	if (new != old) {
		if ((r = mcp23017_write8(ctx, reg, new)) != 0)
			goto fail;
	}
fail:
	return r;
}

static int32_t
mcp23017_set_pullup_value(struct mcp23017_cxt_t *ctx, const uint8_t pin_num, const uint8_t value)
{
	int32_t r;
	uint8_t reg, pos, old, new;
	reg = (pin_num < 8) ? MCP23x17_GPPU : MCP23x17_GPPU + 1;
	pos = (pin_num < 8) ? pin_num : pin_num - 8;
	if ((r = mcp23017_read8(ctx, reg, &old)) != 0) {
		goto fail;
	}
	new = bit_write8(old, pos, value & 1);
	if (new != old) {
		if ((r = mcp23017_write8(ctx, reg, new)) != 0) {
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
 * \param ctx : Pointer to mcp23017_cxt_t
 * \param pin_num : Pin number
 */
int32_t
mcp23017_disable_pullup(struct mcp23017_cxt_t *ctx, const uint8_t pin_num)
{
	return mcp23017_set_pullup_value(ctx, pin_num, 0);
}

/*
 * \brief Enable pullup on a pin
 *
 * \param ctx : Pointer to mcp23017_cxt_t
 * \param pin_num : Pin number
 */
int32_t
mcp23017_enable_pullup(struct mcp23017_cxt_t *ctx, const uint8_t pin_num)
{
	return mcp23017_set_pullup_value(ctx, pin_num, 1);
}
