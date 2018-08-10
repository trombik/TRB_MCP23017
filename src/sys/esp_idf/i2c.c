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

#if defined(TRB_MCP23017_ESP_IDF)

#define ACK_CHECK_ENABLE 0x01

#include <driver/i2c.h>
#include <esp_log.h>
#include "i2c.h"
#include "../../TRB_MCP23017.h"

int32_t
mcp23017_read8(const uint8_t reg, uint8_t *value)
{
	int32_t r = 0;
	uint16_t address;
	i2c_cmd_handle_t command;

	address = mcp23017_get_i2c_address();
	command = i2c_cmd_link_create();
	if ((r = i2c_master_start(command)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_write_byte( command, (address << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_write_byte( command, reg, ACK_CHECK_ENABLE)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_start(command)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_write_byte(command, (address << 1) | I2C_MASTER_READ, ACK_CHECK_ENABLE)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_read(command, value, 1, I2C_MASTER_NACK)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_stop(command)) != ESP_OK) {
		goto fail;
	}

	r = i2c_master_cmd_begin(mcp23017_get_i2c_port(), command, 10 / portTICK_PERIOD_MS);
	if (r != ESP_OK) {
		ESP_LOGE(__func__, "i2c_master_cmd_begin(): %d", r);
	}
fail:
	i2c_cmd_link_delete(command);
	return r;
}

int32_t
mcp23017_write8(const uint8_t reg, uint8_t value)
{
	int32_t r = 0;
	uint16_t address;
	i2c_cmd_handle_t command;
	address = mcp23017_get_i2c_address();

	command = i2c_cmd_link_create();
	if ((r = i2c_master_start(command)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_write_byte(command, (address << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_write_byte(command, reg, ACK_CHECK_ENABLE)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_write(command, &value, 1, ACK_CHECK_ENABLE)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_stop(command)) != ESP_OK) {
		goto fail;
	}
	if ((r = i2c_master_cmd_begin(mcp23017_get_i2c_port(), command, 10 / portTICK_PERIOD_MS)) != ESP_OK) {
		ESP_LOGE(__func__, "i2c_master_cmd_begin(): %d", r);
		goto fail;
	}
fail:
	i2c_cmd_link_delete(command);
	return r;
}
#endif // defined(TRB_MCP23017_ESP_IDF)
