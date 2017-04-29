#ifndef RELAYSHIELDMQTTCONTROLLER_VOLTAGESENSOR_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_VOLTAGESENSOR_CONFIG_H_


const unsigned long VOLTAGE_UPDATE_INTERVAL = 5UL * 60UL * 1000UL;

unsigned long voltagePreviousMillis = 0UL;

// Measurement topics
const char VOLTAGE_MEASUREMENT[] PROGMEM = "relayshield/measurement/voltage";

PGM_P const MEASUREMENT_TOPICS[] PROGMEM = {
    VOLTAGE_MEASUREMENT,     // idx = 0
};

/* STATUS_TOPICS indices, must match table above */
typedef enum {
  VOLTAGE_MEASUREMENT_IDX = 0,
} measurement_topics;

const byte VOLTAGE_PIN = A2;
const byte FLOAT_DECIMAL_PLACES = 1;

void voltage_sensor_init() {
  pinMode(VOLTAGE_PIN, INPUT);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
// 4.8v = 11.5v
float voltage() {
  float voltage_divide = analogRead(VOLTAGE_PIN) * (5.0 / 1023.0);
  return mapfloat(voltage_divide, 0.0, 5.0, 0.0, 12.0);
}

void publish_voltage() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(MEASUREMENT_TOPICS[VOLTAGE_MEASUREMENT_IDX])));
  // create message in format "idx,ON"
  // add relay index
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  payloadBuffer[0] = '\0';
  dtostrf(voltage(),1,FLOAT_DECIMAL_PLACES, payloadBuffer);
  DEBUG_LOG(1, "payloadBuffer: ");
  DEBUG_LOG(1, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}


#endif   /* RELAYSHIELDMQTTCONTROLLER_VOLTAGESENSOR_CONFIG_H_ */
