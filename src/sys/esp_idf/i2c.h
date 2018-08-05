#if !defined(_sys_esp_idf_i2c_h)

#include <stdint.h>
#include <driver/i2c.h>

i2c_port_t
mcp23017_get_i2c_port();

i2c_port_t
mcp23017_set_i2c_port(i2c_port_t port);
#endif
