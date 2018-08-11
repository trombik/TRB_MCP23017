#include <Arduino.h>
#include <stdint.h>

#if defined(TRB_MCP23017_ARDUINO_BRZO)
#include <brzo_i2c.h>
#else
#include <Wire.h>
#endif

#include <TRB_MCP23017.h>

void
delay_ms(const uint16_t milli)
{
	uint32_t now = millis();
	while (millis() < now + milli) {
#if defined(ESP32) || defined(ESP8266)
		yield();
#endif
	}
}

void
halt()
{
	Serial.println(F("Halted. Please reset"));
	while(1) {
#if defined(ESP32) || defined(ESP8266)
		yield();
#endif
	}
}

void
reset_ic()
{
	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, LOW);
	delay_ms(10);
	digitalWrite(RESET_PIN, HIGH);
}

mcp23017_i2c_config_t config;
uint8_t level = 0;
int32_t err;

void
setup()
{
	uint8_t reg_value;
#if defined(TRB_MCP23017_ARDUINO_BRZO)
	uint32_t clock_stretch_time_out_usec = 200;
#endif // defined(TRB_MCP23017_ARDUINO_BRZO)

	Serial.begin(115200);
	delay(1000);
	Serial.println(__FILE__);

	Serial.print(F("Initializing I2C... "));
#if defined(TRB_MCP23017_ARDUINO_WIRE)
#if defined(ESP32) || defined(ESP8266)
	Serial.println("Initializing I2C (Wire)");
	Wire.begin(GPIO_SDA, GPIO_SCL);
#else
	Wire.begin();
#endif // defined(ESP32) || defined(ESP8266)
#elif defined(TRB_MCP23017_ARDUINO_BRZO)
	Serial.println("Initializing I2C (Brzo I2C)");
	brzo_i2c_setup(GPIO_SDA, GPIO_SCL, clock_stretch_time_out_usec);
#endif // defined(TRB_MCP23017_ARDUINO_WIRE)
	Serial.println(F("Initialized."));

	config.scl = GPIO_SCL;
	config.sda = GPIO_SDA;
	config.address = MCP23017_I2C_ADDRESS_DEFAULT;
	config.speed = 100;

	Serial.println(F("Initializing driver."));
	if ((err = mcp23017_init()) != 0) {
		Serial.print(F("mcp23017_init(): "));
		Serial.println(err);
		halt();
	}
	Serial.println(F("Configuring driver."));
	if ((err = mcp23017_set_i2c_config(&config)) != 0) {
		Serial.print(F("mcp23017_set_i2c_config(): "));
		Serial.println(err);
		halt();
	}

	/* reset the IC so that it is in Power on Reset state */
	reset_ic();

	Serial.println(F("Read IODIRA."));
	if ((err = mcp23017_read8(MCP23x17_IODIRB, &reg_value)) != 0) {
		Serial.print(F("mcp23017_read8(): "));
		Serial.println(err);
		halt();
	}

	/* make sure IODIRA has the default value */
	if (reg_value != 0xff) {
		err = -1;
        Serial.print(F("IODIRA: "));
        Serial.println(reg_value);
		halt();
	}
	for (uint8_t pin = 0; pin <= 7; pin++) {
		Serial.print(F("Set pin "));
		Serial.print(pin);
		Serial.println(F(" to OUTPUT"));
		err = mcp23017_set_pin_direction(pin, OUTPUT);
		if (err != 0) {
			Serial.print(F("mcp23017_set_pin_direction(): "));
			Serial.println(err);
			halt();
		}
	}
	Serial.println(F("Starting loop"));
}

void
loop() {
	level ^= 1;
	for (uint8_t pin = 0; pin <= 7; pin++) {
		delay_ms(500);
		err = mcp23017_set_pin_level(pin, level);
		if (err != 0) {
			Serial.print(F("mcp23017_set_pin_level(): "));
			Serial.println(err);
			halt();
		}
	}
}
