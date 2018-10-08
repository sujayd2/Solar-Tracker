#include <SPI.h> 
#include "RF24.h" 

#define led2 2 
RF24 myRadio (7, 8);//3,4     1-GND,2-3.3V NOT 5v, 5-13, 6-11, 7-12
byte addresses[][6] = {"1Node"};
int dataReceived;  
int servohb=0;
int servovs=0;
#define smallStepPin 6   //pin to pulse for steps
#define smallDirPin 4   //pin to change step direction
#define bigStepPin 3   //pin to pulse for steps
#define bigDirPin 2    //pin to change step direction
const float oneRotationBig = 4582;
const float oneRotationSmall = 2100;
int i=0;
int prevBigValue=90;
int prevSmallValue = 90;
int prevValue;
void setup()
{

  //set pins as outputs
  pinMode(smallStepPin, OUTPUT);
  pinMode(smallDirPin, OUTPUT);
  pinMode(bigStepPin, OUTPUT);
  pinMode(bigDirPin, OUTPUT);
  Serial.begin(9600);
    myRadio.begin(); 
    myRadio.setChannel(108); 
    myRadio.setPALevel(RF24_PA_MIN);
    myRadio.openReadingPipe(1, addresses[0]);
    myRadio.startListening();
//    stepperFWD();
//    while(true)
//  {
//    motorStep("small");
//    i++;
//    Serial.println(i);
//}
  
}


void loop()
{

    if ( myRadio.available())
  {
    while (myRadio.available())
    {
      myRadio.read( &dataReceived, sizeof(dataReceived) );
      }
    //Serial.println(dataReceived);
     if(dataReceived>1500 && dataReceived<3000)
      {
        servovs=dataReceived-2000;
        Serial.print("servovs: ");
        Serial.println(servovs);
        rotateDegree("small", servovs);
        prevSmallValue=servovs;
      }
      else if(dataReceived>500 && dataReceived<1500)
     {
      //Serial.println(dataReceived);
        servohb=dataReceived-1000;
        Serial.print("servohb: ");
        Serial.println(servohb);
        rotateDegree("big", servohb);
        prevBigValue=servohb;
      }
      
  } 
}

void rotateDegree(String motor,int degree)    //"big" "small"
{
  int constant;
  if(motor == "big")
  {
    constant=oneRotationBig;
    prevValue = prevBigValue;
  } 
  else if (motor == "small") 
  {
    constant=oneRotationSmall;
    prevValue = prevSmallValue;
  }
  if(degree>90)
  {
    //Serial.println("outside");
    degree=degree-prevValue;
    Serial.println(degree);
    stepperREV();
    float value = ((constant / 360)*degree);
    while(i<=(int)value)
    {
      //Serial.println("hllo");
      motorStep(motor);
      i++;
    }
    i=0;
    value=0;
  }
    else if(degree<=90)
    {
      //Serial.println("inside");
      degree=prevValue-degree;
      Serial.println(degree);
      stepperFWD();
      float value = ((constant / 360)*degree);
      while(i<=(int)value)
     {
      //Serial.println("hello");
      motorStep(motor);
      i++;
     }
     i=0;
     value=0;
    }
}

//change the stepper direction to forward
void stepperFWD()
{
  digitalWrite(smallDirPin, HIGH);
  digitalWrite(bigDirPin, HIGH);
}

////change the stepper direction to reverse
void stepperREV()
{
  digitalWrite(smallDirPin, LOW);
  digitalWrite(bigDirPin, LOW);
}

////have the stepper motor take one step
////DO NOT CHANGE DELAY
void motorStep(String siz)
{
  if(siz == "small")
  {
    digitalWrite(smallStepPin, HIGH);
    delay(3);
    digitalWrite(smallStepPin, LOW);
    delay(3);
  } 
  else if (siz == "big")
  {
    digitalWrite(bigStepPin, HIGH);
    delay(3);
    digitalWrite(bigStepPin, LOW);
    delay(3);
  }
}
//
//
//
//
//
//ItKindaWorks - Creative Commons 2016
//github.com/ItKindaWorks
//
//Simple sctepper motor control demo

//const int stepPin = 4;   //pin to pulse for steps
//const int dirPin = 3;    //pin to change step direction
//
//void setup(){
//
//  //set pins as outputs
//  pinMode(stepPin, OUTPUT);
//  pinMode(dirPin, OUTPUT);
//}
//
//
//
//void loop(){
//
//  //step forward 100 steps
//  stepperFWD();
//  for(int i = 0; i < 100; i++){
//    motorStep();
//    delay(1);
//  }
//
//  //step backwards 100 steps
//  stepperREV();
//  for(int i = 0; i < 100; i++){
//    motorStep();
//    delay(1);
//  }
//
//}








