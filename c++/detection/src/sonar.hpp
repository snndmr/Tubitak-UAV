#ifndef SONAR_HPP
#define SONAR_HPP

#include <wiringPi.h>

class Sonar {
   private:
    const int PIN_ECHO = 0;
    const int PIN_TRIG = 1;

    int trig;
    int echo;
    long now;
    long travelTimeUsec;
    double distanceMeters;
    volatile long endTimeUsec;
    volatile long startTimeUsec;

    void init();
    void recordPulseLength();

   public:
    Sonar(int, int);
    double distance(int);
};

#endif
