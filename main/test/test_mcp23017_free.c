#include <unity.h>

#include "TRB_MCP23017.h"

static char component[] = "[TRB_MCP23017]";

TEST_CASE("when_driver_is_not_initilized_THEN_no_panic", component)
{
	mcp23017_free()
}
