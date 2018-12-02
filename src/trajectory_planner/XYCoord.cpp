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

XYCoord& XYCoord::moveYaw(double yaw, double distance) {
    XYCoord::x += cos(yaw)*distance;
    XYCoord::y += sin(yaw)*distance;
    return *this;
}

double XYCoord::headingTo(const XYCoord &c) const {
    return atan2((c.y - XYCoord::y), (c.x - XYCoord::x));
}

double XYCoord::distanceToOrigin() const {
    return XYCoord::distanceTo(XYCoord(0,0));
}

std::ostream & operator<<(std::ostream & str, XYCoord const & c) {
    str << "(" << c.x << ";" << c.y << ")";
    return str;
}

XYCoord &XYCoord::transformToLocal(XYCoord origin, double yaw) {
    double xLocal = XYCoord::x;
    double yLocal = XYCoord::y;
    // translate
    xLocal -= origin.x;
    yLocal -= origin.y;
    // rotate
    double sinYaw = sin(-yaw);
    double cosYaw = cos(-yaw);
    XYCoord::x = xLocal*cosYaw - yLocal*sinYaw;
    XYCoord::y = xLocal*sinYaw + yLocal*cosYaw;
    return *this;
}

XYCoord &XYCoord::transformToGlobal(XYCoord origin, double yaw) {
    double x = XYCoord::x;
    double y = XYCoord::y;
    // rotate
    double cosYaw = cos(yaw);
    double sinYaw = sin(yaw);
    double xGlobal = x*cosYaw - y*sinYaw;
    double yGlobal = x*sinYaw + y*cosYaw;
    // translate
    XYCoord::x = xGlobal + origin.x;
    XYCoord::y = yGlobal + origin.y;
    return *this;
}

