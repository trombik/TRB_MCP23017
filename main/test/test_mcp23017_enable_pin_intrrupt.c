#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

#define PORTA 0
#define PORTB 1

static char component[] = "[TRB_MCP23017][mcp23017_enable_pin_intrrupt]";
int32_t err, faked_return_value;
uint8_t faked_reg_value;
mcp23017_dev_t *dev;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("WHEN_given_defaul_value_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* when default_value is invalid */
	uint8_t invalid = 10;
	err = mcp23017_enable_pin_intrrupt(dev, PORTA, 1, invalid, ON_CHANGE_FROM_REG);
	/* then return value is EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);
	err = mcp23017_enable_pin_intrrupt(dev, invalid, 1, 0, ON_CHANGE_FROM_REG);
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}

TEST_CASE("WHEN_set_pin_8_is_set_THEN_returns_zero", component)
{
    uint8_t expected;
	my_setup();

	/* given necessry bits of all registers are 0 */
	faked_reg_value = 0b11110000;
	expected = faked_reg_value + 1;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 8, or the first pin of PORTB, is set to cause intrrupt on
	 * change with default HIGH */
	err = mcp23017_enable_pin_intrrupt(dev, PORTB, 0, HIGH, ON_CHANGE_FROM_REG);

	/* then writes 1 to three registers, returns zero */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_DEFVALB, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(expected, mcp23017_write8_fake.arg2_history[0]);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_INTCONB, mcp23017_write8_fake.arg1_history[1]);
	TEST_ASSERT_EQUAL_UINT8(expected, mcp23017_write8_fake.arg2_history[1]);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_GPINTENB, mcp23017_write8_fake.arg1_history[2]);
	TEST_ASSERT_EQUAL_UINT8(expected, mcp23017_write8_fake.arg2_history[2]);

	my_teardown();
}

TEST_CASE("when_mcp23017_write8_failed_THEN_returns_non_zero", component)
{
	my_setup();

	/* given the target bit has not been set */
	faked_reg_value = 0b11110000;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when mcp23017_write8() fails */
	mcp23017_write8_fake.return_val = -1;
	err = mcp23017_enable_pin_intrrupt(dev, PORTB, 0, HIGH, ON_CHANGE_FROM_REG);

	/* then returns non-zero */
	TEST_ASSERT_EQUAL_INT32(-1, err);

	my_teardown();
}

TEST_CASE("when_pin_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* given the target bit has not been set */
	faked_reg_value = 0b11110000;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	uint8_t invalid = 8;

	/* when pin number is invalid */
	err = mcp23017_enable_pin_intrrupt(dev, PORTB, invalid, HIGH, ON_CHANGE_FROM_REG);

	/* then returns EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}

TEST_CASE("when_pin_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* given the target bit has not been set */
	faked_reg_value = 0b11110000;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	uint8_t invalid = 2;

	/* when mcp23017_write8() fails */
	err = mcp23017_enable_pin_intrrupt(dev, invalid, 1, HIGH, ON_CHANGE_FROM_REG);

	/* then returns EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}
