//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"
#include "../Trigonometry.h"

MapCoord::MapCoord(int index, const XYCoord xy, const FrenetCoord f, double dx, double dy) :
    index(index),
    xy(xy),
    f(f),
    dx(dx),
    dy(dy) {
}

double MapCoord::distanceTo(const XYCoord &c) const {
    return MapCoord::xy.distanceTo(c);
}

double MapCoord::headingTo(const XYCoord &xy) const {
    return atan2((MapCoord::xy.y - xy.y), (MapCoord::xy.x - xy.x));
}

double MapCoord::headingTo(const MapCoord &c) const {
    return MapCoord::headingTo(c.xy);
}



