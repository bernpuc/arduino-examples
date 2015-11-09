/*
 * File: proximity_prototype
 * Author: B Puc
 * Date: Nov 9, 2015
 * 
 * Testing the HC-SR04 ultrasonic proximity sensor
 * https://docs.google.com/document/d/1Y-yZnNhMYy7rwhAgyL_pfa39RsB-x2qR4vP8saG73rE/edit
 * Using the NewPing library to control the SR04.
 */
#include <NewPing.h>
#include <Servo.h>
void update_led(int);

#define SERVO 9
#define PING_PIN 12
#define LED_RED 5
#define LED_YELLOW 4
#define LED_GREEN 3

#define DEBUG 0
#define DO_SERVO 1
#define POS_1 -65
#define POS_2 65

/*
 * Tuning the loop time for optimum range reading accuracy.
 * - A delay after setting the servo position seems to increase range repeatability.  
 * - Averaging several readings also improves repeatability by filtering variants at the expense of longer loop times.
 * - 
 */
#define LOOP_MIN 500
#define RANGE_NUM 1
#define SERVO_DELAY 60

/*
 * Loop timing tracking variables
 */
unsigned long end_time;
unsigned long start_time;
unsigned long time_now;
unsigned int loop_time;
unsigned int days;
unsigned int hours;
unsigned int minutes;
unsigned int seconds;

/*
 * Range tracking variables
 */
unsigned int reading;

/*
 * Servo position control
 */
int end_pos;
int pos;
int inc = 10;

//Initialize the range sensor
#define MAX_DISTANCE 150  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.

//Initialize the servor controller
Servo myservo;

void setup() {
  if (DO_SERVO) {
    myservo.attach(SERVO);
    pos = (POS_1 + 90);
    end_pos = (POS_2 + 90);
    myservo.write(pos);
  }
  Serial.begin(115200);
}

void loop() {
  // Record start loop time
  start_time = millis();
  
  //Set servo position
  if (DO_SERVO) {
    myservo.write(pos);
    delay(SERVO_DELAY);
  }

  //Scan range by reading sensor
  reading = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  reading /= US_ROUNDTRIP_CM; // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  Serial.print("Ping: ");
  Serial.print(reading); 
  Serial.println("cm");
  
  // LED OUTPUT: indicate range by lighting appropriate LED
  update_led(reading);

  // Record stop loop time
  end_time = millis();
  loop_time = end_time - start_time;

  // Debugging print to serial output.
  if (DEBUG) {
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
    Serial.print(loop_time);  //Loop time in milliseconds
    Serial.print(" range = ");
    Serial.print(reading);                //Distance in centimeters
    Serial.println(" cm");
    //Serial.print(range/100);            //Distance in meters
    //Serial.println(" m");
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

  // Enforce a constant LOOP_MIN msec loop time.
  if (loop_time < int(LOOP_MIN)) {
    delay(LOOP_MIN - loop_time);
    if (DEBUG) {
      Serial.print("delay ");
      Serial.println(LOOP_MIN-loop_time);
    }
  }

}

void update_led(int _reading){
  if (_reading > 0 && _reading < 50){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    if (DEBUG) Serial.println("RED");
  } else if (_reading > 0 && _reading < 100) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
    if (DEBUG) Serial.println("YELLOW");
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
    if (DEBUG) Serial.println("GREEN");
  }
}
