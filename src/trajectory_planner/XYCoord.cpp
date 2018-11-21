//
// Created by Marc Neumann on 01.11.18.
//

#include "XYCoord.h"

XYCoord::XYCoord(double x, double y) : x(x), y(y) {

}

XYCoord::XYCoord(const XYCoord &xy) : x(xy.x), y(xy.y) {
}

double XYCoord::distanceTo(const XYCoord &c) const {
    double dx = c.x - x;
    double dy = c.y - y;
    return sqrt(dx * dx + dy * dy);
}

