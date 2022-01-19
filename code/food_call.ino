#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#include <HTTPClient.h>
#include <WiFi.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define INVITE_IN 1
#define ACCEPTED_IN 2
#define DECLINED_IN 3
#define DISREGARDED 4
#define INVITE_OUT 5
#define ACCEPTED_OUT 6
#define DECLINED_OUT 7

#define PERIOD 1*20*1000L // Check every 20 sec for invite
unsigned long target_time = 0L ;

int howLongToWait = 5 * 60 * 1000; // wait 5 minuten for response

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Wifi setup
const char* ssid = "type Wifi name here";
const char* password =  "type password here";
String url = "paste firebase url here";  
HTTPClient http;

// Declare pin numbers
int inputPin1 = 4;
int inputPin2 = 0;
int mp3OutPin = 17;
int mp3InPin = 16;

int input1 = HIGH;
int input2 = HIGH;

int invite = 0;

// Speaker setup
HardwareSerial myHardwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;

// Function Declerations
int getRequest();
int putRequest(int invite_new);
void outputDisplay(String message);

void setup() {
  Serial.begin(115200); // For communication with laptop
  
  // Setup display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  outputDisplay("");
  
  // Set up pins
  pinMode(inputPin1, INPUT_PULLUP);
  pinMode(inputPin2, INPUT_PULLUP);

  // Set up wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  http.setTimeout(1000);

  // Set up speaker
  myHardwareSerial.begin(9600, SERIAL_8N1, 16, 17);
  // Code to establish connection to mp3
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(myHardwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}

void loop() {
  // put your main code here, to run repeatedly:

  // Check if wants to send invitation
  input1 = digitalRead(inputPin1);
  input2 = digitalRead(inputPin2);

  if (input1 == LOW || input2 == LOW) {
    
    invite = getRequest();

    if (invite == 0) {
      invite = putRequest(1);
      myDFPlayer.play(INVITE_OUT);
      outputDisplay("Einladung gesendet");
      unsigned long startedWaiting = millis();
      while(getRequest() == 1 && millis() - startedWaiting <= howLongToWait)
      {
         // Check every 10 sec for an answer
         delay(10000);
      }
      invite = getRequest();
      switch (invite) {
        case 1:
          myDFPlayer.play(DISREGARDED);
          outputDisplay("Einladung abgelaufen");
          break;
        case 2:
          myDFPlayer.play(ACCEPTED_IN);
          outputDisplay("Einladung angenommen");
          break;
        case 3:
          myDFPlayer.play(DECLINED_IN);
          outputDisplay("Einladung abgelehnt");
          break;
        default:
          break;
      }

      delay(60000);

      // reset to inital state
      invite = putRequest(0);
      outputDisplay("");
      
    } else{
      invite = putRequest(0);
      outputDisplay("Einladung zeitgleich");
      delay(30000);
      outputDisplay("");
    }
  }

  // Check if invitation is there every PERIOD 
  if (millis () - target_time >= PERIOD)
  {
    target_time += PERIOD ;   // change scheduled time exactly, no slippage will happen

    invite = getRequest();

    if(invite == 1){
      myDFPlayer.play(INVITE_IN);
      outputDisplay("Einladung bekommen");

      // Listen for button response
      unsigned long startedWaiting = millis();
      while(input1 == HIGH && input2 == HIGH && millis() - startedWaiting <= howLongToWait)
      {
        input1 = digitalRead(inputPin1);
        input2 = digitalRead(inputPin2);
        delay(300);
      }

      if(input1 == LOW && input2 == HIGH){
        invite = putRequest(2);
        myDFPlayer.play(ACCEPTED_OUT);
        outputDisplay("Einladung angenommen");
      } else if(input1 == HIGH && input2 == LOW){
        invite = putRequest(3);
        myDFPlayer.play(DECLINED_OUT);
        outputDisplay("Einladung abgelehnt");
      } else{
        invite = putRequest(0);
        myDFPlayer.play(DISREGARDED);
        outputDisplay("Einladung abgelaufen");
      }

      delay(60000);
      // reset to inital state
      outputDisplay("");
      
    }
  }
  

  delay(300);
}


int getRequest() {
  // Make https get request here
  Serial.println("Making GET request");
  http.begin(url);
  // Issue the HTTP GET request.
  int status = http.GET();
  if (status <= 0) {
    Serial.printf("HTTP error: %s\n", 
        http.errorToString(status).c_str());
    return -1;
  }
  // Read the response.
  String payload = http.getString();
  http.end();
  return payload.toInt();
}

int putRequest(int invite_new) {
  // Make https put request here
  Serial.println("Making PUT request");
  http.begin(url);
  // Issue the HTTP PUT request.
  http.addHeader("Content-Type", "text/plain");
  int status = http.PUT(String(invite_new));
  if (status <= 0) {
    Serial.printf("HTTP error: %s\n", 
        http.errorToString(status).c_str());
    return -1;
  }
  // Read the response.
  String payload = http.getString();
  http.end();
  return payload.toInt();
}

void outputDisplay(String message){
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(message);
  display.display(); 
}
