#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

#define PORTA 0
#define PORTB 1

static char component[] = "[TRB_MCP23017][mcp23017_set_pin_level]";
int32_t err, faked_return_value;
uint8_t faked_reg_value;
mcp23017_dev_t *dev;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_given_level_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* when level is invalid */
	err = mcp23017_set_pin_level(dev, PORTA, 1, 10);

	/* then return value is EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}

TEST_CASE("when_pin_1_is_set_to_low_THEN_returns_0", component)
{
	my_setup();

	/* given OLATA is set to all HIGH */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1, or IO1, is set to LOW */
	err = mcp23017_set_pin_level(dev, PORTA, 1, LOW);

	/* then writes logical 0 to OLATA[1], returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_OLATA, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111101, mcp23017_write8_fake.arg2_history[0]);

	my_teardown();
}

TEST_CASE("when_pin_1_is_set_to_high_THEN_returns_0", component)
{
	my_setup();

	/* given OLATA is set to all LOW */
	faked_reg_value = 0x00;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1, or IO1,  set to HIGH */
	err = mcp23017_set_pin_level(dev, PORTA, 1, HIGH);

	/* then writes logical 1 to OLATA[1], returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_OLATA, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b00000010, mcp23017_write8_fake.arg2_history[0]);

	my_teardown();
}

TEST_CASE("when_pin_8_is_set_to_low_THEN_returns_0", component)
{
	my_setup();

	/* given OLATB is set to all HIGH */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1 on PORTB, or IO0, is set to LOW */
	err = mcp23017_set_pin_level(dev, PORTB, 0, LOW);

	/* then writes logical 0 to OLATB[0], returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_OLATB, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111110, mcp23017_write8_fake.arg2_history[0]);

	my_teardown();
}

TEST_CASE("when_pin_8_is_set_to_high_THEN_returns_0", component)
{
	my_setup();

	/* given OLATB is set to all LOW */
	faked_reg_value = 0x00;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1 on PORTB, or IO0, is set to HIGH */
	err = mcp23017_set_pin_level(dev, PORTB, 0, HIGH);

	/* then writes logical 1 to OLATB[0], returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_OLATB, mcp23017_write8_fake.arg1_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b00000001, mcp23017_write8_fake.arg2_history[0]);

	my_teardown();
}

TEST_CASE("when_mcp23017_read8_failed_THEN_returns_non_zero", component)
{
	my_setup();

	/* when mcp23017_read8() fails */
	faked_reg_value = 0xff;
	faked_return_value = -1;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	err = mcp23017_set_pin_level(dev, PORTA, 1, HIGH);

	/* then returns non-zero */
	TEST_ASSERT_EQUAL_INT32(-1, err);

	my_teardown();
}

TEST_CASE("when_mcp23017_write8_failed_THEN_returns_non_zero", component)
{
	my_setup();

	/* given the target bit has not been set */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when mcp23017_write8() fails */
	mcp23017_write8_fake.return_val = -1;
	err = mcp23017_set_pin_level(dev, PORTA, 1, LOW);

	/* then returns non-zero */
	TEST_ASSERT_EQUAL_INT32(-1, err);

	my_teardown();
}
