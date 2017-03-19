#include <ESP8266WiFi.h>

const char* ssid = "NETWORK NAME";
const char* password = "PASSWORD";

WiFiServer server(1234);

#define DEBUG false

void setup()
{
  Serial.begin(115200);

  if (DEBUG) Serial.println("START");
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.begin(ssid, password);

  if (DEBUG) Serial.println("CONNECTING");

  WiFi.config(IPAddress(192,168,1,123), IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  if (DEBUG) Serial.println("CONFIG");
  
  bool status = true;
  while (WiFi.status() != WL_CONNECTED){
    if (DEBUG) Serial.println("CONNECTING...");
    delay(500);
    digitalWrite(LED_BUILTIN, status);
    status = !status;
  }
  if (DEBUG) Serial.println("CONNECTED");
  
  server.begin();
  digitalWrite(LED_BUILTIN, HIGH);
  if (DEBUG) Serial.println("Server started");
}


void loop(){
  
  WiFiClient client = server.available(); 
  
  if (client){
    digitalWrite(LED_BUILTIN, LOW);
    
    while (client.connected()){

      if ( client.available() )
      {
        Serial.write( client.read() );
      }
    
      if ( Serial.available() )
      {
        client.write( Serial.read() );
      }
    }

    digitalWrite(LED_BUILTIN, HIGH);
  }
}
