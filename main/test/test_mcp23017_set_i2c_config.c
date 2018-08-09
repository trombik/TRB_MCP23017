#include <errno.h>
#include <unity.h>

#include <TRB_MCP23017.h>

static char component[] = "[TRB_MCP23017]";

TEST_CASE("when_driver_is_not_initilized_THEN_return_EINVAL", component)
{
	TEST_ASSERT_EQUAL_INT8(EINVAL, mcp23017_set_i2c_config((mcp23017_i2c_config_t *)NULL));
}

TEST_CASE("when_argument_is_null_THEN_return_EINVAL", component)
{
	TEST_ASSERT_EQUAL_INT8(0, mcp23017_init());
	TEST_ASSERT_EQUAL_INT8(EINVAL, mcp23017_set_i2c_config((mcp23017_i2c_config_t *)NULL));
	mcp23017_free();
}
