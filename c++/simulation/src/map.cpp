#include "map.hpp"

Map::Map(Mat image) {
    image.copyTo(original);
    mapSize = Size(image.cols, image.rows);
}

Map::~Map() {}

Size Map::getMapSize() {
    return mapSize;
}

Mat Map::getOriginal() {
    return original;
}

vector<Point>& Map::getMarks() {
    return marks;
}