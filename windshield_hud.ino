#include "SevSeg.h"
#include "String.h"
#include <SoftwareSerial.h> 
#include <ELMduino.h>

SevSeg sevseg;
SevSeg sevseg2;
SevSeg sevseg3;
SoftwareSerial HC05(11, 12);
ELM327 myELM327;

uint32_t kph = 0;


void setup()
{
  Serial.begin(9600);

  while (!Serial) 
  {
    ;
  }
  Serial.println("Ready to connect\nDefault password is 1234\n"); 

  HC05.begin(9600);
  HC05.write("AT+RESET\n");
  HC05.write("AT+ROLE=1\n");
  HC05.write("AT+CMODE=0\n");
  HC05.write("AT+INIT\n");
  HC05.write("AT+BIND=0010,CC,4F3603\n"); //Replace with MAC Address of ELM327
  HC05.write("AT+PAIR=0010,CC,4F3603,20\n"); //Replace with MAC Address of ELM327, 20 is the timeout value
  HC05.write("AT+LINK=0010,CC,4F3603\n"); //Replace with MAC Address of ELM327
  delay(1000);
  myELM327.begin(HC05, true, 2000);

  byte numDigits = 1;
  byte digitPins[] = {};

  // Segment pins are switched to be readable in a reflexion: 7<->6  4<->3  5<->2
  byte segmentPins1[] = {5, 4, 3, 2, 7, 6, 8};
  byte segmentPins2[] = {25, 24, 23, 22, 27, 26, 28};
  byte segmentPins3[] = {32, 33, 30, 31, 35, 34, 36};

  bool resistorsOnSegments = true;
  byte hardwareConfigAnode = COMMON_ANODE; 
  byte hardwareConfigCathode = COMMON_CATHODE; 

  sevseg.begin(hardwareConfigAnode, numDigits, digitPins, segmentPins1, resistorsOnSegments);
  sevseg2.begin(hardwareConfigAnode, numDigits, digitPins, segmentPins2, resistorsOnSegments);
  sevseg3.begin(hardwareConfigAnode, numDigits, digitPins, segmentPins3, resistorsOnSegments);
}

void loop()
{
  if (HC05.available()) {
    Serial.write(HC05.read());
  }
  if (Serial.available()) {
    HC05.write(Serial.read());
  }

  float tempkph = myELM327.kph();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    kph = (uint32_t)tempkph;
    displayKmh(kph);
  }
  //sevseg.setNumber(4);
  //sevseg2.setNumber(9);
  //sevseg3.setNumber(-1);

  //sevseg.setBrightness(1); // -200 to 200
  //sevseg2.setBrightness(1); // -200 to 200
  //sevseg3.setBrightness(1); // -200 to 200

  //sevseg.refreshDisplay();
  //sevseg2.refreshDisplay();  
  //sevseg3.refreshDisplay();  
}

void displayKmh(uint32_t rpm)
{
    if (rpm < 10) 
    {
      sevseg3.setNumber(rpm);
    }
    else if (rpm < 100)
    {
      char buffer[2];
      itoa(rpm, buffer, 10);
      sevseg2.setNumber(buffer[0] - '0');
      sevseg3.setNumber(buffer[1] - '0');
    }
    else 
    {
      char buffer[3];
      itoa(rpm, buffer, 10);
  
      sevseg.setNumber(buffer[0] - '0');
      sevseg2.setNumber(buffer[1] - '0');
      sevseg3.setNumber(buffer[2] - '0');
    }
    
    sevseg.setBrightness(200); // -200 to 200
    sevseg2.setBrightness(200); // -200 to 200
    sevseg3.setBrightness(200); // -200 to 200
  
    sevseg.refreshDisplay();
    sevseg2.refreshDisplay();  
    sevseg3.refreshDisplay();  
}
