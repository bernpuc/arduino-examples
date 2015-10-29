/*
	hcsr04.h - Library for controlling ultrasonice proximity sensor HC-SR04 - description
*/
#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef hcsr04_h
#define hcsr04_h

// Library interface description
class hcsr04
{
	public:
		hcsr04(int, int);
		float getrange(void);
        
    private:
    	int TRIGGER;
        int ECHO_SIG;
        unsigned long pulsewidth;
        float range;
    	void send_trigger(void);
};
 
#endif
