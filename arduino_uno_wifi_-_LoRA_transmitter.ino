#include "src/LoRa/LoRa.h"

// setari
#define SERIAL_BAUDRATE 115200

const long frequency = 433E6;  // LoRa Frequency
const int csPin = 6;           // LoRa radio chip select
const int resetPin = 7;        // LoRa radio reset
// const int resetPin = -1;       // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
const long SPIfrequency = 8E6; // SPI Frequency

long lastSendTime = 0;         // last send time
int interval = 500;            // interval between sends
int index = 1;                 // incrementor for message sending
bool high = false;

void setup() {
	Serial.begin(SERIAL_BAUDRATE);

	LoRa.setPins(csPin, resetPin, irqPin);
	// LoRa.setSPIFrequency(SPIfrequency);

	if (!LoRa.begin(frequency)) {
		Serial.println("LoRa init failed. Check your connections.");
		while(1); // if failed, do nothing
	}

	Serial.println("LoRa init succeeded.");

	lastSendTime = millis();
}

void loop() {
	if (millis() - lastSendTime > interval) {
		if (index % 5 == 0) { // every 5 packets we change fro low power to high and viceversa
			high = !high;
		}

		String message = "LoRa ";   // send a message
		message += index;   // send a message

		int txPower;
		if (high) {
			message += " H";
			txPower = 20;
		} else {
			txPower = 17;
		}
		LoRa.setTxPower(txPower);

		sendMessage(message);
		Serial.println("Sending " + message);

		lastSendTime = millis();
		index++;
	}
}

void sendMessage(String outgoing) {
	LoRa.beginPacket();                   // start packet
	LoRa.print(outgoing);                 // add payload
	LoRa.endPacket();                     // finish packet and send it
}