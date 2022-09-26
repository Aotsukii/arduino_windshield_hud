#include <SoftwareSerial.h>
#include "SevSeg.h"
#include <ELMduino.h>

SevSeg sevseg;

#define rxPin 10
#define txPin 11
#define baudrate 38400
String msg;
SoftwareSerial hc05(rxPin ,txPin);
ELM327 myELM327;

uint32_t kph = 0;
uint32_t rpm = 0;

int brightness = 100;
int dayBrightness = 100;
int nightBrightness = 0;

void setup()
{
    pinMode(rxPin,INPUT);
    pinMode(txPin,OUTPUT);
  
    Serial.begin(9600);
    Serial.println("ENTER AT Commands:");
    hc05.begin(baudrate);

    byte numDigits = 4;
    byte digitPins[] = {30, 31, 32, 33};
    byte segmentPins[] = {25, 23, 22, 29, 28, 26, 27, 24}; // reorder segments for reflexion display

    // 3x seven segment display setup
    bool resistorsOnSegments = true;
    bool updateWithDelaysIn = true;
    byte hardwareConfig = COMMON_CATHODE;
    
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    // sevseg.setNumber(420 , -1);
    // sevseg.setBrightness(brightness);
    // sevseg.refreshDisplay();

    // HC-05 force role + connect to mac address
    // OBDII MAC ADDRESS : 0010,CC,4F3603
    // PHONE MAC ADDRESS : E0DC,FF,F29306
    Serial.println("AT+RESET\n");
    Serial.println("AT+PSWD=1234\n");
    Serial.println("AT+ROLE=1\n");
    Serial.println("AT+CMODE=0\n");
    Serial.println("AT+INIT\n");
    Serial.println("AT+BIND=0010,CC,4F3603\n"); //Replace with MAC Address of ELM327
    Serial.println("AT+PAIR=0010,CC,4F3603,20\n"); //Replace with MAC Address of ELM327, 20 is the timeout value
    Serial.println("AT+LINK=0010,CC,4F3603\n"); //Replace with MAC Address of ELM327

    // Init elm327
    myELM327.begin(hc05, true, 2000);
}

void loop()
{
    readSerialPort();
    
    if (msg!="") 
    {
        hc05.println(msg);
    }
    
    if (hc05.available()>0)
    {
      Serial.write(hc05.read());
    }

    float tempKPH = myELM327.kph();
    float tempRPM = myELM327.rpm();
    if (myELM327.nb_rx_state == ELM_SUCCESS)
    {
        Serial.println(tempKPH);
        kph = (uint32_t)tempKPH;
        rpm = (uint32_t)tempRPM;
        sevseg.setNumber(kph , -1);
    }
    sevseg.setBrightness(brightness);
    sevseg.refreshDisplay();
}

void readSerialPort()
{
    msg="";
    while (Serial.available())
    {
        delay(10);  
        if (Serial.available() >0)
        {
            char c = Serial.read(); // Gets one byte from serial buffer
            msg += c; // Makes the string readString
        }
    }
}
