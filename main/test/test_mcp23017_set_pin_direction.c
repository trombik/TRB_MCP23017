#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

static char component[] = "[TRB_MCP23017][mcp23017_set_pin_direction]";
int32_t r, faked_return_value;
uint8_t faked_reg_value;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_direction_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* when direction is invalid */
	mcp23017_init();
	r = mcp23017_set_pin_direction(1, 0xff);

	/* returns EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, r);

	my_teardown();
}

TEST_CASE("when_direction_is_valid_THEN_returns_zero", component)
{
	my_setup();

	/* when argument is one of HIGH, LOW, and INPUT_PULLUP, returns zero */
	r = mcp23017_set_pin_direction(1, HIGH);
	TEST_ASSERT_EQUAL_INT32(0, r);
	r = mcp23017_set_pin_direction(1, LOW);
	TEST_ASSERT_EQUAL_INT32(0, r);
	r = mcp23017_set_pin_direction(1, INPUT_PULLUP);
	TEST_ASSERT_EQUAL_INT32(0, r);

	my_teardown();
}

TEST_CASE("when_pin_1_is_set_as_output_THEN_sets_zero_bit_in_MCP23x17_IODIRA", component)
{
	my_setup();

	/* given MCP23x17_IODIRA has all bits set */
	faked_reg_value = 0xff;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when called to set pin 1 as output */
	r = mcp23017_set_pin_direction(1, OUTPUT);

	/* then, clear IO1 bit in MCP23x17_IODIRA */
	TEST_ASSERT_EQUAL_UINT32(0, r);
	TEST_ASSERT_EQUAL_HEX(MCP23x17_IODIRA, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111101, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}

TEST_CASE("when_pin_8_is_set_as_output_THEN_sets_zero_bit_in_MCP23x17_IODIRB", component)
{
	my_setup();

	/* given MCP23x17_IODIRB has all bits set */
	faked_reg_value = 0xff;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when called to set pin 8 as output */
	TEST_ASSERT_EQUAL_UINT8(0, mcp23017_init());
	r = mcp23017_set_pin_direction(10, OUTPUT);

	/* then, clear IO0 bit in MCP23x17_IODIRB */
	TEST_ASSERT_EQUAL_UINT32(0, r);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_IODIRB, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111011, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}
