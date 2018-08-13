#if !defined(shared_fakes_h)
#define shared_fakes_h
#include <stdint.h>
#include <TRB_MCP23017.h>
#include "../fff.h"

/* mock peripheral interfaces */
DECLARE_FAKE_VALUE_FUNC(int32_t, mcp23017_read8,  const mcp23017_dev_t *, const uint8_t, uint8_t *);
DECLARE_FAKE_VALUE_FUNC(int32_t, mcp23017_write8, const mcp23017_dev_t *, const uint8_t, const uint8_t);

/* set these to fake value from mcp23017_read8() */
extern uint8_t faked_reg_value; /** faked value of register */
extern int32_t faked_return_value; /** faked return value of mcp23017_read8() */

/*
 * \brief function to set faked register value in mcp23017_read8()
 *
 * mcp23017_read8_fake.custom_fake = mcp23017_read8_fake_custom_fake;
 */
int32_t
mcp23017_read8_fake_custom_fake(const mcp23017_dev_t *dev, const uint8_t reg, uint8_t *reg_value);

/*
 * \brief our own setup function, must be placed at the begining of tests because the
 * official setUp() has been defined in $IDF_PATH/tools/unit-test-app and the
 * unit-test-app component does not provide us with a way to add additional
 * setup/teardown function.
 *
 * calls mcp23017_init(), resets defined faked functions, free fff history.
 *
 * call my_setup() at the begining, my_teardown() at the end of tests.
 */
uint8_t
my_setup();

/* \brief complementary function of my_setup().
 */
uint8_t
my_teardown();

#endif
