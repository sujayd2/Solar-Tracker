#include <Servo.h> // include Servo library 
#include <SoftwareSerial.h>
#include <SPI.h>
#include "RF24.h"
#include <string.h>

RF24 myRadio (7, 8);
byte addresses[][6] = {"1Node"};
int dataTransmitted;
String voice;

SoftwareSerial Ser (3,4);

Servo horizontal; // horizontal servo
int servoh = 90; // stand horizontal servo

Servo vertical; // vertical servo 
int servov = 90; // stand vertical servo

// LDR pin connections
// name = analogpin;
String data;
int counter=0;
int counter2=0;
int ldrlt = 0; //LDR top left
int ldrrt = 1; //LDR top rigt
int ldrld = 2; //LDR down left
int ldrrd = 3; //ldr down rigt

void setup()
{
  Ser.begin(9600);
  Serial.begin(9600);
// servo connections
// name.attacht(pin);
  horizontal.attach(9);
  vertical.attach(10);  
  myRadio.begin();
  myRadio.setChannel(108);
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop() 
{
  int lt = analogRead(ldrlt); // top left
  int rt = analogRead(ldrrt); // top right
  int ld = analogRead(ldrld); // down left
  int rd = analogRead(ldrrd); // down rigt

  int dtime = analogRead(4)/20; // read potentiometers
  int tol = analogRead(5)/4;

int avt = (lt + rt) / 2; // average value top
int avd = (ld + rd) / 2; // average value down
int avl = (lt + ld) / 2; // average value left
int avr = (rt + rd) / 2; // average value right

int dvert = avt - avd; // check the diffirence of up and down
int dhoriz = avl - avr;// check the diffirence og left and rigt

if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
{
if (avt > avd)
{
servov = ++servov;
if (servov > 180)
{
servov = 180;
}
}
else if (avt < avd)
{
servov= --servov;
if (servov < 0)
{
servov = 0;
}
}
vertical.write(servov);
}

if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
{
if (avl > avr)
{
servoh = --servoh;
if (servoh < 0)
{
servoh = 0;
}
}
else if (avl < avr)
{
servoh = ++servoh;
if (servoh > 180)
{
servoh = 180;
}
}
else if (avl == avr)
{
// nothing
}
horizontal.write(servoh);
}
delay(dtime);

String data = "{\"d\":{";
data+="\"servoh\":";
data+=servoh;
data+=",\"servov\":";
data+=servov;
data+="}}";
data+="#";

counter++;
//counter2++;
//Serial.println(counter);
while(counter>500)// for 10 seconds make counter 1600
{
  Ser.println(data);
  Serial.println(data);

  dataTransmitted= servov+2000;
  Serial.println(servov);
  myRadio.write( &dataTransmitted, sizeof(dataTransmitted) );
  
  Serial.println(servoh);
  dataTransmitted= servoh+1000;
  myRadio.write( &dataTransmitted, sizeof(dataTransmitted) );
  counter=0;
  
}

 
}


