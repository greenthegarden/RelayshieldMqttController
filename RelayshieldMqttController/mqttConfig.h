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
const char VERSION_STATUS[]    PROGMEM = "relayshield/status/version";
const char INTERVAL_STATUS[]   PROGMEM = "relayshield/status/interval";
const char IP_ADDR_STATUS[]    PROGMEM = "relayshield/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "relayshield/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "relayshield/status/memory";
const char TIME_STATUS[]       PROGMEM = "relayshield/status/time";
const char ALARM_STATUS[]      PROGMEM = "relayshield/status/alarm";
const char RELAY_STATUS[]      PROGMEM = "relayshield/status/relay";

PGM_P const STATUS_TOPICS[]    PROGMEM = { CONNECTED_STATUS,    // idx = 0
                                           VERSION_STATUS,      // idx = 1
                                           INTERVAL_STATUS,     // idx = 2
                                           IP_ADDR_STATUS,      // idx = 3
                                           UPTIME_STATUS,       // idx = 4
                                           MEMORY_STATUS,       // idx = 5
                                           TIME_STATUS,         // idx = 6
                                           ALARM_STATUS,        // idx = 7
                                           RELAY_STATUS,        // idx = 8
                                         };

/* STATUS_TOPICS indices, must match table above */
typedef enum {
  CONNECTED_STATUS_IDX  = 0,
  VERSION_STATUS_IDX    = 1,
  INTERVAL_STATUS_IDX   = 2,
  IP_ADDR_STATUS_IDX    = 3,
  UPTIME_STATUS_IDX     = 4,
  MEMORY_STATUS_IDX     = 5,
  TIME_STATUS_IDX       = 6,
  ALARM_STATUS_IDX      = 7,
  RELAY_STATUS_IDX      = 8,
} status_topics;

// Control topics
const char RELAY_CONTROL[]     PROGMEM = "relayshield/control/relay";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,         // idx = 0
                                         };

/* CONTROL_TOPICS indices, must match table above */
typedef enum {
  RELAY_CONTROL_IDX     = 0,
} control_topics;

void publish_connected() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[CONNECTED_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_version() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[VERSION_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  sprintf(payloadBuffer, "%i.%i", VERSION_MAJOR, VERSION_MINOR);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_status_interval() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[INTERVAL_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(STATUS_UPDATE_INTERVAL, payloadBuffer, 10));
}

void publish_ip_address() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  IPAddress ip = Ethernet.localIP();
  sprintf(payloadBuffer,
    "%i%c%i%c%i%c%i",
    ip[0], '.',
    ip[1], '.',
    ip[2], '.',
    ip[3]
  );
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_uptime() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
}

void publish_memory() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MEMORY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
}

void publish_status() {
  publish_version();
  publish_status_interval();
  publish_ip_address();
  publish_uptime();
  publish_memory();
}


#endif   /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */
