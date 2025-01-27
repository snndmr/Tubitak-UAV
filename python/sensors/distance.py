import time
import RPi.GPIO as GPIO

try:
      GPIO.setmode(GPIO.BOARD)

      PIN_TRIG = 7
      PIN_ECHO = 11

      GPIO.setup(PIN_TRIG, GPIO.OUT)
      GPIO.setup(PIN_ECHO, GPIO.IN)

      GPIO.output(PIN_TRIG, GPIO.LOW)

      print("Waiting for sensor to settle")
      time.sleep(2)

      while True:
          GPIO.output(PIN_TRIG, GPIO.HIGH)
          time.sleep(0.00001)
          GPIO.output(PIN_TRIG, GPIO.LOW)

          while GPIO.input(PIN_ECHO) == 0:
                pulse_start_time = time.time()
          while GPIO.input(PIN_ECHO) == 1:
                pulse_end_time = time.time()

          pulse_duration = pulse_end_time - pulse_start_time
          distance = round(pulse_duration * 340.29 / 2, 2)
          print("Distance: {}m".format(distance))
          time.sleep(0.1)
finally:
      GPIO.cleanup()