/*
  Box1

  modified  August 2017
  by Jacoby Daniel
  
*/

// Board NodeMCU 0.9 (ESP-12 Module)

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <ArduinoJson.h> 
#include <string.h>
/***************************************
 * PubSubClient Documentation
 * https://pubsubclient.knolleary.net/
 **************************************/

// Mqtt Stuff

// ****************************** Cloud *********************************************
#define CLOUD_MQTT 0  //https://www.cloudmqtt.com/ (Amazon Web Services)
#define CLOUD_DIOTY 1 //http://www.dioty.co/       (colombian Server)
#define CLOUD_DANY 2  //At home Local network      (At home)
//==========Select cloud======================

#define CLOUD CLOUD_DANY

//============================================
#if CLOUD == CLOUD_DANY

IPAddress MqttServer(192, 168, 0, 20);           // MQTT server URL or IP
const unsigned int MqttPort = 1883;              // MQTT port (default 1883)
const char MqttUser[] = "itba.jacoby@gmail.com"; // user name
const char MqttPassword[] = "password";          // user password
const char MqttClientID[] = "aname";             // Device ID (warning: must be unique)

#elif CLOUD == CLOUD_DIOTY

const char MqttServer[] = "mqtt.dioty.co";
const unsigned int MqttPort = 1883;
const char MqttUser[] = "itba.jacoby@gmail.com";
const char MqttPassword[] = "password";
const char MqttClientID[] = "aname";

#elif CLOUD == CLOUD_MQTT

const char MqttServer[] = "m12.cloudmqtt.com";
const unsigned int MqttPort = 16111;
const char MqttUser[] = "user";
const char MqttPassword[] = "password";
const char MqttClientID[] = "aname";

#endif

// Create Cloud sockets
WiFiClient wclient;
PubSubClient mqtt_client(wclient);
//=====================================================================
// WiFi credentials

char ssid[] = "tomates y aceitunas";
char password[] = "POM26dulc"; // Set password to "" for open networks

//char ssid[] = "NODE_RED";
//char password[] = "GEDA2016";   // Set password to "" for open networks.
//=====================================================================

//Board defifinitions

//======================
#define HARD AMICA // Select Board Type or External pin
//======================

#define AMICA 0
#define LOLIN 1
#define EXTERN_PIN 2

// NODEMCU Boards & pin defs
#define AMICA_OnBoard_LED D0 // AMICA NodeMCU OnBoadrd LED  (on board blue led same as D0 but inverted)
#define LOLIN_OnBoard_LED D4 // LOLIN NodeMCU V3 OnBoadrd LED  (on board blue led same as D4 but inverted)
#define External_LED D1      // External led on D1

// **** Pin polarity ****
// Board
#define Board_LED_OFF 1
#define Board_LED_ON 0
// External
#define External_LED_OFF 0
#define External_LED_ON 1
// **********************//
// Select here the LED pin to be used: Board Build In led or External Pin//
#if HARD == AMICA
#define MYLED AMICA_OnBoard_LED // User LED
#define LED_ON Board_LED_ON
#define LED_OFF Board_LED_OFF

#elif HARD == LOLIN
#define MYLED LOLIN_OnBoard_LED // User LED
#define LED_ON Board_LED_ON
#define LED_OFF Board_LED_OFF

#elif HARD == EXTERN_PIN
#define MYLED External_LED // User LED
#define LED_ON External_LED_ON
#define LED_OFF External_LED_OFF
#endif

/////////////////////////////////////////////
#define OFF LOW
#define ON HIGH

#define MQTT_STS D6
#define STATUS_LED MYLED //Wifi - broker connection status indicator

// Status Led (blue)
// If it blinks fast: Unable to connect to Wifi network
// If it blinks every 2 seconds: Unable to find Broker
// If it Stays on device ready to operate

// *************************************************
//
// 				Print debug messages macro
//
// *************************************************
#define DEBUG_OFF 0
#define DEBUG_ON 1

#define debug DEBUG_OFF

#define debug_message(fmt, ...)          \
  do                                     \
  {                                      \
    if (debug)                           \
      Serial.printf(fmt, ##__VA_ARGS__); \
  } while (0)

//==================================================

void setup_wifi(void);
void setup_mqtt(void);

long TimeSinceLastMeasure = 0;
uint32_t Ldr_delayMS = 1000;

unsigned char ldr_enable = true;
unsigned char count_ldr = 0;

// the setup function runs once when you press reset or power the board
void setup()
{

  Serial.begin(9600, SERIAL_8N1); // Serial Port Parameters
  //Serial.swap();

  // Setup Digital Pins
  pinMode(STATUS_LED, OUTPUT); //Wifi & MQTT server Status led

  // Pins initial values

  digitalWrite(STATUS_LED, LED_OFF);

  // Setup Wifi and MQTT
  delay(1000);
  setup_wifi(); // initialize WIFI an connect to network
  setup_mqtt(); // initialize mqtt server
}

// the loop function runs over and over again forever
void loop()
{

  // Check MQTT conection is still active

  if (!mqtt_client.connected())
  {
    reconnect();
  }

  // This should be called regularly to allow the client
  // to process incoming messages and maintain
  // its connection to the server

  mqtt_client.loop();

  // mqtt_client.publish("light_box/ldrvalue",String(count_ldr++).c_str(),false);       //(Simulate ADC)

} // End of main Loop

//=======================SET UP WiFi==================================================================
void setup_wifi(void)
{

  // Start connecting to a WiFi network

  //debug_message("\n\nConnecting to %s\n",ssid);

  WiFi.softAPdisconnect(true); // Shut Down SoftAP
  WiFi.disconnect();           // If warm reset (not power down esp) ensure to signal router that old conection has finished
  WiFi.begin(ssid, password);  // Connect to WiFi network

  // Wait until WiFi conection is done
  while (WiFi.status() != WL_CONNECTED)
  {

    digitalWrite(STATUS_LED, LED_OFF);
    delay(250);
    debug_message(".");
    digitalWrite(STATUS_LED, LED_ON);
    delay(250);
  }

  digitalWrite(STATUS_LED, LED_OFF);

  //debug_message("\nWiFi Connected!!  IP Address:%s \n\n",WiFi.localIP().toString().c_str() );
}

//=======================SET UP MQTT==================================================================
void setup_mqtt(void)
{

  //http://pubsubclient.knolleary.net/api.html   (Arduino mqtt API)

  mqtt_client.setServer(MqttServer, MqttPort);
  mqtt_client.setCallback(callback);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = 0; // Set terminator
  ParseTopic(topic, payload, length);
}

//SEND RGB PACKAGE
/*void SendRGB(byte *payload)
{
  String a;
  int i = 5;
  while (((char *)payload)[i])
  {
    if(((char *)payload)[i] == ',' || ((char *)payload)[i] == ')'){
      Serial.write(a.toInt());
      a = "";
    }
    a+=((char *)payload)[i];
    i++;
  }
  
}*/
// ============================== MQTT PARSER ==================================================
void ParseTopic(char *topic, byte *payload, unsigned int length)
{
  /*if (!strcmp(topic, "bright"))
  {
   
   }*/
  if (!strcmp(topic, "bright"))
  {
    DynamicJsonDocument doc(1024);
    String data = String((char *)payload);
    char buff[100];
    data.toCharArray(buff,100);
    deserializeJson(doc, (const char*)buff);
    const uint8_t brightness = doc["bright"];
    Serial.write('B');
    Serial.write(brightness);
    Serial.write('D');
    Serial.write('D');
    }
  if (!strcmp(topic, "movement"))
  {
    //D: dont care
    if (!strcmp((char *)payload, "left"))
    {
      Serial.write('E');
      Serial.write('D');
      Serial.write('D');
      Serial.write('D');
    }
    if (!strcmp((char *)payload, "right"))
    {
      Serial.write('D');
      Serial.write('D');
      Serial.write('D');
      Serial.write('D');
    }
    //Serial.write("DDDD");
    if (!strcmp((char *)payload, "down"))
    {
      Serial.write('A');
      Serial.write('D');
      Serial.write('D');
      Serial.write('D');
    }
    //Serial.write("DDDA");
    if (!strcmp((char *)payload, "rotate"))
    {
      Serial.write('G');
      Serial.write('D');
      Serial.write('D');
      Serial.write('D');
    }
    //Serial.write("DDDG");
  }

  if (!strcmp(topic, "Piece-RGB"))
  {
    //Read JSON
   DynamicJsonDocument doc(1024);
   String data = String((char *)payload);
   char buff[100];
   data.toCharArray(buff,100);
   deserializeJson(doc, (const char*)buff);
   
   const char* piece = doc["piece"];
   const uint8_t r = doc["r"];
   const uint8_t g = doc["g"];
   const uint8_t b = doc["b"];
   Serial.write(piece);
   Serial.write(r);
   Serial.write(g);
   Serial.write(b);
   
  }
  if (!strcmp(topic, "reset")){
    Serial.write('R');
    Serial.write('R');
    Serial.write('R');
    Serial.write('R');
  }
}

void reconnect()
{

  while (!mqtt_client.connected()) // Loop until we're reconnected
  {
    //debug_message("Attempting MQTT connection...");

    // Try to connect to broker
    if (mqtt_client.connect(MqttClientID, MqttUser, MqttPassword))
    {
      //debug_message("Connected \r\n");
      digitalWrite(STATUS_LED, LED_ON);

      // ... and subscribe to topics

      mqtt_client.subscribe("movement");
      mqtt_client.subscribe("Piece-RGB");
      mqtt_client.subscribe("reset");
      mqtt_client.subscribe("bright");
      mqtt_client.subscribe("fall");
    }
    else
    {
      // If conection is not established just make a short blink

      //debug_message("failed, rc=");       //For rc codes see https://pubsubclient.knolleary.net/api.html#state
      //debug_message("%d",mqtt_client.state());
      //debug_message(" try again in 2 seconds \r\n");

      delay(2000); //Wait 2 seconds before retrying
      digitalWrite(STATUS_LED, LED_ON);
      delay(200); //Short 200 ms blink
      digitalWrite(STATUS_LED, LED_OFF);
    }

  } // end of while

} // End reconnect
