#include <LwRx.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#if EEPROM_EN == 1
#include <EEPROM.h>
#endif

unsigned long millisOffset = millis();

//Msg data
byte msg[10];
byte msglen = 10;
byte transmitter[6];
byte wifi[6] =  {0x3, 0x0, 0xB, 0x8, 0x9, 0x0};
byte lounge_remote[6] = {0xF, 0x1, 0x6, 0xF, 0xA, 0x0};


void setup() {
   // set up with rx into pin 2
   lwrx_setup(2);
   Serial.begin(9600);
   Serial.println("Set up completed and stats enabled");
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
   Serial.print(millis() - millisOffset);
   Serial.print(" ");
   for(int i=0;i<len;i++) {
      Serial.print(msg[i],HEX);
      Serial.print(" ");
   }
   for (int i=4;i<len;i++) {
      transmitter[i-4] = msg[i];
   }
   if (memcmp (transmitter, wifi, sizeof(transmitter)) == 0) {
     Serial.println("matched wifi controller");
   }
   if (memcmp (transmitter, lounge_remote, sizeof(transmitter)) == 0) {
     Serial.println("matched lounge_remote");
   }
   Serial.println();
}
