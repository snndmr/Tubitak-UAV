import cv2

from fps import FPS


def findRedColor(image, hsv):
    maskRedLeft = cv2.inRange(hsv, (0, 100, 100), (10, 255, 255))
    maskRedRight = cv2.inRange(hsv, (160, 100, 100), (180, 255, 255))
    maskedImage = cv2.bitwise_and(image, image, mask=maskRedLeft | maskRedRight)
    return cv2.cvtColor(maskedImage, cv2.COLOR_BGR2GRAY)


def findBlueColor(image, hsv):
    maskBlue = cv2.inRange(hsv, (90, 100, 100), (140, 255, 255))
    maskedImage = cv2.bitwise_and(image, image, mask=maskBlue)
    return cv2.cvtColor(maskedImage, cv2.COLOR_BGR2GRAY)


def isSquare(contour):
    (width, height) = cv2.minAreaRect(contour)[1]
    aspectRatio = min(width, height) / max(width, height)
    approx = cv2.approxPolyDP(contour, 0.04 * cv2.arcLength(contour, True), True)
    return True if len(approx) == 4 and cv2.isContourConvex(approx) and 0.9 < aspectRatio < 1.1 else False


def isCircle(contour):
    approx = cv2.approxPolyDP(contour, 0.02 * cv2.arcLength(contour, True), True)
    return True if len(approx) > 6 and cv2.isContourConvex(approx) else False


class Detector:
    def __init__(self):
        self.cap = cv2.VideoCapture()
        self.fps = FPS()

    @staticmethod
    def process(image):
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        hsv = cv2.medianBlur(hsv, 3)

        contourRed = cv2.findContours(findRedColor(image, hsv), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]
        contourBlue = cv2.findContours(findBlueColor(image, hsv), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]

        bundles = list()

        for contour in contourRed:
            if cv2.contourArea(contour) > 1e3 and isCircle(contour):
                bundles.append(('Circle', contour))

        for contour in contourBlue:
            if cv2.contourArea(contour) > 1e3 and isSquare(contour):
                bundles.append(('Square', contour))
        return bundles
