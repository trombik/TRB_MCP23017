#include "shared_fakes.h"
#include <unity.h>
#include <TRB_MCP23017.h>

DEFINE_FAKE_VALUE_FUNC(int32_t, mcp23017_read8,  const mcp23017_dev_t *, const uint8_t, uint8_t *);
DEFINE_FAKE_VALUE_FUNC(int32_t, mcp23017_write8, const mcp23017_dev_t *, const uint8_t, const uint8_t);

/* pass faked register value by custom function */
int32_t
mcp23017_read8_fake_custom_fake(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t *reg_value)
{
	*reg_value = faked_reg_value;
	return faked_return_value;
}

uint8_t
my_setup()
{
	RESET_FAKE(mcp23017_read8);
	RESET_FAKE(mcp23017_write8);
	FFF_RESET_HISTORY();
	return 0;
}

uint8_t
my_teardown()
{
	RESET_FAKE(mcp23017_read8);
	RESET_FAKE(mcp23017_write8);
	FFF_RESET_HISTORY();
	return 0;
}
