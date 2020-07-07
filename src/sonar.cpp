#include "sonar.hpp"

Sonar::Sonar(int trig, int echo) : trig(trig), echo(echo) {
    init();
}

void Sonar::init() {
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trig, LOW);
}

double Sonar::distance(int timeout) {
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    now=micros();

    while (digitalRead(echo) == LOW && micros()-now<timeout);
        recordPulseLength();

    travelTimeUsec = endTimeUsec - startTimeUsec;
    distanceMeters = 100*((travelTimeUsec/1000000.0)*340.29)/2;

    return distanceMeters;
}

void Sonar::recordPulseLength() {
    startTimeUsec = micros();
    while ( digitalRead(echo) == HIGH );
    endTimeUsec = micros();
}
