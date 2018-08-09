#include <stdint.h>
#include <errno.h>
#include "unity.h"
#include "../fff.h"
#include "shared_fakes.h"

#include "TRB_MCP23017.h"

static char component[] = "[TRB_MCP23017]";
uint32_t err;
uint8_t faked_reg_value;
uint32_t faked_return_value;

/* setup fff */
DEFINE_FFF_GLOBALS;

TEST_CASE("when_pullup_a_pin_THEN_write_1_to_GPPU", component)
{
	my_setup();

	/* given all pullups are disabled (0 == disabled) */
	faked_reg_value = 0x00;
	faked_return_value = 0;

	/* when pin 1, or IO1, is enabled */
	err = mcp23017_enable_pullup(1);


	/* then it writes 1 to GPPUA[1] */
	TEST_ASSERT_EQUAL_INT32(0, err);
	TEST_ASSERT_EQUAL_UINT8(MCP23x17_GPPUA, mcp23017_write8_fake.arg0_history[0]);
	TEST_ASSERT_EQUAL_UINT8(0b00000010, mcp23017_write8_fake.arg1_history[0]);

	my_teardown();
}
