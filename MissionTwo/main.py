import cv2
import math
import time
import argparse
# import RPi.GPIO as GPIO

from pointer import Pointer
from detector import Detector

from pynput import keyboard
from pymavlink import mavutil
from dronekit import connect, VehicleMode, LocationGlobalRelative

TOLERANCE = 0.0001

drop_area = {}
target_locked = False
drop_area_find = False
keyboard_listener_alive = True

parser = argparse.ArgumentParser(description='Mission Two')
parser.add_argument('--camera', help='Raspberry Pi camera id string.', default=0)  # -1
parser.add_argument('--takeoff', help='Takeoff height.', default=10)
parser.add_argument('--connect', help='Vehicle connection target string.', default='127.0.0.1:14550')  # /dev/serial0
args = parser.parse_args()


def send_body_ned_velocity(velocity_x, velocity_y, velocity_z):
    msg = vehicle.message_factory.set_position_target_local_ned_encode(
        0, 0, 0,
        mavutil.mavlink.MAV_FRAME_BODY_NED,
        0b0000111111000111,
        0, 0, 0,
        velocity_x, velocity_y, velocity_z,
        0, 0, 0, 0, 0)
    vehicle.send_mavlink(msg)


def detect():
    ret, frame = detector.cap.read()

    if not ret:
        return False

    width, height = detector.cap.get(cv2.CAP_PROP_FRAME_WIDTH), detector.cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
    centerOfCap = (int(width / 2), int(height / 2))
    bundles = Detector.process(frame)

    for bundle in bundles:
        M = cv2.moments(bundle[1])
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])

        if bundle[0] == 'Circle':
            cv2.polylines(frame, bundle[1], True, [100, 255, 255], 2)
            cv2.line(frame, centerOfCap, (cX, cY), [200, 255, 100], 2)

            info = vehicle.location.global_relative_frame

            x = centerOfCap[0] - cX
            y = centerOfCap[1] - cY

            try:
                rad = math.atan(x / y)
            except ZeroDivisionError:
                rad = 0

            global target_locked
            if not target_locked:
                deg = -1 * math.degrees(rad) if x > 0 and y > 0 or x < 0 and y > 0 else -1 * math.degrees(rad) + 180
                deg += math.degrees(vehicle.attitude.yaw)
                vehicle.gimbal.rotate(0, 0, int(deg))
                target_locked = True

            # velocity_x = 1, velocity_y = 0, velocity_z = 0
            send_body_ned_velocity(1, 0, 0)

            if abs(cX - centerOfCap[0]) < 10 and abs(cY - centerOfCap[1]) < 10:
                global drop_area_find
                global drop_area

                print(' Drop area found.')
                vehicle.simple_goto(vehicle.location.global_relative_frame)
                time.sleep(3)
                drop_area = {'Lat': info.lat, 'Lon': info.lon}
                drop_area_find = True

    cv2.putText(frame, 'FPS: {:.3f}'.format(detector.fps.calcFPS()), (5, 24), cv2.FONT_ITALIC, 0.75, [255, 255, 255], 2)
    cv2.drawMarker(frame, centerOfCap, [100, 255, 255], cv2.MARKER_CROSS, 20, 2)
    cv2.imshow("Window", frame)
    cv2.waitKey(1)


def arm_and_takeoff(mAlt):
    print('\n Basic pre-arm checks.')
    while not vehicle.is_armable:
        print(' Waiting for vehicle to initialise.')
        time.sleep(3)

    print(' Arming motors.')
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print(' Waiting for arming.')
        time.sleep(3)

    vehicle.simple_takeoff(mAlt)

    while True:
        if vehicle.location.global_relative_frame.alt >= mAlt * 0.95:
            print(" Reached target altitude.")
            break
        print(' Reaching the target altitude {} ~ {}'.format(vehicle.location.global_relative_frame.alt, mAlt))
        time.sleep(1)


def isReach(target):
    info = vehicle.location.global_relative_frame

    if target['Alt'] * 0.95 <= info.alt >= target['Alt'] * 1.05 and \
            target['Lat'] + TOLERANCE >= info.lat >= target['Lat'] - TOLERANCE and \
            target['Lon'] + TOLERANCE >= info.lon >= target['Lon'] - TOLERANCE:
        return False
    return True


def move():
    for index in pointer.get_data():
        pointer.show_specific_data(index)
        target = pointer.get_data()[index]

        point = LocationGlobalRelative(target['Lat'], target['Lon'], target['Alt'])
        vehicle.simple_goto(point, target['Speed'])

        while isReach(target):
            if target['Feature'] == 'Detect':
                if not detector.cap.isOpened():
                    detector.cap.open(args.camera)
                if not drop_area_find:
                    detect()
                if drop_area_find:
                    detector.cap.release()
                    cv2.destroyAllWindows()
                    break
        if target['Feature'] == 'Pickup':
            # turn_left()
            print(' Servo will run for 4 seconds')
            print('')
        elif target['Feature'] == 'Drop':
            # turn_right()
            print(' Servo will run for 4 seconds.')
            print('')


# #For SITL
# def add(feature):
#     info = vehicle.location.global_relative_frame
#     pointer.add_data({'Feature': feature, 'Alt': 10, 'Lat': info.lat, 'Lon': info.lon, 'Speed': 15})
#     print(' {} point added.'.format(feature))

# def on_press(key):
#     try:
#         if key.char == '1':
#             add('Normal')
#         elif key.char == '2':
#             add('Detect')
#         elif key.char == '3':
#             add('Pickup')
#         elif key.char == '4':
#             add('Drop')
#         elif key.char == '5':
#             add('Turn')
#     except AttributeError:
#         if key == keyboard.Key.enter:
#             pointer.write_to_file()
#             print(' Data was created with {} points and saved in data.json file.'.format(len(pointer.get_data())))
#         elif key == keyboard.Key.space:
#             print(' The flight is starting.')
#             global keyboard_listener_alive
#             keyboard_listener_alive = False


# def turn_left():
#     global pwm
#     pwm.ChangeDutyCycle(3)
#     time.sleep(5)
#     pwm.ChangeDutyCycle(7)
#
#
# def turn_right():
#     global pwm
#     pwm.ChangeDutyCycle(11)
#     time.sleep(5)
#     pwm.ChangeDutyCycle(7)


if __name__ == '__main__':
    # # Initialize of Servo
    # servoPIN = 18
    # GPIO.setwarnings(False)
    # GPIO.setmode(GPIO.BOARD)
    # GPIO.setup(servoPIN, GPIO.OUT)
    # pwm = GPIO.PWM(servoPIN, 50)
    # pwm.start(7)

    print('\n Connecting to vehicle on: {}'.format(args.connect))
    vehicle = connect(args.connect, wait_ready=True)

    pointer = Pointer()
    pointer.read_from_file()

    # First lap
    #
    #    (o)                                   (o)
    #       6N....5D................4N........3T
    #     .                 |                   .
    #   7T                  |                     2T
    #     .                 |                   .
    #       8T...........H..|................1N
    #                       |

    # # For SITL.
    # if not bool(pointer.get_data()):
    #     print(' Data file not found for the flight, data generator will be started.')
    #     print(' 1: Normal | 2: Detect | 3: Pickup | 4: Drop | 5: Turn | Enter: Save Points | Space: Start ')
    #     listener = keyboard.Listener(on_press=on_press)
    #     listener.start()
    #
    #     while keyboard_listener_alive:
    #         pass

    detector = Detector(args.camera)
    arm_and_takeoff(args.takeoff)
    move()

    # The second round will be like this autonomously.
    #
    #    (o)                                   (o)
    #       6N..................5D.....4P....3T
    #     .                 |                   .
    #   7T                  |                     2T
    #     .                 |                   .
    #       8T...........H..|................1N
    #                       |

    pointer.get_data()['4'] = {'Feature': 'Pickup', 'Alt': 0.5, 'Lat': -35.3625873, 'Lon': 149.1643327, 'Speed': 3}
    if drop_area_find:
        pointer.get_data()['5'] = \
            {'Feature': 'Drop', 'Alt': 3, 'Lat': drop_area['Lat'], 'Lon': drop_area['Lon'], 'Speed': 3}
        move()
    else:
        print(' Drop area not found.')

    print(' Return To Launch')
    vehicle.mode = VehicleMode("RTL")
