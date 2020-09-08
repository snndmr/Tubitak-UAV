import cv2

from fps import FPS

MIN_AREA = 1e3


def findRedColor(image, hsv):
    maskRedLeft = cv2.inRange(hsv, (0, 100, 100), (10, 255, 255))
    maskRedRight = cv2.inRange(hsv, (160, 100, 100), (180, 255, 255))
    maskedImage = cv2.bitwise_and(image, image, mask=maskRedLeft | maskRedRight)
    return cv2.cvtColor(maskedImage, cv2.COLOR_BGR2GRAY)


def isCircle(contour):
    approx = cv2.approxPolyDP(contour, 0.02 * cv2.arcLength(contour, True), True)
    return True if len(approx) > 6 and cv2.isContourConvex(approx) else False


class Detector:
    def __init__(self, cam_id):
        self.cap = cv2.VideoCapture(cam_id)
        self.fps = FPS()

    @staticmethod
    def process(image):
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        hsv = cv2.medianBlur(hsv, 3)

        contourRed = cv2.findContours(findRedColor(image, hsv), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]

        bundles = list()

        for contour in contourRed:
            if cv2.contourArea(contour) > MIN_AREA and isCircle(contour):
                bundles.append(('Circle', contour))

        return bundles
