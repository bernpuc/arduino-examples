/*
	hcsr04.h - Library for controlling ultrasonice proximity sensor HC-SR04 - implementation
*/

// include this library's description file
#include "hcsr04.h"

//Constructor
hcsr04::hcsr04(int chan_trig, int chan_signal)
{
	// initialize this instance's values
    TRIGGER = chan_trig;
    ECHO_SIG = chan_signal;
    
    // setup IO
	pinMode(TRIGGER, OUTPUT);
	pinMode(ECHO_SIG, INPUT);
	digitalWrite(TRIGGER, LOW);
}

//Public Methods

// Return range in centimeters
// Formula from http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/step3/Upload-the-sketch/
float hcsr04::getrange(void){
    
	send_trigger();
	pulsewidth = pulseIn(ECHO_SIG, HIGH);
	range = (pulsewidth/2) / 29.1;
	return range;
}

//Private Methods

// Send trigger signal (10 usec pulse)
void hcsr04::send_trigger(void){

    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(1);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);

}
