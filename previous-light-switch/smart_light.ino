
/*
 * -this is basically the same as the smart door lock. in the future, im going to combine them into 1 program
 * -there has been  a basic web app made with "MIT AI2 App Inventor"
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <EEPROM.h>         //writes either a 1 or 0 to memory, so it can remember what state to be in if it turns off

Servo motor;
int led = 2;
int button = 5;

const char* ssid = "XXXXXXXX";    //enter the WiFi SSID and password 
const char* password = "XXXXXXXX";

int addr = 0;                  //address where state information will be help
int data;                      //variable for the state information

ESP8266WebServer server(80); //Declaring a global object variable from the ESP8266WebServer on port 80

//---------------------------------------------------------------HTML SOURCE CODE
const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">

  <title>Calum's Smart House</title>
  <meta name="description" content="Smart House">
  <meta name="author" content="Calum Evans">
  <link rel="stylesheet" href="css/styles.css?v=1.0">
</head>
<body>
  <script src="js/scripts.js"></script>  
  <center>
  <h1>CALUM'S AMAZING SMART HOUSE!!</h1><br>
  Click to turn on the light <a href="M7r1zUeZcQrlz44RxycZujE3Zi24dBipPqYEDBHacXbxQU9LvEMu9KoFMJvBCf">LIGHT ON</a><br>
  Click to turn off the light <a href="sQICgRJK1lEyPX7plaHjJ5fo5Jz6c2TbWIP4nPORkh8oK5mnrnOC63rEipxfV9">LIGHT OFF</a><br>
  <hr>
  </center>  
</body>
</html>
)=====";
 
void handleRoot(){                       //when the ESP8266's IP address is opened
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleLIGHTON(){                        //when the on button is pressed
 lightOn();
 server.send(200, "text/html", "LIGHT ON");
}
 
void handleLIGHTOFF(){                     //when the off button is pressed
 lightOff();
 server.send(200, "text/html", "LIGHT OFF");
}

void lightOff(){                          
  motor.write(170);
  motor.write(166);
  delay(500);
  EEPROM.put(addr,0);
  EEPROM.commit();
}

void lightOn(){                          
  motor.write(2);
  delay(500);
  EEPROM.put(addr,1);
  EEPROM.commit();
}

void toggle(){
  int state;
  EEPROM.get(addr,state);
  if(state){
    lightOff();
  }else{
    lightOn();
  }
}

void buttonCheck(){
  if(!digitalRead(button)){
    toggle();
  }else{
    return;
  }
}


//---------------------------------------------------------------SETUP
void setup(){
  EEPROM.begin(512);
  motor.attach(4);                // Connects the servo
  pinMode(button, INPUT);
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  EEPROM.get(addr,data);          //reading what the current state is
  if(data){
    lightOn();
  }else{
    lightOff();
  }
  
  while (WiFi.status() != WL_CONNECTED) {     // Wait for connection
    delay(500);
  }
 
  server.on("/", handleRoot);      //these are the dirrent webpages (buttons)
  server.on("/M7r1zUeZcQrlz44RxycZujE3Zi24dBipPqYEDBHacXbxQU9LvEMu9KoFMJvBCf", handleLIGHTON);
  server.on("/sQICgRJK1lEyPX7plaHjJ5fo5Jz6c2TbWIP4nPORkh8oK5mnrnOC63rEipxfV9", handleLIGHTOFF);
 
  server.begin();                  //Start server
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}

//---------------------------------------------------------------LOOP
void loop(){
  server.handleClient();          //Handle client requests
  buttonCheck();
}
