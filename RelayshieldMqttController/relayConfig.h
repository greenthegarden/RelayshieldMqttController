#ifndef RELAYSHIELDMQTTCONTROLLER_RELAY_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_RELAY_CONFIG_H_


typedef enum {
  OFF = 0,
  ON  = 1,
} states_t;

// forward declarations of relay switch functions
byte relay_switch_off(byte idx, boolean report=true);
byte relay_switch_on(byte idx, boolean report=true);

// define relay control pins
const byte RELAY_1_CONTROL_PIN = 7;  // Relay 1 is connected to Arduino Digital 7
const byte RELAY_2_CONTROL_PIN = 6;  // Relay 2 is connected to Arduino Digital 6
const byte RELAY_3_CONTROL_PIN = 5;  // Relay 3 is connected to Arduino Digital 5
const byte RELAY_4_CONTROL_PIN = 4;  // Relay 4 is connected to Arduino Digital 4

/*
// define relay connection types of 
// NC - Normally Closed, in which case NC pin is connected with COM pin when control pin is set low and disconnected when control pin is high;
// NO - Normally Open, in which case NO pin is disconnected with COM pin when control pin is set low and connected when control pin is high.
typedef enum {
  NC, NO
} relayConnection_t;
*/

// define structure to represent a relay
typedef struct {
   byte               controlPin;
//   byte               ledPin;
//   relayConnection_t  relayConnection;
//   byte               state;
} relay_t;

relay_t relays[] = { { RELAY_1_CONTROL_PIN },
                     { RELAY_2_CONTROL_PIN },
                     { RELAY_3_CONTROL_PIN },
                     { RELAY_4_CONTROL_PIN },
                   };

void relays_init()
{
  for (byte idx = 0; idx < ARRAY_SIZE(relays); idx++) {
    pinMode(relays[idx].controlPin, OUTPUT);
    digitalWrite(relays[idx].controlPin, LOW);
  }
}

// returns 1 if relay connected to given pin is on, else returns 0
byte relay_state(byte idx)
{
  return(digitalRead(relays[idx].controlPin));
}

void publish_relay_state(byte relayIdx, boolean relayState)
{
  payloadBuffer[0] = '\0';
  if (relayState == ON) { // relay ON
    DEBUG_LOG(1, "relay on");
    sprintf(payloadBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 1);
  } else {
    DEBUG_LOG(1, "relay off");
    sprintf(payloadBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 0);
  }
  DEBUG_LOG(1, "payloadBuffer: ");
  DEBUG_LOG(1, payloadBuffer);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[RELAY_STATUS_IDX])));
  // create message in format "idx,ON"
  // add relay index
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

// returns 1 if relay is currently on and switched off, else returns 0
byte relay_switch_off(byte idx, boolean report)
{
  // only switch relay off if it is currently on
  if (relay_state(idx) == ON) {
    digitalWrite(relays[idx].controlPin, LOW);
    DEBUG_LOG(1, "relay off");
    if (report) {
      publish_relay_state(idx, false);
    }
    return 1;
  } else {
    DEBUG_LOG(1, "relay already off");
  }
  return 0;
}

// returns 1 if relay is currently off and switched on, else returns 0
byte relay_switch_on(byte idx, boolean report)
{
  if (relay_state(idx) == OFF) {
    digitalWrite(relays[idx].controlPin, HIGH);
    DEBUG_LOG(1, "relay on");
    if (report) {
      publish_relay_state(idx, true);
    }
    return 1;
  } else {
    DEBUG_LOG(1, "relay already on");
  }
  return 0;
}

void relays_switch_off()
{
  for (byte idx = 0; idx < ARRAY_SIZE(relays); idx++) {
    if (relay_state(idx) == ON)
      relay_switch_off(idx);
  }
}

void relays_switch_on()
{
  for (byte idx = 0; idx < ARRAY_SIZE(relays); idx++) {
    if (relay_state(idx) == OFF)
      relay_switch_on(idx);
  }
}



// take into account NC, NO
//relays_default_state() {
//  for 
//}


#endif   /* RELAYSHIELDMQTTCONTROLLER_RELAY_CONFIG_H_ */

