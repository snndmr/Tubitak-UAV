import cv2
from fps import FPS
from detector import Detector

if __name__ == '__main__':
    cap = cv2.VideoCapture(0)
    width, height = cap.get(cv2.CAP_PROP_FRAME_WIDTH), cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
    centerOfCap = (int(width / 2), int(height / 2))

    fps = FPS()

    while cap.isOpened():
        ret, frame = cap.read()

        if not ret:
            break

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

        cv2.putText(frame, 'FPS: {:.3f}'.format(fps.calcFPS()), (5, 24), cv2.FONT_ITALIC, 0.75, [255, 255, 255], 2)
        cv2.drawMarker(frame, centerOfCap, [255, 255, 255], cv2.MARKER_CROSS, 20, 2)
        cv2.imshow("Window", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            cap.release()
            break

    cv2.destroyAllWindows()
