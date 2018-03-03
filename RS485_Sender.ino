#include "Arduino.h"

// Library to allow a Serial port on arbitrary pins
#include <SoftwareSerial.h>

// These are the pins we'll be talking to the RS485 device on
#define RS485rx		6 // RS485 Receive pin
#define RS485Tx		7 // RS485 Transmit pin
#define RS485inout	5 // RS485 Transmit or Receive status

#define RS485Transmit	HIGH
#define RS485Receive	LOW
#define ledPin			13
#define baudRate		115200

// Define the RS485 object
SoftwareSerial RS485(RS485rx, RS485Tx);

// The data bytes we're sending or receiving
byte rxValue;
byte txValue;

// -----------------------------------------------------------------
// SETUP         SETUP         SETUP         SETUP         SETUP
// -----------------------------------------------------------------
void setup() /****** SETUP: RUNS ONCE ******/
{
	// Debugging window
	Serial.begin(9600);

	// Set modes for pins
	pinMode(ledPin, OUTPUT);
	pinMode(RS485inout, OUTPUT);

	// Ensure the on-board LED is off
	digitalWrite(ledPin, HIGH);

	// Set RS485 device to read initially
	digitalWrite(RS485inout, RS485Receive);

	// Set the baud rate. The longer the wire the slower you should
	// set the transmission rate. Anything here:
	// 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250,
	// 38400, 57600, and 115200
	// MUST BE THE SAME AS THE SENDER UNIT!
	RS485.begin(baudRate);
}

// -----------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------
void loop()
{
	static String teststring = "RS485"; // Serial Communication is easy!";

	// Continually send the test string with a pause at the end
	for (unsigned int cnt = 0; cnt < teststring.length(); cnt++) {

		// Get the next character to send
		txValue = teststring[cnt];
		Serial.print("Sending:");Serial.println(char(txValue));

		// Set the RS485 to transmit mode and send the value
		digitalWrite(RS485inout, RS485Transmit);
		RS485.write(txValue);

		// small delay to allow transmission to complete
		delay(1);

		// Switch RS485 to receive mode
		digitalWrite(RS485inout, RS485Receive);

		// After each character is sent look for a received answer
		if (RS485.available())
		{
			// LED flicker
			digitalWrite(ledPin, HIGH);

			// Read the incoming byte
			rxValue = RS485.read();

			// Display it on the Serial Monitor as a char (not an integer value)
			Serial.print("                 Got back:");	Serial.println(char(rxValue));

			// Turn off LED
			digitalWrite(ledPin, LOW);  // Show activity
		}

		// Debugging delay so we can follow activity
		delay(100);
	}

	// Entire string sent, longer delay between loops
	while (!Serial.available());
	while (Serial.available()){Serial.read();}
	//delay(1500);
}
