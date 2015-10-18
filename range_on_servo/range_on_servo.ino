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
#define DO_SERVO 0
#define POS_1 -65
#define POS_2 65

/*
 * Tuning the loop time for optimum range reading accuracy.
 * - A delay after setting the servo position seems to increase range repeatability.  
 * - Averaging several readings also improves repeatability by filtering variants at the expense of longer loop times.
 * - 
 */
#define LOOP_DELAY 0
#define RANGE_NUM 5
#define SERVO_DELAY 120

/*
 * Loop timing tracking variables
 */
unsigned long end_time;
unsigned long start_time;
int loop_time;
unsigned int time_now;
unsigned int days;
unsigned int hours;
unsigned int minutes;
unsigned int seconds;

/*
 * Range tracking variables
 */
float range;
float reading;
float range_sum;

/*
 * Servo position control
 */
int end_pos;
int pos;
int inc = 10;

//Initialize the range sensor
hcsr04 range_sensor(TRIGGER, ECHO_SIG);

//Initialize the servor controller
Servo myservo;

void setup() {
  if (DO_SERVO) {
    myservo.attach(SERVO);
    pos = (POS_1 + 90);
    end_pos = (POS_2 + 90);
    myservo.write(pos);
  }
  Serial.begin(57600);
}

void loop() {
  // Record start loop time
  start_time = millis();
  
  //Set servo position
  if (DO_SERVO) {
    myservo.write(pos);
  }
  delay(SERVO_DELAY);

  //Scan range by reading sensor
  //range = range_sensor.getrange();
  range_sum = 0;
  for (int i=0; i < RANGE_NUM; i++){
    reading = range_sensor.getrange();
    range_sum += reading;
    delay(60); // Minimum of 60 mS cycle time to eliminate pulsing into the echo signal
    Serial.print(reading);
    Serial.print(" ");
  }
  Serial.println();
  range = range_sum/RANGE_NUM;
  
  // LED OUTPUT
  if (range < 50){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
  } else if (range < 100) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
  // Record stop loop time
  end_time = millis();
  loop_time = end_time - start_time;
  
  if (DEBUG == 1) {
    //Serial.print(millis()/1000);         //System run time in seconds
    time_now = millis()/1000;
    days = time_now / 86400;
    time_now = time_now % 86400;
    hours = time_now / 3600;
    time_now = time_now % 3600;
    minutes = time_now / 60;
    seconds = time_now % 60;
    Serial.print(days);
    Serial.print(" days ");
    Serial.print(hours);
    Serial.print(" hours ");
    Serial.print(minutes);
    Serial.print(" minutes ");
    Serial.print(seconds);
    Serial.println(" seconds");
    Serial.print(" looptime = ");
    Serial.print(end_time-start_time);  //Loop time in milliseconds
    Serial.print(" range = ");
    //Serial.print(range);                //Distance in centimeters
    //Serial.println(" cm");
    Serial.print(range/100);            //Distance in meters
    Serial.println(" m");
  }

  // Calculate next position
  if (DEBUG) Serial.println(pos);
  if (abs(end_pos-pos) <= 5){
    inc = -inc;
    // if end_pos is POS_2 + 90 then new end_pos is POS_1 + 90
    if (end_pos == POS_2 + 90) {
      end_pos = POS_1 + 90;
    } else {
      end_pos = POS_2 + 90;
    }
  }
  pos += inc;

  // Empirically, infinite range takes 193-197 msec.  Enforce a constant 200msec loop time.
  if (loop_time < LOOP_DELAY) {
    delay(LOOP_DELAY - loop_time);
    if (DEBUG) {
      Serial.print("delay ");
      Serial.println(LOOP_DELAY-loop_time);
    }
  }

}
