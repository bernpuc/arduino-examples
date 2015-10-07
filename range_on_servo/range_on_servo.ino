/*
 * File: proximity_prototype
 * Author: B Puc
 * Date: Oct 5, 2015
 * 
 * Testing the HC-SR04 ultrasonic proximity sensor
 * https://docs.google.com/document/d/1Y-yZnNhMYy7rwhAgyL_pfa39RsB-x2qR4vP8saG73rE/edit
 */
#include <hcsr04.h>
#include <Servo.h>

#define SERVO 9
#define ECHO_SIG 7
#define TRIGGER 6
#define LED_RED 5
#define LED_YELLOW 4
#define LED_GREEN 3

#define DEBUG 1
#define POS_1 -45
#define POS_2 45

unsigned long end_time;
unsigned long start_time;
unsigned long pulsewidth;

float range;

int start_pos;
int end_pos;
int pos;
int inc = 10;
int current_pos;

//Initialize the range sensor
hcsr04 range_sensor(TRIGGER, ECHO_SIG);

//Initialize the servor controller
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(SERVO);
  start_pos = (POS_1 + 90);
  end_pos = (POS_2 + 90);
  pos = start_pos;
  myservo.write(start_pos);
  Serial.begin(57600);
}

void loop() {
  // Mark a timer to start loop
  start_time = millis();
  
  //Set servo position
  myservo.write(pos);
  current_pos = pos;
  
  //Scan range by reading sensor
  range = range_sensor.getrange();
  delay(250);
  
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
    Serial.print(millis()/1000);  //System run time in seconds
    Serial.print(" msec = ");
    Serial.print(end_time-start_time);  //Loop time in milliseconds
    Serial.print(" range = ");          //Distance in centimeters
    Serial.print(range);
    Serial.println("");
  }

  // Calculate next position
  Serial.print(start_pos);
  Serial.print(" ");
  Serial.print(current_pos);
  Serial.print(" ");
  Serial.println(end_pos);
  if (abs(end_pos-current_pos) <= 5){
    inc = -inc;
    start_pos = current_pos;
    // if end_pos is POS_2 + 90 then new end_pos is POS_1 + 90
    if (end_pos == POS_2 + 90) {
      end_pos = POS_1 + 90;
    } else {
      end_pos = POS_2 + 90;
    }
  }
  pos += inc;
  delay(1000);
}
