#include <LwRx.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#if EEPROM_EN == 1
#include <EEPROM.h>
#endif

//Msg data
byte msg[10];
byte msglen = 10;

// Transmitter data
byte transmitter[6];
byte command;
byte dimlevel[2];
byte subunit;
byte wifir1[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x0};
byte wifir2[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x1};
byte wifir3[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x2};
byte wifir4[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x3};
byte wifir5[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x4};
byte wifir6[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x5};
byte lounge_remote[6] = {0xF, 0x1, 0x6, 0xF, 0xA, 0x0};
byte misc_remote[6] = {0xF, 0x3, 0x7, 0x1, 0xF, 0x0};
byte knowndevice = 0;

// MQTT
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 100, 112 };
byte ip[]     = { 192, 168, 100, 219 };

String message ="";

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);


void setup() {
   // set up with rx into pin 2
   Serial.begin(9600);
   Serial.println("Initialising Ethernet Stack and connecting to MQTT");
   Ethernet.begin(mac, ip);
   client.connect("nook-arduinoClient", "domoticz", "domoticz");
   Serial.println("Initialsing 433 Module");
   lwrx_setup(2);
   Serial.println("Set up completed");
}

void loop() {
   //collect any incoming command message and execute when complete
   if (lwrx_message()) {
      lwrx_getmessage(msg, msglen);
      printMsg(msg, msglen);
   }
   delay(5);
}

/**
   Retrieve and print out received message
**/
void printMsg(byte *msg, byte len) {
   for(int i=0;i<len;i++) {
      Serial.print(msg[i],HEX);
      Serial.print(" ");
   }
   for (int i=4;i<len;i++) {
      transmitter[i-4] = msg[i];
   }
   subunit = msg[2];
   command = msg[3];
   for (int i=0;i<2;i++) {
      dimlevel[i] = msg[i];
   }
   if (memcmp (transmitter, wifir1, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller room 1");
   }
   if (memcmp (transmitter, wifir2, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller room 2");
   }
   if (memcmp (transmitter, wifir3, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller room 3");
   }
   if (memcmp (transmitter, wifir4, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller room 4");
   }
   if (memcmp (transmitter, wifir5, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller room 5");
   }
   if (memcmp (transmitter, wifir6, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller room 6");
   }
   if (memcmp (transmitter, lounge_remote, sizeof(transmitter)) == 0) {
     Serial.println("matched lounge_remote");
   }
   if (memcmp (transmitter, misc_remote, sizeof(transmitter)) == 0) {
     Serial.println("matched misc_remote");
   }
   Serial.print("dimlevel is ");
   Serial.print(dimlevel[0]);
   Serial.println(dimlevel[1]);
   Serial.print("command is ");
   if (command == 0) {
     Serial.println("off");
   }
   else if (command == 1) {
     Serial.println("on");
   };
   Serial.print("subunit is ");
   Serial.println(subunit);
}
