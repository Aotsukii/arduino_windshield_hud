#include "SevSeg.h"
#include "String.h"

SevSeg sevseg; 
SevSeg sevseg2;
SevSeg sevseg3;

void setup()
{
  byte numDigits = 1;
  byte digitPins[] = {};
  // Segment pins are switched to be readable in a reflexion: 7<->6  4<->3  5<->2
  byte segmentPins1[] = {5, 4, 3, 2, 7, 6, 8};
  byte segmentPins2[] = {25, 24, 23, 22, 27, 26, 28};
  byte segmentPins3[] = {32, 33, 30, 31, 35, 34, 36};

  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_ANODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins1, resistorsOnSegments);
  sevseg2.begin(hardwareConfig, numDigits, digitPins, segmentPins2, resistorsOnSegments);
  sevseg3.begin(hardwareConfig, numDigits, digitPins, segmentPins3, resistorsOnSegments);
}

void loop()
{
  displayKmh();
}

void displayKmh()
{
  for (int i = 1; i <= 999; i++)
  {
    if (i < 10) 
    {
      sevseg3.setNumber(i);
    }
    else if (i < 100)
    {
      char buffer[2];
      itoa(i, buffer, 10);
      sevseg2.setNumber(buffer[0] - '0');
      sevseg3.setNumber(buffer[1] - '0');
    }
    else 
    {
      char buffer[3];
      itoa(i, buffer, 10);
  
      sevseg.setNumber(buffer[0] - '0');
      sevseg2.setNumber(buffer[1] - '0');
      sevseg3.setNumber(buffer[2] - '0');
    }
    
    sevseg.setBrightness(-200); // -200 to 200
    sevseg2.setBrightness(-200); // -200 to 200
    sevseg3.setBrightness(-200); // -200 to 200
  
    sevseg.refreshDisplay();
    sevseg2.refreshDisplay();  
    sevseg3.refreshDisplay();  
    delay(5);
  }
}
