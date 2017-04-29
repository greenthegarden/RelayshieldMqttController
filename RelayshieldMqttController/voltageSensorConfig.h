#ifndef RELAYSHIELDMQTTCONTROLLER_VOLTAGESENSOR_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_VOLTAGESENSOR_CONFIG_H_


const unsigned long VOLTAGE_UPDATE_INTERVAL = 1UL * 60UL * 1000UL;

unsigned long voltagePreviousMillis = 0UL;

// Measurement topics
const char VOLTAGE_MEASUREMENT_TOPIC[] PROGMEM = "relayshield/measurement/voltage";

PGM_P const VOLTAGE_MEASUREMENT_TOPICS[] PROGMEM = {
    VOLTAGE_MEASUREMENT_TOPIC,     // idx = 0
};

/* STATUS_TOPICS indices, must match table above */
typedef enum {
  VOLTAGE_MEASUREMENT_TOPIC_IDX = 0,
} voltage_measurement_topics;

const byte VOLTAGE_SENSOR_PIN = A2;

void voltage_sensor_init() {
  pinMode(VOLTAGE_SENSOR_PIN, INPUT);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

// 4.8v = 11.5v
float voltage() {
  float reading = analogRead(VOLTAGE_SENSOR_PIN) * (5.0 / 1023.0);
  return mapfloat(reading, 0.0, 5.0, 0.0, 14.0);
}

void publish_voltage() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(VOLTAGE_MEASUREMENT_TOPICS[VOLTAGE_MEASUREMENT_TOPIC_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(voltage(),1,FLOAT_DECIMAL_PLACES, payloadBuffer);
  DEBUG_LOG(1, topicBuffer);
  DEBUG_LOG(1, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}


#endif   /* RELAYSHIELDMQTTCONTROLLER_VOLTAGESENSOR_CONFIG_H_ */
