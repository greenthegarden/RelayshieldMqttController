#ifndef RELAYSHIELDMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_ETHERNET_CONFIG_H_


// Use NetEEPROM library read MAC address from EEPROM
#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <NetEEPROM.h>


const unsigned long NETWORK_STARTUP_DELAY = 1000UL;

//const byte          ETHERNET_CS_PIN       = 10;

EthernetClient ethernetClient;

byte ethernet_init()
{
  // Configure Ethernet
  delay(NETWORK_STARTUP_DELAY); // allow some time for Ethernet processor to come out of reset on Arduino power up:

  //Ethernet.begin(mac);
  NetEeprom.begin();
//  if (NetEeprom.begin() == 0) {
//    DEBUG_LOG(1, "IP failed");
//  } else {
    DEBUG_LOG(1, Ethernet.localIP());
    delay(NETWORK_STARTUP_DELAY);
//    return 1;
//  }
  return 1;
}


#endif   /* RELAYSHIELDMQTTCONTROLLER_ETHERNET_CONFIG_H_ */
