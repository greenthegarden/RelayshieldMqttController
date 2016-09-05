#ifndef RELAYSHIELDMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 50);        // openHAB
const char mqttClientId[]                         = "relayshield";
const int mqttPort                                = 1883;

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

boolean mqttClientConnected                       = false;

const char COMMAND_SEPARATOR                      = ',';

// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient mqttClient(mqttServerAddr, mqttPort, callback, ethernetClient);

// MQTT topic definitions

// MQTT payloads
const char MQTT_PAYLOAD_CONNECTED[]   PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_OK[]          PROGMEM = "OK";
const char MQTT_PAYLOAD_ERROR[]       PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[]       PROGMEM = "START";
const char MQTT_PAYLOAD_END[]         PROGMEM = "END";
const char MQTT_PAYLOAD_SLEEP[]       PROGMEM = "SLEEP";

PGM_P const MQTT_PAYLOADS[]           PROGMEM = { MQTT_PAYLOAD_CONNECTED,   // idx = 0
                                                  MQTT_PAYLOAD_OK,          // idx = 1
                                                  MQTT_PAYLOAD_ERROR,       // idx = 2
                                                  MQTT_PAYLOAD_START,       // idx = 3
                                                  MQTT_PAYLOAD_END,         // idx = 4
                                                  MQTT_PAYLOAD_SLEEP,       // idx = 5
                                                };

/* MQTT_PAYLOADS indices, must match table above */
typedef enum {
  MQTT_PAYLOAD_CONNECTED_IDX = 0,
  MQTT_PAYLOAD_OK_IDX        = 1,
  MQTT_PAYLOAD_ERROR_IDX     = 2,
  MQTT_PAYLOAD_START_IDX     = 3,
  MQTT_PAYLOAD_END_IDX       = 4,
  MQTT_PAYLOAD_SLEEP_IDX     = 5,
} mqtt_payloads;

// Status topics
const char CONNECTED_STATUS[]  PROGMEM = "relayshield/status/connected";
const char IP_ADDR_STATUS[]    PROGMEM = "relayshield/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "relayshield/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "relayshield/status/memory";
const char TIME_STATUS[]       PROGMEM = "relayshield/status/time";
const char ALARM_STATUS[]      PROGMEM = "relayshield/status/alarm";
const char RELAY_STATUS[]      PROGMEM = "relayshield/status/relay";

PGM_P const STATUS_TOPICS[]    PROGMEM = { CONNECTED_STATUS,    // idx = 0
                                           IP_ADDR_STATUS,      // idx = 1
                                           UPTIME_STATUS,       // idx = 2
                                           MEMORY_STATUS,       // idx = 3
                                           TIME_STATUS,         // idx = 4
                                           ALARM_STATUS,        // idx = 5
                                           RELAY_STATUS,        // idx = 6
                                         };
                                         
/* STATUS_TOPICS indices, must match table above */
typedef enum {
  CONNECTED_STATUS_IDX  = 0,
  IP_ADDR_STATUS_IDX    = 1,
  UPTIME_STATUS_IDX     = 2,
  MEMORY_STATUS_IDX     = 3,
  TIME_STATUS_IDX       = 4,
  ALARM_STATUS_IDX      = 5,
  RELAY_STATUS_IDX      = 6,
} status_topics;

void publish_connected() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[CONNECTED_STATUS_IDX])));
  mqttClient.publish(topicBuffer, "");
}

// Control topics
const char RELAY_CONTROL[]     PROGMEM = "relayshield/control/relay";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,         // idx = 0
                                         };

typedef enum {
  RELAY_CONTROL_IDX     = 0,
} control_topics;


#endif   /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */
