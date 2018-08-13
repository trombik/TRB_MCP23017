#include <stdint.h>
#include <errno.h>
#include <unity.h>
#include "../fff.h"
#include "shared_fakes.h"

#include <TRB_MCP23017.h>

static char component[] = "[TRB_MCP23017][mcp23017_get_bit]";
int32_t err, faked_return_value;
uint8_t faked_reg_value, reg_value;
mcp23017_dev_t *dev;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_given_pos_is_invalid_THEN_returns_EINVAL", component)
{
	my_setup();

	/* when level is invalid */
	uint8_t invalid_pos = 10;
	err = mcp23017_get_bit(dev, 1, &reg_value, invalid_pos);

	/* then return value is EINVAL */
	TEST_ASSERT_EQUAL_INT32(EINVAL, err);

	my_teardown();
}

TEST_CASE("WHEN_set_1st_bit_in_MCP23x17_IODIRA_THEN_returns_zero", component)
{
	my_setup();

	/* given 1st bit in MCP23x17_IODIRA is 1 */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when get 1st bit in MCP23x17_IODIRA  */
	err = mcp23017_get_bit(dev, MCP23x17_IODIRA, &reg_value, 0);

	/* then writes 1 to pointer, returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(1, reg_value);

	my_teardown();
}

TEST_CASE("WHEN_set_1st_bit_in_MCP23x17_IODIRA_THEN_returns_zero_with_zero", component)
{
	my_setup();

	/* given 1st bit in MCP23x17_IODIRA is 0 */
	faked_reg_value = 0b00001110;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when get 1st bit in MCP23x17_IODIRA  */
	err = mcp23017_get_bit(dev, MCP23x17_IODIRA, &reg_value, 0);

	/* then writes 1 to pointer, returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(0, reg_value);

	my_teardown();
}

TEST_CASE("WHEN_set_2nd_bit_in_MCP23x17_IODIRA_THEN_returns_zero", component)
{
	my_setup();

	/* given MCP23x17_IODIRA is set to all HIGH */
	faked_reg_value = 0xff;
	faked_return_value = 0;
 	mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;

	/* when get 2nd bit in MCP23x17_IODIRA  */
	err = mcp23017_get_bit(dev, MCP23x17_IODIRA, &reg_value, 1);

	/* then writes 1 to pointer, returns 0 */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(1, reg_value);

	my_teardown();
}
