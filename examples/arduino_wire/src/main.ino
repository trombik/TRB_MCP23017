#include <Arduino.h>
#include <stdint.h>

#if defined(TRB_MCP23017_ARDUINO_BRZO)
#include <brzo_i2c.h>
#else
#include <Wire.h>
#endif

#include <TRB_MCP23017.h>
#define PORT_A 0
#define PORT_B 1

mcp23017_i2c_config_t config;
mcp23017_dev_t *dev;
uint8_t level = 0;
uint8_t reg_value;
volatile uint8_t pin_changed = 0;
int32_t err;

void
delay_ms(const uint16_t milli)
{
	uint32_t now = millis();
	while (millis() < now + milli) {
		yield();
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

void
pin_change_isr()
{
	pin_changed = 1;
}

void
setup()
{
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
	config.freq = 100;
	dev = (mcp23017_dev_t*)calloc(1, sizeof(mcp23017_dev_t));
	if (dev == NULL) {
		Serial.println(F("Out of memory"));
		halt();
	}
	dev->i2c_config = &config;
	dev->address = MCP23017_I2C_ADDRESS_DEFAULT;

	/* reset the IC so that it is in Power on Reset state */
	reset_ic();

	Serial.println(F("Read IODIRA."));
	if ((err = mcp23017_read8(dev, MCP23x17_IODIRB, &reg_value)) != 0) {
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

	Serial.println(F("Set all pins on PORT_A as OUTPUT"));
	if ((err = mcp23017_write8(dev, MCP23x17_IODIRA, 0x00)) != 0) {
		Serial.print(F("mcp23017_write8(): "));
		Serial.println(err);
		halt();
	}

	Serial.println(F("Enable pullup for PORTB_INT_PIN on PORTB"));
	if ((err = mcp23017_set_bit(dev, MCP23x17_GPPUB, 1, PORTB_INT_PIN))
	    != 0) {
		Serial.print(F("mcp23017_set_bit(): "));
		Serial.println(err);
		halt();
	}

	Serial.println(F("Set direction on PORTB INPUT_PULLUP"));
	if ((err = mcp23017_set_pin_direction(dev, PORT_B, PORTB_INT_PIN, INPUT_PULLUP))
	    != 0) {
		Serial.print(F("mcp23017_set_pin_direction(): "));
		Serial.println(err);
		halt();
	}

	Serial.println(F("Configure PORTB_INT_PIN on PORTB to generate INT"));
	if ((err = mcp23017_enable_pin_intrrupt(dev, PORT_B, PORTB_INT_PIN, HIGH,
	    ON_CHANGE_FROM_REG)) != 0) {
		Serial.print(F("mcp23017_enable_pin_intrrupt(): "));
		Serial.println(err);
		halt();
	}

	Serial.print("Set FALLING IRQ on ");
	Serial.println(IRQ_PIN);
	pinMode(IRQ_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(IRQ_PIN), pin_change_isr, FALLING);
	Serial.println(F("Starting loop"));
}

void
loop() {
	delay_ms(500);
	if (pin_changed == 1) {
		pin_changed = 0;
		Serial.println("Pressed");
	}
	err = mcp23017_read8(dev, MCP23x17_INTCAPB, &reg_value);
	if ((err = mcp23017_read8(dev, MCP23x17_OLATA, &reg_value)) != 0) {
		Serial.print(F("mcp23017_read8(): "));
		Serial.println(err);
		halt();
	}
	if (reg_value == 0xff || reg_value == 0) {
		level ^= 1;
	}
	if (level == 1) {
		reg_value = (reg_value << 1) | 1;
	} else {
		reg_value = (reg_value << 1);
	}
	if ((err = mcp23017_write8(dev, MCP23x17_OLATA, reg_value)) != 0) {
		Serial.print(F("mcp23017_write8(): "));
		Serial.println(err);
		halt();
	}
}
