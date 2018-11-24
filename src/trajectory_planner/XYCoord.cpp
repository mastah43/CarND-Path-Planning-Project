//
// Created by Marc Neumann on 01.11.18.
//

#include "XYCoord.h"
#include <iostream>

XYCoord::XYCoord(double x, double y) : x(x), y(y) {

}

XYCoord::XYCoord(const XYCoord &xy) : x(xy.x), y(xy.y) {
}

double XYCoord::distanceTo(const XYCoord &c) const {
    double dx = c.x - x;
    double dy = c.y - y;
    return sqrt(dx * dx + dy * dy);
}

XYCoord XYCoord::movedYawXY(double yaw, double distance) {
    return XYCoord(XYCoord::x + cos(yaw)*distance, XYCoord::y + sin(yaw)*distance);
}

double XYCoord::headingTo(const XYCoord &c) const {
    return atan2((XYCoord::y - c.y), (XYCoord::x - c.x));
}

void XYCoord::transform(double tx, double ty, double tyaw) {
    double x = XYCoord::x;
    double y = XYCoord::y;
    x += tx;
    y += ty;
    double sinYaw = sin(tyaw);
    double cosYaw = cos(tyaw);
    XYCoord::x = x*cosYaw - y*sinYaw;
    XYCoord::y = x*sinYaw + y*cosYaw;
}

double XYCoord::distanceToOrigin() const {
    return XYCoord::distanceTo(XYCoord(0,0));
}

void XYCoord::cout() const {
    std::cout << "(" << x << ";" << y << ")";
}


