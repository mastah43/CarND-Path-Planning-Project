//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"
#include "../Trigonometry.h"

MapCoord::MapCoord(int id, const XYCoord &xy, const FrenetCoord &f) :
    id(id),
    xy(xy),
    f(f),
    nextCoord(this),
    prevCoord(this) {
}

double MapCoord::distanceTo(const XYCoord &c) const {
    return MapCoord::xy.distanceTo(c);
}

MapCoord& MapCoord::getNext() const {
    return *MapCoord::nextCoord;
}

MapCoord& MapCoord::getPrev() const {
    return *MapCoord::prevCoord;
}

double MapCoord::headingTo(const XYCoord &xy) const {
    return atan2((MapCoord::xy.y - xy.y), (MapCoord::xy.x - xy.x));
}

double MapCoord::headingTo(const MapCoord &c) const {
    return MapCoord::headingTo(c.xy);
}



