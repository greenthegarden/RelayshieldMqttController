#include <Arduino.h>

#include "config.h"

boolean mqtt_connect() {
  DEBUG_LOG(1, "Attempting MQTT connection ...");
  if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    DEBUG_LOG(1, "  connected");
    // Once connected, publish an announcement ...
    publish_connected();
    publish_configuration();
    publish_status();
    // ... and subscribe to topics (should have list)
    mqttClient.subscribe("relayshield/control/#");
  } else {
    DEBUG_LOG(1, "failed, rc = ");
    DEBUG_LOG(1, mqttClient.state());
  }
  return mqttClient.connected();
}

void callback(char *topic, uint8_t *payload, unsigned int payloadLength) {
  // handle message arrived
  /* topic = part of the variable header:has topic name of the topic where the
     publish received
       NOTE: variable header does not contain the 2 bytes with the
            publish msg ID
      payload = pointer to the first item of the buffer array that
                contains the message tha was published
               EXAMPLE of payload: lights,1
      length = the length of the payload, until which index of payload
  */

  DEBUG_LOG(1, "Payload length is");
  DEBUG_LOG(1, payloadLength);

  // Copy the payload to the new buffer
  char *message =
      (char *)malloc((sizeof(char) * payloadLength) +
                     1); // get the size of the bytes and store in memory
  memcpy(message, payload, payloadLength * sizeof(char)); // copy the memory
  message[payloadLength * sizeof(char)] = '\0'; // add terminating character

  DEBUG_LOG(1, "Message with topic");
  DEBUG_LOG(1, topic);
  DEBUG_LOG(1, "arrived with payload");
  DEBUG_LOG(1, message);

  boolean controlTopicFound = false;

  // check control topics
  byte topicIdx = 0;
  // find if topic is matched
  for (byte idx = 0; idx < ARRAY_SIZE(CONTROL_TOPICS); idx++) {
    topicBuffer[0] = '\0';
    strcpy_P(topicBuffer, (PGM_P)pgm_read_word(&(CONTROL_TOPICS[idx])));
    if (strcmp(topic, topicBuffer) == 0) {
      topicIdx = idx;
      controlTopicFound = true;
      break;
    }
  }

  if (controlTopicFound) {
    DEBUG_LOG(1, "Control topic index");
    DEBUG_LOG(1, topicIdx);
    // switch to case statements
    if (topicIdx == RELAY_CONTROL_IDX) { // topic is RELAY_CONTROL
      DEBUG_LOG(1, "RELAY_CONTROL topic arrived");

      char *separator = strchr(message, COMMAND_SEPARATOR);
      DEBUG_LOG(1, "separator: ");
      DEBUG_LOG(1, separator);
      if (separator != 0) {
        byte relayIdx =
            atoi(message) - 1; // relay numbers in message from 1 to 4
        if (relayIdx < ARRAY_SIZE(relays)) {
          DEBUG_LOG(1, "relayIdx: ");
          DEBUG_LOG(1, relayIdx);
          ++separator;
          byte state = atoi(separator);
          DEBUG_LOG(1, "state: ");
          DEBUG_LOG(1, state);
          if (state == ON) {
            relay_switch_on(relayIdx);
          } else {
            relay_switch_off(relayIdx);
          }
        }
      }
    } else { // unknown control topic has arrived - ignore!!
      DEBUG_LOG(1, "Unknown control topic arrived");
    }
  }

  // free memory assigned to message
  free(message);
}

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup() {
#if DEBUG_LEVEL > 0
  Serial.begin(BAUD_RATE);
#endif

  if (ethernet_init()) {
    DEBUG_LOG(1, "Ethernet configured");
    status_led_blink(2);
  } else {
    DEBUG_LOG(1, "Ethernet failed");
    DEBUG_LOG(1, "Halting");
    status_led_blink(3);
    for (;;)
      ;
  }

  // Connect to MQTT client
  if (mqttClient.connected()) {
    status_led_blink(4);
  } else {
    status_led_blink(5);
  }

  // configure relay pins as outputs and set to off (where off depends on
  // connection type)
  relays_init();
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop() {
  unsigned long now = millis();

  if (!mqttClient.connected()) {
    mqttClientConnected = false;
    if (now - lastReconnectAttempt >= RECONNECTION_ATTEMPT_INTERVAL) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
        mqttClientConnected = true;
      }
    }
  } else {
    // Client connected
    mqttClient.loop();
  }

  if (now - statusPreviousMillis >= STATUS_UPDATE_INTERVAL) {
    if (mqttClientConnected) {
      statusPreviousMillis = now;
      publish_status();
    }
  }

  if (!mqttClientConnected) {
    no_network_behaviour();
  }
}
