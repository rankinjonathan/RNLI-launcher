#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
//-------------------------- Configuration --------------------------


// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"



//-------------------------- Definitions --------------------------



// the Adafruit IO username of whomever owns the feed
#define FEED_OWNER "Rankinjonathan"

// set up the `sharedFeed`
AdafruitIO_Feed *sharedFeed = io.feed("rnli-launcher", FEED_OWNER);

//
//const int buttonPin = 10;     // the number of the pushbutton pin
const int ledPin = 13;       // the pin that the LED is attached to


// Variables will change:

unsigned long timer;
unsigned long timerOffset;

unsigned long currentTime;
unsigned long startTime;

unsigned long contractLength = 600000;
unsigned long timeLeft;

String inputString = "";         // a String to hold incoming data
String lastSerialVal = "";      //Put the serial value in from 0s to 5min
String currentSerialVal = "";   //Put the current serial value in this variable

String tweet = "";


boolean stringComplete = false;  // whether the string is complete

int happened = 0;
int X = 11;

U8X8_SSD1322_NHD_256X64_4W_SW_SPI u8x8(/* clock=*/ 24, /* data=*/ 23, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 5);


//-------------------------- Setup --------------------------



void setup() {

  Serial.begin(9600);        // Start reading from Serial communication interface

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.println("IO test");




  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);

  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8, 7, 4, 2);

  // initialize serial communication:

  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  //  Serial.println("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'sharedFeed' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  sharedFeed->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    //    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  sharedFeed->get();

  // setting the start time and print it
  timer = 0;
  Serial.print("Start time = ");
  Serial.println(timer);

  startTime = millis();


  Serial.print("Finished the setup");
  Serial.println("\n");

}




//-------------------------- Loop --------------------------




void loop() {

  io.run();

  //  tweet = "Jonathan";
  //  Serial.print("TEST POINT 1");
  //  Serial.println("\n");
  //  u8x8.clear();
  //  u8x8.setCursor(0, 0);
  //  u8x8.print(tweet);
  //
  //  delay(10000);
  //  tweet = "Clare";
  //  Serial.print("TEST POINT 2");
  //  Serial.println("\n");
  //  u8x8.clear();
  //  u8x8.setCursor(0, 0);
  //  u8x8.print(tweet);
  //  delay(10000);

  currentTime = millis();

  timer = currentTime - startTime;

  Serial.print("Time Left in Contract: ");
  Serial.print((contractLength - (currentTime - startTime)) / 1000);
  Serial.println(" seconds");

  //-------------------------- If there is a timer runing and an event happened  --------------------------

  if (X > 0 && timer < contractLength && happened == 1) {
    Serial.println("Donate £1");
    Serial.print("counter =    ");
    Serial.println(X);
    //-------------------------- Do this section if there is a RNLI launch  --------------------------

    Serial.print("TEST POINT 3 -- ");
    Serial.println(tweet);

    u8x8.clear();
    u8x8.setCursor(0, 0);
    u8x8.print(tweet);

    //-------------------------- Reset trigger and take 1 off the count  --------------------------

    happened = 0;
    X = X - 1;


  }


  else if (X == 0 && timer < contractLength) {
    Serial.println("All money has been donated");;
    timeLeft = contractLength - timer;
    delay(timeLeft);
    happened = 0;

  }

  else if (timer > contractLength) {
    Serial.println("Contract finished - restarting contract");
    happened = 0;
  }

  delay(1000);

}



void handleMessage(AdafruitIO_Data * data) {

  tweet = data->toString();

  happened = 1;



}
