/*
 * File: proximity_prototype
 * Author: B Puc
 * Date: Oct 5, 2015
 * 
 * Testing the HC-SR04 ultrasonic proximity sensor
 * https://docs.google.com/document/d/1Y-yZnNhMYy7rwhAgyL_pfa39RsB-x2qR4vP8saG73rE/edit
 */
#include <hcsr04.h>

//Digital IO channels
#define ECHO_SIG    7
#define TRIGGER     6
#define LED_RED     5
#define LED_YELLOW  4
#define LED_GREEN   3

#define DEBUG 1

unsigned long end_time;
unsigned long start_time;
unsigned long pulsewidth;

float range;

//Initialize the range sensor
hcsr04 range_sensor(TRIGGER, ECHO_SIG);

void setup() {
  // put your setup code here, to run once:
  if (DEBUG == 1) Serial.begin(57600);
}

void loop() {
  // Mark a timer to start loop
  start_time = millis();
  
  // query proximity sensor for current range
  range = range_sensor.getrange();

  // LED OUTPUT
  if (range < 50){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
  } else if (range < 200) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
  
  // Mark a time to stop loop
  end_time = millis();
  
  if (DEBUG == 1) {
    Serial.print(float(millis()/1000.0));  //System run time in seconds
    Serial.print(" msec = ");
    Serial.print(end_time-start_time);  //Loop time in milliseconds
    Serial.print(" range = ");          //Distance in centimeters
    Serial.print(range);
    Serial.println("");
  }
  
  // Minimum of 60 mS cycle time to eliminate pulsing into the echo signal
  // Empirically, infinite range takes 193-197 msec.  Enforce a constant 200msec loop time.
  delay(abs(200 - (end_time - start_time)));
}
