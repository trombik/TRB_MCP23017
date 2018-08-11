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

#if !defined(_TRB_MCP23017_h)
#define _TRB_MCP23017_h

#include <stdint.h>
#include "TRB_MCP23X17.h"

#if defined(TRB_MCP23017_ESP_IDF)
#include <driver/i2c.h>
#endif

#if !defined(ARDUINO)
#define INPUT		0x0
#define OUTPUT		0x1
#define INPUT_PULLUP	0x2
#endif

#define HIGH	0x1
#define LOW	0x0

#define MCP23017_I2C_ADDRESS_DEFAULT    (0x20)

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * \brief I2C configuration
 */
typedef struct
{
	/** I2C SCL pin number */
	uint8_t scl;
	/** I2C SDA pin number */
	uint8_t sda;
	/** I2C address of the device */
	uint16_t address;
	/** I2C clock frequency in Khz (brzo I2C only) */
	uint16_t freq;
	/** I2C port number (ESP32 only) */
	uint8_t i2c_port;
} mcp23017_i2c_config_t;

/*
 * \brief Initialize the driver
 *
 * \return : zero on success. ENOMEM on out of memory.
 */
int8_t
mcp23017_init();

/*
 * \brief Free the driver.
 */
void
mcp23017_free();

int8_t
mcp23017_set_i2c_config(const mcp23017_i2c_config_t *new_config);

/*
 * \brief Set directon of a PIN
 *
 * \param pin_num : Pin number, starting from zero (GPA0, or physical pin 21) to
 * 15 (GPB7, or physical pin 8).
 * \param directon : Direction of the pin, INPUT, INPUT_PULLUP, or OUTPUT.
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_set_pin_direction(const uint8_t pin_num, const uint8_t direction);

/*
 * \brief Set level of a pin
 *
 * \param pin_num : Pin number, starting from zero to 15.
 * \param level : HIGH or LOW.
 * \return zero on success, non-zero on failure.
 */
int32_t
mcp23017_set_pin_level(const uint8_t pin_num, const uint8_t level);

/*
 * \brief Disable pullup on a pin
 *
 * \param pin_num : Pin number
 */
int32_t
mcp23017_disable_pullup(const uint8_t pin_num);

/*
 * \brief Enable pullup on a pin
 *
 * \param pin_num : Pin number
 */
int32_t
mcp23017_enable_pullup(const uint8_t pin_num);

/*
 * \brief Read a byte from a register
 *
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_read8(const uint8_t reg, uint8_t *value);

/*
 * \brief Write a byte to a register
 *
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_write8(const uint8_t reg, uint8_t value);

/*
 * \brief Returns configured I2C address
 *
 * \return I2C address
 */
uint16_t
mcp23017_get_i2c_address();

#if defined(TRB_MCP23017_ESP_IDF)
/*
 * \brief Return I2C port number (ESP32 only)
 *
 * \return i2c_port_t number
 */
i2c_port_t
mcp23017_get_i2c_port();
#endif

uint16_t
mcp23017_get_i2c_freq();

#if defined(__cplusplus)
}
#endif

#endif // !defined(_TRB_MCP23017_h)
