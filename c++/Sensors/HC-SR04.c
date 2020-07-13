#include <iostream>
#include <wiringPi.h>

using namespace std;

const int PIN_ECHO = 0;
const int PIN_TRIG = 1;

void init(){
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIN_TRIG, OUTPUT);
    digitalWrite(PIN_TRIG, LOW);
}

double distance(){
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    volatile long beg = micros();
    volatile long end = micros();
    
    while(digitalRead(PIN_ECHO) == LOW) {
        beg = micros();
    }
    
    while(digitalRead(PIN_ECHO) == HIGH) {
        end = micros();
    }
    
    return (((end - beg) / 1000000.0) * 340.29) / 2; // micro to sec 10-e6
}

int main() {
    if (wiringPiSetup() == -1)
    	return EXIT_FAILURE;

    init();

    while (1) {
        cout << "Distance is " << distance() << " m." << endl;
        delay(10);
    }
    return EXIT_SUCCESS;
}
