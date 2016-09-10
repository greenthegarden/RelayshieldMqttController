#ifndef RELAYSHIELDMQTTCONTROLLER_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_CONFIG_H_


#include "debug.h"

// external libraries
#include <MemoryFree.h>

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// global variable definitions
#if DEBUG_LEVEL > 0
const int BAUD_RATE               = 9600;
#endif

const byte BUFFER_SIZE            = 32;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];

#include "ethernetConfig.h"
#include "mqttConfig.h"
#include "relayConfig.h"

const unsigned long STATUS_UPDATE_INTERVAL = 10000UL;
unsigned long statusPreviousMillis = 0UL;

const byte STATUS_LED_CONTROL_PIN = 13;

void no_network_behaviour() {
  relays_switch_off();
}

const unsigned long LED_ON_DELAY  = 1000UL;
const unsigned long LED_OFF_DELAY = 500UL;

void status_led_blink(byte blinks)
{
  for (byte cnt = 0; cnt < blinks; cnt++) {
    digitalWrite(STATUS_LED_CONTROL_PIN, HIGH);
    delay(LED_ON_DELAY);
    digitalWrite(STATUS_LED_CONTROL_PIN, LOW);
    delay(LED_OFF_DELAY);
  }
}


#endif   /* RELAYSHIELDMQTTCONTROLLER_CONFIG_H_ */
