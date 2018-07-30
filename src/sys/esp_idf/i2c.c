#if defined(TRB_MCP23017_ESP_IDF)

#define ACK_CHECK_ENABLE 0x01

#include <driver/i2c.h>
#include <esp_log.h>
#include "i2c.h"
#include "../../TRB_MCP23017.h"

static i2c_port_t port = I2C_NUM_0;

i2c_port_t
mcp23017_get_i2c_port()
{
	return port;
}

void
mcp23017_set_i2c_port(i2c_port_t p)
{
	port = p;
	return;
}

int32_t
mcp23017_read8(struct mcp23017_cxt_t *ctx, const uint8_t reg, uint8_t *value)
{
	int32_t r;
	i2c_cmd_handle_t command;
	command = i2c_cmd_link_create();
	if ((r = i2c_master_start(command)) != ESP_OK)
		goto fail;
	if ((r = i2c_master_write_byte( command, (ctx->i2c_config->address << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE)) != ESP_OK) {
		ctx->err_str = (char *)"Failed to i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_write_byte( command, reg, ACK_CHECK_ENABLE)) != ESP_OK) {
		ctx->err_str = (char *)"Failed to i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_start(command)) != ESP_OK) {
		ctx->err_str = (char *)"Failed to i2c_master_start()";
		goto fail;
	}
	if ((r = i2c_master_write_byte(command, (ctx->i2c_config->address << 1) | I2C_MASTER_READ, ACK_CHECK_ENABLE)) != ESP_OK) {
		ctx->err_str = (char *)"Failed to i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_read(command, value, 1, I2C_MASTER_NACK)) != ESP_OK) {
		ctx->err_str = (char *)"Failed to i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_stop(command)) != ESP_OK) {
		ctx->err_str = (char *)"Failed to i2c_master_stop()";
		goto fail;
	}

	r = i2c_master_cmd_begin(port, command, 10 / portTICK_PERIOD_MS);
fail:
	i2c_cmd_link_delete(command);
	if (r != 0)
#if defined(HAVE_ESP_ERR_TO_NAME)
		ESP_LOGE(__func__, "%s: %s", ctx->err_str, esp_err_to_name(r));
#else
		ESP_LOGE(__func__, "%s: %d", ctx->err_str, r);
#endif
	return r;
}

int32_t
mcp23017_write8(struct mcp23017_cxt_t *ctx, const uint8_t reg, uint8_t value)
{
	int32_t r = 0;
	i2c_cmd_handle_t command;

	command = i2c_cmd_link_create();
	if ((r = i2c_master_start(command)) != ESP_OK) {
		ctx->err_str = (char *)"i2c_master_start()";
		goto fail;
	}
	if ((r = i2c_master_write_byte(command, (ctx->i2c_config->address << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE)) != ESP_OK) {
		ctx->err_str = (char *)"i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_write_byte(command, reg, ACK_CHECK_ENABLE)) != ESP_OK) {
		ctx->err_str = (char *)"i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_write(command, &value, 1, ACK_CHECK_ENABLE)) != ESP_OK) {
		ctx->err_str = (char *)"i2c_master_write_byte()";
		goto fail;
	}
	if ((r = i2c_master_stop(command)) != ESP_OK) {
		ctx->err_str = (char *)"i2c_master_stop()";
		goto fail;
	}
	if ((r = i2c_master_cmd_begin(port, command, 10 / portTICK_PERIOD_MS)) != ESP_OK) {
		ctx->err_str = (char *)"i2c_master_cmd_begin()";
		goto fail;
	}
fail:
	i2c_cmd_link_delete(command);
	if (r != 0) {
#if defined(HAVE_ESP_ERR_TO_NAME)
		ESP_LOGE(__func__, "%s: %s", ctx->err_str, esp_err_to_name(r));
#else
		ESP_LOGE(__func__, "%s: %d", ctx->err_str, r);
#endif
	}
	return r;
}
#endif // defined(TRB_MCP23017_ESP_IDF)
