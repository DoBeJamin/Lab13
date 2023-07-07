/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/benja/OneDrive/Desktop/visualStudioCode/summerIOT/Lab13/src/Lab13.ino"
#include "MQTT.h"
#include "oled-wing-adafruit.h"

void setup();
void loop();
void format_display ();
void timer_end ();
#line 4 "c:/Users/benja/OneDrive/Desktop/visualStudioCode/summerIOT/Lab13/src/Lab13.ino"
SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);
MQTT client("lab.thewcl.com", 1883, callback);

double latitude = 0;
double longitude = 0;

double value;
String s;

//used to check the topic of mqtt callback
String theTopic = "benlab13/latitude";

bool timer_active = false;

Timer timer(2000, timer_end);

OledWingAdafruit display;


void setup() {

  display.setup();

  client.subscribe("benlab13/latitude");
  client.subscribe("benlab13/longitude");
}

void loop() {

  display.loop();

  if (client.isConnected())
  {
    client.loop();
    if (!timer_active) {
      //sends a message to node red to do an api request every 2 seconds
      timer.start();
      timer_active = true;
      client.publish("benLab13/sendrequest", "0");
    }

  }
  else {
    //subscribes and connects if not connected
    client.connect(System.deviceID());
    client.subscribe("benlab13/latitude");
    client.subscribe("benlab13/longitude");
  }

  format_display();

  display.println("latitude Longitude");

  display.print(latitude);
  display.print(" : ");
  display.print(longitude);
  display.display();

}

void callback(char *topic, byte *payload, unsigned int length) {

  //formats and extracts data from mqtt 
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  s = p;
  value = s.toFloat();

  String callbackTopic = topic; //makes the mqtt topic of the data into an arduino string

  if (theTopic.compareTo(callbackTopic)) {
    latitude = value;
  } else {
    longitude = value;
  }

}

void format_display () {
  //sets up basic formatting for when using the display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE); 
  display.setCursor(0,0);
}

void timer_end () {
  timer_active = false;
}