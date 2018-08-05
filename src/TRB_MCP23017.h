#if !defined(_TRB_MCP23017_h)

#include "TRB_MCP23X17.h"

#if defined(TRB_MCP23017_ESP_IDF)
#include "sys/esp_idf/i2c.h"
#endif

#if !defined(ARDUINO)
#define INPUT		0x0
#define OUTPUT		0x1
#define INPUT_PULLUP	0x2

#define HIGH	0x1
#define LOW	0x0
#endif

#define MCP23017_I2C_ADDRESS_DEFAULT    (0x20)

/*
 * \brief I2C configuration
 */
struct mcp23017_i2c_t
{
	/** I2C SCL pin number */
	uint8_t scl;
	/** I2C SDA pin number */
	uint8_t sda;
	/** I2C address of the device */
	uint16_t address;
	/** I2C clock speed in Khz (brzo I2C only) */
	uint16_t speed;
	/** I2C port number (ESP32 only) */
	uint8_t i2c_port;
};

struct mcp23017_cxt_t; // dummy

/*
 * \brief MCP23017 context
 */
struct mcp23017_cxt_t
{
	/** I2C config */
	struct mcp23017_i2c_t *i2c_config;
	/** Error message of the last error */
	char *err_str;
};

/*
 * \brief Create device context
 *
 * \return pointer to mcp23017_cxt_t, or NULL on failure.
 */
struct mcp23017_cxt_t *
mcp23017_new(void);

/*
 * \brief Free device context
 */
void
mcp23017_free(struct mcp23017_cxt_t *ctx);

/*
 * \brief Set directon of a PIN
 *
 * \param ctx : Pointer to mcp23017_cxt_t
 * \param pin_num : Pin number, starting from zero (GPA0, or physical pin 21) to
 * 15 (GPB7, or physical pin 8).
 * \param directon : Direction of the pin, INPUT, INPUT_PULLUP, or OUTPUT.
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_set_pin_direction(struct mcp23017_cxt_t *ctx, const uint8_t pin_num, const uint8_t direction);

/*
 * \brief Set level of a pin
 *
 * \param ctx : Pointer to mcp23017_cxt_t
 * \param pin_num : Pin number, starting from zero to 15.
 * \param level : HIGH or LOW.
 * \return zero on success, non-zero on failure.
 */
int32_t
mcp23017_set_pin_level(struct mcp23017_cxt_t *ctx, const uint8_t pin_num, const uint8_t level);

/*
 * \brief Disable pullup on a pin
 *
 * \param ctx : Pointer to mcp23017_cxt_t
 * \param pin_num : Pin number
 */
int32_t
mcp23017_disable_pullup(struct mcp23017_cxt_t *ctx, const uint8_t pin_num);

/*
 * \brief Enable pullup on a pin
 *
 * \param ctx : Pointer to mcp23017_cxt_t
 * \param pin_num : Pin number
 */
int32_t
mcp23017_enable_pullup(struct mcp23017_cxt_t *ctx, const uint8_t pin_num);

/*
 * \brief Read a byte from a register
 *
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_read8(struct mcp23017_cxt_t *ctx, const uint8_t reg, uint8_t *value);

/*
 * \brief Write a byte to a register
 *
 * \return zero on success, non-zero on failure
 */
int32_t
mcp23017_write8(struct mcp23017_cxt_t *ctx, const uint8_t reg, uint8_t value);

#endif // !defined(_TRB_MCP23017_h)
