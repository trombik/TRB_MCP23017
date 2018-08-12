#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

static char component[] = "[TRB_MCP23017][mcp23017_set_bit]";
int32_t err, faked_return_value;
uint8_t faked_reg_value;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_given_pos_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* when level is invalid */
	uint8_t invalid_pos = 10;
	err = mcp23017_set_bit(1, 1, invalid_pos);

	/* then return value is EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}

TEST_CASE("WHEN_set_2nd_bit_in_MCP23x17_IODIRA_THEN_returns_zero", component)
{
	my_setup();

	/* given MCP23x17_IODIRA is set to all HIGH */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when set 2nd bit in MCP23x17_IODIRA to 0 */
	err = mcp23017_set_bit(MCP23x17_IODIRA, 0, 1);

	/* then writes logical 0 to MCP23x17_IODIRA[1], returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_IODIRA, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b11111101, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}

TEST_CASE("WHEN_set_3rd_bit_in_MCP23x17_IODIRA_THEN_returns_zero", component)
{
	my_setup();

	/* given MCP23x17_IODIRA is set to 0b10000001 */
	faked_reg_value = 0b10000001;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when set 3rd bit in MCP23x17_IODIRA set to 1 */
	err = mcp23017_set_bit(MCP23x17_IODIRA, 1, 2);

	/* then writes logical 1 to MCP23x17_IODIRA[2], returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_IODIRA, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b10000101, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}

TEST_CASE("when_mcp23017_read8_failed_THEN_returns_non_zero", component)
{
	my_setup();

	/* when mcp23017_read8() fails */
	faked_reg_value = 0xff;
	faked_return_value = -1;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
	err = mcp23017_set_bit(MCP23x17_IODIRA, 1, 2);

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
	err = mcp23017_set_bit(MCP23x17_IODIRA, 0, 1);

	/* then returns non-zero */
	TEST_ASSERT_EQUAL_INT32(-1, err);

	my_teardown();
}

TEST_CASE("WHEN_value_has_been_set_THEN_does_not_call_mcp23017_write8", component)
{
	my_setup();

	/* given the target bit has been set */
	faked_reg_value = 0xff;
	faked_return_value = 0;
	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when value has been set */
	err = mcp23017_set_bit(MCP23x17_IODIRA, 1, 1);

	/* then mcp23017_write8() is not called */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_INT(0, mcp23017_write8_fake.call_count);

	my_teardown();
}
