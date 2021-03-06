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
	/** I2C clock frequency in Khz (implementation varies depending on
	* libraries. see mcp23017_read8) */
	uint16_t freq;
	/** I2C port number (ESP32 only) */
	uint8_t i2c_port;
} mcp23017_i2c_config_t;

typedef struct mcp23017_dev_t
{
	/** I2C bus configuration */
	mcp23017_i2c_config_t *i2c_config;
	/** I2C address of the device */
	uint8_t address;
} mcp23017_dev_t;

/*
 * \brief Set directon of a PIN
 *
 * \param dev : device variable
 * \param port : 0 (PORTA), or 1 (PORTB)
 * \param pin_num : Pin number, starting from zero (GPA0, or physical pin 21) to
 * 15 (GPB7, or physical pin 8).
 * \param directon : Direction of the pin, INPUT, INPUT_PULLUP, or OUTPUT.
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_set_pin_direction(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t direction);

/*
 * \brief Set level of a pin
 *
 * \param dev : device variable
 * \param port : 0 (PORTA), or 1 (PORTB)
 * \param pin_num : Pin number, starting from zero to 15.
 * \param level : HIGH or LOW.
 * \return zero on success, non-zero on failure.
 */
int32_t
mcp23017_set_pin_level(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t level);

/*
 * \brief Configure intrrupt of a pin
 *
 * * Set default value for the intrrupt pin
 * * Set intrrupt condition
 * * Enable intrrupt on the pin
 *
 * \param dev : device variable
 * \param pin_num : Pin number between 0 and 15
 * \param default_value : default value to compare when interrupt-on-change
 * from register value is used
 * \param condition : ON_CHANGE or ON_CHANGE_FROM_REG
 * \return Zero on success, EINVAL when argument is invalid, other values on
 * I2C failures
 */
int32_t
mcp23017_enable_pin_intrrupt(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num, const uint8_t default_value, const mcp_int_condition_t condition);

/*
 * \brief Set a bit on a register
 *
 * \param dev : device variable
 * \param reg : Register address
 * \param value : 1 or 0.
 * \param pos : Bit position to set the value
 * \return Zero on success. EINVAL if argument is invalid. Other values on I2C
 * failure.
 */
int32_t
mcp23017_set_bit(const mcp23017_dev_t *dev, const uint8_t reg, const uint8_t value, const uint8_t pos);

/*
 * \brief Get a bit of a register
 *
 * \param dev : device variable
 * \param reg : Register address
 * \param value : Variable to write the bit to
 * \param pos : Bit position to set the value
 * \return Zero on success. EINVAL if argument is invalid. Other values on I2C
 * failure.
 */
int32_t
mcp23017_get_bit(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t *value, const uint8_t pos);

/*
 * \brief Disable pullup on a pin
 *
 * \param dev : device variable
 * \param port : 0 (PORTA), or 1 (PORTB)
 * \param pin_num : Pin number
 */
int32_t
mcp23017_disable_pin_pullup(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num);

/*
 * \brief Enable pullup on a pin
 *
 * \param dev : device variable
 * \param port : 0 (PORTA), or 1 (PORTB)
 * \param pin_num : Pin number
 */
int32_t
mcp23017_enable_pin_pullup(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num);

/*
 * \brief Flip output level of a pin
 *
 * \param dev : device variable
 * \param port : 0 (PORTA), or 1 (PORTB)
 * \param pin_num : Pin number
 */
int32_t
mcp23017_flip_pin(const mcp23017_dev_t *dev, const uint8_t port, const uint8_t pin_num);

/*
 * \brief Read a byte from a register
 *
 * This function reads 8bit value from a register.
 *
 * `freq` in `mcp23017_i2c_config_t` is NOT used by the library except Brzo
 * I2C. It is the application code's responsibility to manage SCL frequency.
 *
 * \param dev : device variable
 * \param reg : Register address
 * \param value : Variable to save the value from the register
 * \return zero on success, non-zero on failure
 *
 * When experimental WITH_DYNAMIC_I2C_FREQ is defined as build flag (disabled
 * by default), and the underlying I2C library provides a function to set SCL
 * frequency, it sets the pre-configured clock frequency by
 * mcp23017_set_i2c_config() before each I2C transaction. Following libraries
 * supports this feature:
 *
 * * Arduino esp32 with Wire
 * * Arduino esp8266 with Wire
 * * Arduino Brzo I2C for ESP8266
 *
 * When underlying I2C library also provides a function to get currently
 * configured frequency, it restores the configured frequency after each I2C
 * transaction.  Following libraries supports this feature:
 *
 * * Arduino ESP32 with Wire
 * * Arduino Brzo I2C for ESP8266
 *
 * With other libraries, it is necessary to restore the old frequency after
 * every function call that reads or writes registers, or simply use same SCL
 * clock throughout the application code. Note that Brzo I2C behaves this way
 * with or without WITH_DYNAMIC_I2C_FREQ defined.
 */
int32_t
mcp23017_read8(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t *value);

/*
 * \brief Write a byte to a register
 *
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_write8(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t value);

#if defined(__cplusplus)
}
#endif

#endif // !defined(_TRB_MCP23017_h)
