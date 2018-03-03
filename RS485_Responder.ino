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
void setup()
{
  // Debugging window
  Serial.begin(9600);

  // Set modes for pins
  pinMode(ledPin, OUTPUT);
  pinMode(RS485inout, OUTPUT);

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
  // Is there something on the serial pin?
  if (RS485.available())
  {
    txValue = RS485.read();
    Serial.print("Received:");Serial.println(char(txValue));

    // Turn on the on-board LED pin to show activity
    digitalWrite(ledPin, HIGH);

    // Set the direction pin to xmit (HIGH)
    digitalWrite(RS485inout, RS485Transmit);

    // This delay is REQUIRED for things to settle before transmission
    delay(10);

    // Send back a modified value
    RS485.write(txValue);

    // Wait for the transmission to complete.
    // This delay is dependent on the baud rate. To calculate:
    // delay = (baud rate / 10) * 2
    int bufferDelay = (baudRate / 10) * 2 + 100;
    delayMicroseconds(bufferDelay);

    // Set the RS485 back to receive
    digitalWrite(RS485inout, RS485Receive);

    // Turn off built in LED pin
    digitalWrite(ledPin, LOW);
  }
}
