/*
*  Ronitor OS v 0.1 for ESP32-S2-Mini
*  Pin Configuration :
*  MOSI: 11
*  MISO: 9
*  SCK: 7
*  SS: 12
*  RS: 13
*  
#define DHTPIN      38           // DHT11 data pin is connected to NodeMCU pin D1 (GPIO5)
#define BUZZERPIN   18
#define DISPLAY     4
#define BUTTON      8
*/

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

String chipId = String(ESP.getEfuseMac(),HEX);
String clientId = "Ronitor-"+ (String) chipId;

#include "display.h"
#include "OTA.cpp"

unsigned long lastMsg = 0; // For 10 Sec counter from temperature sensor
unsigned long memory_check;

extern TFT_eSPI tft;
 
int PWM1_DutyCycle = 255;

WiFiClient espClient;
PubSubClient MQTTclient(espClient);
WiFiClientSecure wificlient;
extern TFT_eSPI tft ;

DHT dht(DHTPIN,DHTTYPE);  
extern NTPClient timeClient;
extern unsigned long unix_epoch;
extern unsigned long lastMsg;
extern float temperature;
extern float humidity;
int screen=0;
extern char second_line_text[6][20];
extern unsigned long color_settings[16];

extern String sSubs;
extern String sViews;
extern String sVideos;
extern String channelName;
extern int Icon;
extern char condition[6];
extern int moon_icon ;
extern int moon_illumination; 


const char* mqtt_server = "";
const char* mqtt_client = "iQub3App";
const char* mqtt_user = "iQub3App";
const char* mqtt_pass = "123456";
const int mqtt_port = 1883;


char* subscrib_topic_1; 
char* subscrib_topic_2; 
char* publish_topic_1; 

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

/*
*       Button Settings
*
*/

byte buttonState = false;
byte printFlag = false;

// Variables will change:
int lastState = HIGH; // the previous state from the input pin
int currentState;  


void callback(char* topic, byte* payload, unsigned int length) {
  char str[length+1];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i;
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str[i]=(char)payload[i];
  }
  str[i] = 0;
  Serial.println();

  //String subscrip_topics_1 = chipId+"/weather";

  if(strcmp(topic,subscrib_topic_1)==0){
    Serial.println("Data Received");
    Serial.println(str);
    
    StaticJsonDocument <256> doc;
    deserializeJson(doc,payload);
    
    float t = doc["t"];
    float f = doc["f"];
    float m = doc["m"];
    float M = doc["M"];
    float H = doc["H"];
    Icon =(int) doc["i"];
    const char *w = doc["w"];
    const char *W = doc["W"];
    const int moon = doc["moon"];
    const int Moon = doc["Mi"];

    moon_icon = moon ;
    moon_illumination = Moon;
    
    sprintf(second_line_text[0], "Temp %.0f°C",round(t));
    sprintf(second_line_text[1], "Feels Like %.0f°C",round(f));
    sprintf(second_line_text[2], "Min Temp %.0f°C",round(m));
    sprintf(second_line_text[3], "Max Temp %.0f°C",round(M));
    sprintf(second_line_text[4], "%s",W);
    sprintf(second_line_text[5], "Humidity %.0f%%",round(H));
    //sprintf(Icon, "%d",icon);
    if(strlen(w)>5)
      sprintf(condition,"%.5",w);
    else
      sprintf(condition,"%s",w);
    }

    if(strcmp(topic,subscrib_topic_2)==0){
    Serial.println("Data Received");

    Serial.println(str);
    
    StaticJsonDocument <256> doc;
    deserializeJson(doc,payload);

    for(int k=0;k<15;k++){
      char index[3];
      sprintf(index, "%d", k);

      unsigned long color_code = doc[index];
      
      if(color_code>0)
      color_settings[k] = color_code;
      }
    
    Serial.println("Color Setting received");
    for(int k=0;k<15;k++){
    Serial.print("Color #");
    Serial.println(k);
    Serial.println(color_settings[k]);
    }
    //setColors();
    RTC_display();
    }
}

void reconnect() {
  // Loop until we're reconnected
  while (!MQTTclient.connected()) {
    Serial.print("Attempting MQTT connection...");
   
    // Attempt to connect
    if (MQTTclient.connect(mqtt_client,mqtt_user,mqtt_pass)) {
    //if (MQTTclient.connect(mqtt_client)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      MQTTclient.publish("chip/id", chipId.c_str(),2);
      // ... and resubscribe
      //sprintf(subscrib_topic_1, "%c/weather",ChipID);
      //sprintf(subscrib_topic_2, "%c/settings",ChipID);

      subscrib_topic_1 = (char*)malloc(strlen(chipId.c_str()) + strlen("/weather") + 1); // Allocate memory for the concatenated string
      strcpy(subscrib_topic_1, chipId.c_str()); // Copy the chipId string to subscrib_topic_1
      strcat(subscrib_topic_1, "/weather"); 

      subscrib_topic_2 = (char*)malloc(strlen(chipId.c_str()) + strlen("/settings/clock") + 1); // Allocate memory for the concatenated string
      strcpy(subscrib_topic_2, chipId.c_str()); // Copy the chipId string to subscrib_topic_1
      strcat(subscrib_topic_2, "/settings/clock"); 
      
      //subscrib_topic_1 = "d70dfa/weather";
      //subscrib_topic_2 = "d70dfa/settings/clock";
      MQTTclient.subscribe(subscrib_topic_1);
      MQTTclient.subscribe(subscrib_topic_2);
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
    Serial.begin(115200);
    pinMode(BUZZERPIN, OUTPUT);
    pinMode(DISPLAY, OUTPUT);

    ledcAttachPin(DISPLAY, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
    PlaySound();
    tft.init();
    tft.fillRect(0, 0, 240, 240, TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);     // set text color to yellow and black background
    tft.setRotation(2);
    ledcWrite(PWM1_Ch, PWM1_DutyCycle);

    // Initialise LittleFS
      if (!LittleFS.begin()) {
        Serial.println("LittleFS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
      }
      Serial.println("\r\nInitialisation done.");

      // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
      TJpgDec.setJpgScale(1);

      // The decoder must be given the exact name of the rendering function above
      TJpgDec.setCallback(tft_output);
      
      tft.setCursor(0, 10);              // move cursor to position (4, 27) pixel
      tft.loadFont(SMALL_FONT, LittleFS);
      DontWorry();
    
    //tft.loadFont(SMALL_FONT);
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect(clientId.c_str()); // password protected ap
  
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

//  AsyncWiFiManager wifiManager(&server);
  Serial.println(chipId);
  wificlient.setInsecure();
     
  tft.fillScreen(TFT_BLACK);
  timeClient.begin();
  dht.begin();

  //strcpy(ChipID, chipId.c_str());
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback(callback); 
  
  MQTTclient.publish("chip/id", chipId.c_str(),2);

  RTC_display();
  setupOTA();
}

void loop() {
    // put your main code here, to run repeatedly:   

buttonState = digitalRead(BUTTON);
  if(buttonState == LOW && printFlag == false) 
  {
    //Serial.println("button is pressed");
    printFlag = true;
    delay(200);
  }
  else
  {
    printFlag = false;
  }

  timeClient.update();
  unix_epoch = timeClient.getEpochTime();   // get UNIX Epoch time

  if (!MQTTclient.connected()) {
    reconnect();
  }
  MQTTclient.loop();

  unsigned long now = millis();
  
  if (now - lastMsg > 10*1000) {
    lastMsg = now;
    //tft.fillScreen(TFT_BLACK);
    //humidity = dht.readHumidity();
    temperature = dht.readTemperature();
   
    snprintf (msg, MSG_BUFFER_SIZE, "Temperature: %.2f", temperature);
    //free(temperature);
    Serial.print("Publish message: ");
    Serial.println(msg);
    //sprintf(publish_topic_1, "%c/temp",ChipID);
    //publish_topic_1 = "d70dfa/temp";
    
    publish_topic_1 = (char*)malloc(strlen(chipId.c_str()) + strlen("/temp") + 1); // Allocate memory for the concatenated string
    strcpy(publish_topic_1, chipId.c_str()); // Copy the chipId string to subscrib_topic_1
    strcat(publish_topic_1, "/temp"); 
    
    MQTTclient.publish(publish_topic_1, msg);

    free(msg);
    free(publish_topic_1);
    //ledcWrite(PWM1_Ch, PWM1_DutyCycle);
     
  }
 
    RTC_display();
    delay(20);
    
  unsigned long now2 = millis();
  long int mem_in_kb;
  if (now2 - memory_check > 60*1000) {
    memory_check = now2;
    long  fh = ESP.getFreeHeap();
    Serial.print("Free Memory: ");
    mem_in_kb=fh/1000;
    Serial.print(mem_in_kb);
    Serial.println(" KB");
 } 
   handleOTA();

}
