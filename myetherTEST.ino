/*
 * Simple code for demonstration of UDP packet sending
 * using Arduino UNO, HR91105A Hanrun Ethernet module and
 * EtherCard library (http://jeelabs.org/pub/docs/ethercard/)
 * 
 * define DEBUG 1 for printing serial messages with status of
 * connection, define DEBUG 0 to mute and save memory
 * 
 * Made by Jakub Jun 
 * https://github.com/jayjay221
 */

#define DEBUG 0 // 1 for serial messages at 9600 baud, 0 for none

#define BUFFERSIZE 700 // size of buffer in bytes
#define MACADDR { 0x74,0x69,0x69,0x2D,0x30,0x31 } // has to be unique within network
#define DESTINATIONADRESS "255.255.255.255" // ip adress of UDP server
#define SOURCEPORT 55056
#define DESTINATIONPORT 55056
#define MESSAGE "message" // char string representing message
#define PACKETDELAY 1000 // delay in ms between sending of packets

#define PRINT(message) Serial.println(message); delay(1000) // macro for simplification

#include <EtherCard.h>

uint8_t Ethernet::buffer[BUFFERSIZE];
static const uint8_t mymac[] = MACADDR;
static uint8_t adress[4];
char message[] = MESSAGE;

void setup() {

  #if DEBUG == 1
  Serial.begin(9600);
  #endif
  
  if (ether.begin(sizeof Ethernet::buffer, mymac)) { // initialize ethernet module
    #if DEBUG == 1
    PRINT("1. Ether.begin: OK");
  } else {
    PRINT("1. Ether.begin: FAIL");
    #endif
  }

  ether.parseIp(adress, DESTINATIONADRESS); // parse ip to memory
  
  if(ether.dhcpSetup()) { // let the dhcp server give you an ip
    #if DEBUG == 1
    PRINT("2. DHCP server: OK");
  } else {
    PRINT("2. DHCP server: FAIL");
    #endif
  }

  #if DEBUG == 1
  PRINT("3. info");
  ether.printIp(" - IP adress:   ", ether.myip); 
  ether.printIp(" - Gateway:   ", ether.gwip);
  ether.printIp(" - DHCP server: ", ether.dhcpip);
  PRINT("----------");
  #endif
}

void loop() {
  //ether.packetLoop(ether.packetReceive()) // if not working, uncomment this

  ether.sendUdp(message, sizeof message, SOURCEPORT, adress, DESTINATIONPORT); // send UDP packet
  
  #if DEBUG == 1
  PRINT("Sending packet");
  #endif
  
  delay( PACKETDELAY ); // delay sending of packets
}
