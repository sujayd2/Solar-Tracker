#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
#include <Servo.h> // include Servo library 
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX

char ssid[] = "Orignal sujay";            // your network SSID (name)
char pass[] = "sujay1595";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vE0AAADC46015BCD"; //device ID from Pushingbox 

WiFiEspServer server(80);




Servo horizontal; // horizontal servo
int servoh = 90; // stand horizontal servo

Servo vertical; // vertical servo 
int servov = 90; // stand vertical servo

// LDR pin connections
// name = analogpin;
int ldrlt = 0; //LDR top left
int ldrrt = 1; //LDR top rigt
int ldrld = 2; //LDR down left
int ldrrd = 3; //ldr down rigt

void setup()
{
  // put your setup code here, to run once:

 Serial.begin(9600);

  horizontal.attach(9); 
  vertical.attach(10);
  
 Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
 
  
  // start the web server on port 80
  server.begin();
}
/*int servov_final = servov;
int servoh_final = servoh;
int servov_ready = 0;
int servoh_ready = 0;*/
void loop() 
{
  // put your main code here, to run repeatedly:
  delay(10000); //10 seconds, (sampling rate vs. service call quota)

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
int dhoriz = avl - avr;// check the diffirence of left and rigt

//if((dvert<10)||(dvert>-10))
//{
// servov_ready = 0;
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
//servov_final += servov;
}
//}
/*else
{
  servov_ready = 1;
}*/

//if((dhoriz<10)||(dhoriz>-10))
//{
// servoh_ready = 0;

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
//servoh_final += servoh;
}
//}
/*else
{
    servoh_ready = 1;
}*/
delay(dtime);

WiFiEspClient client;

 //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
         client.print("GET /pushingbox?devid=" + devid
       + "&humidityData=" + "vatsa;" 
       + "&celData="      + "vat"   
       );
       //(String) servoh
       //(String) servov
      client.println(" HTTP/1.1"); 
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: ESP8266/1.0");
      client.println("Connection: close");
      client.println();
      }
}
