#ifndef RELAYSHIELDMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_ETHERNET_CONFIG_H_


#include <SPI.h>
#include <Ethernet.h>


const unsigned long NETWORK_STARTUP_DELAY = 1000UL;

//const byte          ETHERNET_CS_PIN       = 10;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient ethernetClient;

byte ethernet_init() {
  // Configure Ethernet
  delay(NETWORK_STARTUP_DELAY); // allow some time for Ethernet processor to come out of reset on Arduino power up:

//  NetEeprom.begin();
  if (Ethernet.begin(mac) != 0) {
    DEBUG_LOG(1, Ethernet.localIP());
    delay(NETWORK_STARTUP_DELAY);
    return 1;
  }
  return 0;
}


#endif   /* RELAYSHIELDMQTTCONTROLLER_ETHERNET_CONFIG_H_ */
