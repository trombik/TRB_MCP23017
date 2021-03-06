Import("env")
src_filter = ["+<TRB_MCP23017.h>"]
env.Replace(SRC_FILTER=src_filter)

build_flags = env.ParseFlags(env['BUILD_FLAGS'])
cppdefines = build_flags.get("CPPDEFINES")

if "TRB_MCP23017_ESP_IDF" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_MCP23017.c>", "+<sys/esp_idf>"])
if "TRB_MCP23017_ARDUINO_WIRE" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_MCP23017.cpp>", "+<sys/arduino_wire>"])
if "TRB_MCP23017_ARDUINO_BRZO" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_MCP23017.cpp>", "+<sys/arduino_brzo>"])
