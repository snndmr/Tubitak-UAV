#include <wiringPi.h>
#include <stdio.h>    
#include <softPwm.h> 

const int PIN_PWM = 1;

int main(){		
	if (wiringPiSetup() == -1)
    	return EXIT_FAILURE;
    		
    int intensity = 0;		
	pinMode(PIN_PWM, OUTPUT);	
	softPwmCreate(PIN_PWM, 1, 100);
	
	while (1)
	  {
		for (intensity = 0; intensity < 101; intensity++)
		{
		  softPwmWrite (PIN_PWM, intensity);
		  delay (20) ;
		}
		delay(1);

		for (intensity = 100; intensity >= 0; intensity--)
		{
		  softPwmWrite (PIN_PWM, intensity);
		  delay (20);
		}
		delay(1);
	}
}
