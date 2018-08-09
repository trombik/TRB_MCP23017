#include <stdint.h>
#include <errno.h>
#include "unity.h"
#include "../fff.h"

#include "TRB_MCP23017.h"

static char component[] = "[TRB_MCP23017][mcp23017_set_pin_direction]";
uint32_t r;

/* setup fff */
DEFINE_FFF_GLOBALS;
/* mock peripheral  interfaces */
FAKE_VALUE_FUNC(int32_t, mcp23017_read8,  const uint8_t, uint8_t *);
FAKE_VALUE_FUNC(int32_t, mcp23017_write8, const uint8_t, const uint8_t);

/* faked register value */
static uint8_t faked_reg_value;

/* pass faked register value by custome function */
uint32_t mcp23017_read8_fake_custom_fake(const uint8_t reg, uint8_t *reg_value)
{
	*reg_value = faked_reg_value;
	return 0;
}

TEST_CASE("when_direction_is_invalid_THEN_returns_EINVAL", component)
{
	/* when direction is invalid */
	mcp23017_init();
	r = mcp23017_set_pin_direction(1, 0xff);
	mcp23017_free();

	/* returns EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, r);
}

TEST_CASE("when_direction_is_valid_THEN_returns_zero", component)
{
	/* when argument is one of HIGH, LOW, and INPUT_PULLUP, returns zero */
	mcp23017_init();
	r = mcp23017_set_pin_direction(1, HIGH);
	TEST_ASSERT_EQUAL_INT32(0, r);
	r = mcp23017_set_pin_direction(1, LOW);
	TEST_ASSERT_EQUAL_INT32(0, r);
	r = mcp23017_set_pin_direction(1, INPUT_PULLUP);
	TEST_ASSERT_EQUAL_INT32(0, r);
	mcp23017_free();
}

TEST_CASE("when_pin_1_is_set_as_output_THEN_sets_zero_bit_in_MCP23x17_IODIRA", component)
{
	RESET_FAKE(mcp23017_read8);
	RESET_FAKE(mcp23017_write8);
	FFF_RESET_HISTORY();
	/* given MCP23x17_IODIRA has all bits set */
	faked_reg_value = 0xff;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	mcp23017_read8_fake.return_val = 0;

	/* when called to set pin 1 as output */
	TEST_ASSERT_EQUAL_UINT8(0, mcp23017_init());
	r = mcp23017_set_pin_direction(1, OUTPUT);
	mcp23017_free();

	/* then, clear IO1 bit in MCP23x17_IODIRA */
	TEST_ASSERT_EQUAL_UINT32(0, r);
	TEST_ASSERT_EQUAL_HEX(MCP23x17_IODIRA, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111101, mcp23017_write8_fake.arg1_history[0]);
}

TEST_CASE("when_pin_8_is_set_as_output_THEN_sets_zero_bit_in_MCP23x17_IODIRB", component)
{
	RESET_FAKE(mcp23017_read8);
	RESET_FAKE(mcp23017_write8);
	FFF_RESET_HISTORY();
	/* given MCP23x17_IODIRB has all bits set */
	faked_reg_value = 0xff;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	mcp23017_read8_fake.return_val = 0;

	/* when called to set pin 8 as output */
	TEST_ASSERT_EQUAL_UINT8(0, mcp23017_init());
	r = mcp23017_set_pin_direction(10, OUTPUT);
	mcp23017_free();

	/* then, clear IO0 bit in MCP23x17_IODIRB */
	TEST_ASSERT_EQUAL_UINT32(0, r);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_IODIRB, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111011, mcp23017_write8_fake.arg1_history[0]);
}
