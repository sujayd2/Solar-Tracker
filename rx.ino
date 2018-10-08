#include <Queue.h>


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <queue.h>

const char* ssid = "Orignal sujay";
const char* password = "sujay1595";

const char* host = "https://spreadsheets.google.com";
const int httpsPort = 80;



void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  

  String url = "/feeds/cells/13yJJhNT1IIezNomMv1BGNWdWxnoutEnCUrM4knIaXmk/1/public/basic/R1C1?alt=json";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while(client.available()){
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}

void loop() {
}
