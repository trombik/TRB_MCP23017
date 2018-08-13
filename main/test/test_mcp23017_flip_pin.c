#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

#define PORTA 0
#define PORTB 1

static char component[] = "[TRB_MCP23017][mcp23017_disable_pin_pullup]";
int32_t err, faked_return_value;
uint8_t faked_reg_value;
mcp23017_dev_t *dev;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_pin_is_flipped_THEN_flip_the_bit_in_", component)
{
	my_setup();

	/* given the pin is set to HIGH */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1 is flipped */
	err = mcp23017_flip_pin(dev, PORTA, 1);

	/* then it writes 0 to OLATA[1] */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_OLATA, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111101, mcp23017_write8_fake.arg2_history[0]);

	my_teardown();
}

TEST_CASE("when_pin_is_flipped_THEN_flip_the_bit_in_", component)
{
	my_setup();

	/* given the pin is set to HIGH */
	faked_reg_value = 0b11111101;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1 is flipped */
	err = mcp23017_flip_pin(dev, PORTA, 1);

	/* then it writes 1 to OLATA[1] */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_OLATA, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0xff, mcp23017_write8_fake.arg2_history[0]);

	my_teardown();
}

TEST_CASE("WHEN_pin_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* given the pin is set to HIGH */
	faked_reg_value = 0b11111101;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
    uint8_t invalid = 8;

	/* when pin 1 is flipped */
	err = mcp23017_flip_pin(dev, PORTA, invalid);

	/* then returns EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}
TEST_CASE("WHEN_port_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* given the pin is set to HIGH */
	faked_reg_value = 0b11111101;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
    uint8_t invalid = 2;

	/* when pin 1 is flipped */
	err = mcp23017_flip_pin(dev, invalid, 1);

	/* then returns EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}
