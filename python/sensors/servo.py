import RPi.GPIO as GPIO
from time import sleep

GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)

pwm=GPIO.PWM(11, 100)
pwm.start(0)

while True:
    for i in range(0, 24, 1):
        pwm.ChangeDutyCycle(i)
        sleep(0.08)
    
    for i in range(24, 0, -1):
        pwm.ChangeDutyCycle(i)
        sleep(0.08)

pwm.stop()
GPIO.cleanup()