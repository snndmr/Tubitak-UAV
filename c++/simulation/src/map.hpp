#ifndef Map_HPP
#define Map_HPP

#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

class Map {
   private:
    Mat original;
    Size mapSize;
    vector<Point> marks;

   public:
    Map(Mat);
    ~Map();

    Mat getOriginal();
    Size getMapSize();
    vector<Point>& getMarks();
};

#endif /* Map_HPP */