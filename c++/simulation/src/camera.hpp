#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "detector.hpp"
#include "map.hpp"

class Camera {
   private:
    Map map;
    Point2d pos;
    Detector *detector;
    const Size sizeOfView = Size(400, 400);

    bool isCircleFind = false;
    bool isSquareFind = false;

    Rect getROI();
    bool isArrive(Point2d);
    void moveTo(Mat, Point2d);
    void grabble(Mat, Bundle);

   public:
    Camera(Map);
    ~Camera();

    void init();
};

#endif /* CAMERA_HPP */
