#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

static char component[] = "[TRB_MCP23017][mcp23017_enable_pullup]";
int32_t err, faked_return_valu;
uint8_t faked_reg_value;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_pullup_a_pin_THEN_write_1_to_GPPU", component)
{
	my_setup();

	/* given all pullups are disabled (0 == disabled) */
	faked_reg_value = 0x00;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 1, or IO1, is enabled */
	err = mcp23017_enable_pullup(1);


	/* then it writes 1 to GPPUA[1] */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_GPPUA, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b00000010, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}

TEST_CASE("when_pin_is_on_B_then_register_is_MCP23x17_GPPUB", component)
{
	my_setup();

	/* given all pullups are disabled (0 == disabled) */
	faked_reg_value = 0x00;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pin 8, or IO0 on PORTB, is enabled */
	err = mcp23017_enable_pullup(8);


	/* then it writes 1 to GPPUB[0] */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_GPPUB, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b00000001, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}

TEST_CASE("when_requested_value_has_been_set_THEN_does_not_call_mcp23017_write8", component)
{
	my_setup();

	/* given the target bit has already been set */
	faked_reg_value = 0b00000001;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when pullup for pin 0 is requested */
	err = mcp23017_enable_pullup(0);

	/* then mcp23017_write8() is not called */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_INT(0, mcp23017_write8_fake.call_count);

	my_teardown();
}

TEST_CASE("when_mcp23017_read8_failed_THEN_returns_non_zero", component)
{
	my_setup();

	/* when mcp23017_read8() fails */
	faked_reg_value = 0xff;
	faked_return_value = -1;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	err = mcp23017_enable_pullup(1);

	/* then returns non-zero */
	TEST_ASSERT_EQUAL_INT32(-1, err);

	my_teardown();
}

TEST_CASE("when_mcp23017_write8_failed_THEN_returns_non_zero", component)
{
	my_setup();

	/* when mcp23017_write8() fails */
	faked_reg_value = 0x00;
	faked_return_value = 0;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	mcp23017_write8_fake.return_val = -1;
	err = mcp23017_enable_pullup(1);

	/* then returns non-zero */
	TEST_ASSERT_EQUAL_INT32(-1, err);

	my_teardown();
}
