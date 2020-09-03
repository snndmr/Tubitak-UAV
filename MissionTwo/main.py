import cv2
import time
import argparse

from pynput import keyboard
from pointer import Pointer
from detector import Detector
from dronekit import connect, VehicleMode, LocationGlobalRelative

MIN_ALT = 5.0
TOLERANCE = 0.0001
isListenerAlive = True

parser = argparse.ArgumentParser(description='Mission Two')
parser.add_argument('--camera', help='Raspberry Pi camera id string.', default=0)  # -1
parser.add_argument('--connect', help='Vehicle connection target string.', default='127.0.0.1:14550')  # /dev/serial0
args = parser.parse_args()


def add(feature):
    info = vehicle.location.global_relative_frame
    pointer.add_data({'Feature': feature, 'Alt': info.alt, 'Lat': info.lat, 'Lon': info.lon})
    print(' {} point added.'.format(feature))


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
            cv2.putText(frame, 'Circle {}, {}'.format(cX, cY), (cX, cY), cv2.FONT_ITALIC, 0.75, [255, 255, 255], 2)
        elif bundle[0] == 'Square':
            cv2.putText(frame, 'Square {}, {}'.format(cX, cY), (cX, cY), cv2.FONT_ITALIC, 0.75, [255, 255, 255], 2)
        cv2.polylines(frame, bundle[1], True, [255, 255, 255], 2)
        cv2.line(frame, centerOfCap, (cX, cY), [200, 255, 100], 2)

    cv2.putText(frame, 'FPS: {:.3f}'.format(detector.fps.calcFPS()), (5, 24), cv2.FONT_ITALIC, 0.75, [255, 255, 255], 2)
    cv2.drawMarker(frame, centerOfCap, [255, 255, 255], cv2.MARKER_CROSS, 20, 2)
    cv2.imshow("Window", frame)
    cv2.waitKey(1)


def arm_and_takeoff():
    print('\n Basic pre-arm checks.')
    while not vehicle.is_armable:
        print(' Waiting for vehicle to initialise.')
        time.sleep(1)

    print(' Arming motors.')
    # Copter should arm in GUIDED mode
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    # Confirm vehicle armed before attempting to take off
    while not vehicle.armed:
        print(' Waiting for arming.')
        time.sleep(1)

    vehicle.simple_takeoff(MIN_ALT)

    while True:
        # Break and return from function just below target altitude.
        if vehicle.location.global_relative_frame.alt >= MIN_ALT * 0.95:
            print(" Reached target altitude.")
            break
        print(' Reaching the target altitude {} ~ {}'.format(vehicle.location.global_relative_frame.alt, MIN_ALT))
        time.sleep(1)


def isReach(target):
    info = vehicle.location.global_relative_frame

    if info.alt >= target['Alt'] * 0.95 and \
            target['Lat'] + TOLERANCE >= info.lat >= target['Lat'] - TOLERANCE and \
            target['Lon'] + TOLERANCE >= info.lon >= target['Lon'] - TOLERANCE:
        return False
    return True


def move():
    for index in pointer.get_data():
        target = pointer.get_data()[index]

        point = LocationGlobalRelative(target['Lat'], target['Lon'], target['Alt'])
        vehicle.simple_goto(point)

        print(' Reaching the point {} - Feature: {}'.format(index, target['Feature']))
        while isReach(target):
            if target['Feature'] == 'Normal':
                if detector.cap.isOpened():
                    detector.cap.release()
            if target['Feature'] == 'Detect':
                if not detector.cap.isOpened():
                    detector.cap.open(args.camera)
                detect()


def on_press(key):
    try:
        if key.char == '1':
            add('Normal')
        elif key.char == '2':
            add('Detect')
        elif key.char == '3':
            add('Pickup')
        elif key.char == '4':
            add('Drop')
        elif key.char == '5':
            add('Turn')
    except AttributeError:
        if key == keyboard.Key.enter:
            pointer.write_to_file()
            print(' Data was created with {} points and saved in data.json file.'.format(len(pointer.get_data())))
        elif key == keyboard.Key.space:
            print(' The flight is starting.')
            global isListenerAlive
            isListenerAlive = False


if __name__ == '__main__':
    print('\n Connecting to vehicle on: {}'.format(args.connect))
    vehicle = connect(args.connect, wait_ready=True)

    pointer = Pointer()
    pointer.read_from_file()

    if not bool(pointer.get_data()):
        print(' Data file not found for the flight, data generator will be started.')
        print(' 1: Normal | 2: Detect | 3: Pickup | 4: Drop | 5: Turn | Enter: Save Points | Space: Start ')
        listener = keyboard.Listener(on_press=on_press)
        listener.start()

        while isListenerAlive:
            pass

    detector = Detector()
    arm_and_takeoff()
    move()

    vehicle.mode = VehicleMode("RTL")
