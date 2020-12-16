#include <LiquidCrystal.h>


/*
>> Pulse Sensor Amped 1.2 <<
This code is for Pulse Sensor Amped by Joel Murphy and Yury Gitman
    www.pulsesensor.com 
    >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
Pulse Sensor sample aquisition and processing happens in the background via Timer 2 interrupt. 2mS sample rate.
PWM on pins 3 and 11 will not work when using this code, because we are using Timer 2!
The following variables are automatically updated:
Signal :    int that holds the analog signal data straight from the sensor. updated every 2mS.
IBI  :      int that holds the time interval between beats. 2mS resolution.
BPM  :      int that holds the heart rate value, derived every beat, from averaging previous 10 IBI values.
QS  :       boolean that is made true whenever Pulse is found and BPM is updated. User must reset.
Pulse :     boolean that is true when a heartbeat is sensed then false in time with pin13 LED going out.

This code is designed with output serial data to Processing sketch "PulseSensorAmped_Processing-xx"
The Processing sketch is a simple data visualizer. 
All the work to find the heartbeat and determine the heartrate happens in the code below.
Pin 13 LED will blink with heartbeat.
If you want to use pin 13 for something else, adjust the interrupt handler
It will also fade an LED on pin fadePin with every beat. Put an LED and series resistor from fadePin to GND.
Check here for detailed code walkthrough:
http://pulsesensor.myshopify.com/pages/pulse-sensor-amped-arduino-v1dot1

Code Version 1.2 by Joel Murphy & Yury Gitman  Spring 2013
This update fixes the firstBeat and secondBeat flag usage so that realistic BPM is reported.

*/


//  VARIABLES
int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

LiquidCrystal lcd(12,11,5,4,3,2);

void setup()
{
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  Serial.begin(9600);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
  
   // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
   // AND APPLY THAT VOLTAGE TO THE A-REF PIN
   //analogReference(EXTERNAL); 

    
    delay(200);

    lcd.begin(16,2);
    lcd.print("Heart Rate is:");
    
}



void loop(){
  //sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
                         // Set 'fadeRate' Variable to 255 to fade LED with pulse
        //sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
        //sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
        QS = false;                      // reset the Quantified Self flag for next time    
     }
  
  delay(20);                             //  take a break

  lcd.setCursor(0,1);
  //lcd.print("   ");
  lcd.print(BPM);
  //Serial.println(BPM);
  //Serial.println(sizeof(BPM));
  if(BPM<100)
  {
    lcd.setCursor(2,1);
    lcd.print(" ");
  }
  lcd.setCursor(3,1);
  lcd.print("bpm");

  if(BPM<70)
  {
    lcd.setCursor(8,1);
    lcd.print("(Low)   ");
    
    Serial.println("AT+CMGF=1");
    delay(100);
    Serial.println("AT+CMGS=\"+919916520176\"");
    delay(100);
    Serial.print("Low Heart Rate: ");
    Serial.print(BPM);
    Serial.println(" bpm");
    delay(100);
    Serial.println((char)26);
    delay(6000);

  }
  else if(BPM>75)
  {
    lcd.setCursor(8,1);
    lcd.print("(High)  ");
    
    Serial.println("AT+CMGF=1");
    delay(100);
    Serial.println("AT+CMGS=\"+919916520176\"");
    delay(100);
    Serial.print("High Heart Rate: ");
    Serial.print(BPM);
    Serial.println(" bpm");
    delay(100);
    Serial.println((char)26);
    delay(6000);

  }
  else
  {
    lcd.setCursor(8,1);
    lcd.print("(Normal)");
    
    Serial.println("AT+CMGF=1");
    delay(100);
    Serial.println("AT+CMGS=\"+919916520176\"");
    delay(100);
    Serial.print("Normal Heart Rate: ");
    Serial.print(BPM);
    Serial.println(" bpm");
    delay(100);
    Serial.println((char)26);
    delay(6000);

  }
  
}


//void sendDataToProcessing(char symbol, int data ){
  //  Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
    //Serial.println(data);                // the data to send culminating in a carriage return
 // }







