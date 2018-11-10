//
// Created by Marc Neumann on 29.10.18.
//

#include "Map.h"

#include <memory>
#include "../Trigonometry.h"




Map::Map(double maxS) : maxS(maxS) {}

MapCoord& Map::getClosestWaypoint(const XYCoord &xy) const {

    double closestLen = 100000; // large number

    std::shared_ptr<MapCoord> closest = nullptr;
    for (MapCoord c : Map::coords) {
        double dist = c.distanceTo(xy);
        if (dist < closestLen) {
            closestLen = dist;
            closest = std::make_shared<MapCoord>(c);
        }
    }

    if (closest == nullptr) {
        throw std::out_of_range("map contains no coordinates");
    }

    return *closest;
}

MapCoord &Map::getNextWaypoint(const XYCoord &xy, double theta) const {
    MapCoord& closestWaypoint = getClosestWaypoint(xy);
    double heading = closestWaypoint.headingTo(xy);
    double angle = angleRadDiff(theta, heading);
    if (angle > pi() / 4) {
        return closestWaypoint.getNext();
    } else {
        return closestWaypoint;
    }
}

const FrenetCoord &Map::getFrenet(const XYCoord &xy, double theta) const {
    return coords[0].f; // TODO
}

MapCoord& Map::getPrevWaypointByFrenetS(double s) const {
    // TODO optimize performance by implementing binary search via s of given frenet coord
    std::shared_ptr<MapCoord> waypointNext = nullptr;
    for (const MapCoord &c : Map::coords) {
        if (s < c.f.s) {
            waypointNext = std::make_shared<MapCoord>(c);
        }
    }

    if (waypointNext == nullptr) {
        throw std::out_of_range("no way points in map");
    }

    return *waypointNext;
}

// TODO implement tests
const XYCoord &Map::getXY(const FrenetCoord &f) const {
    MapCoord waypointPrev = getPrevWaypointByFrenetS(f.s);
    MapCoord &waypointNext = waypointPrev.getNext();

    double heading = waypointNext.headingTo(waypointPrev);
    double segS = f.s - waypointPrev.f.s;
    double segX = waypointPrev.xy.x + segS * cos(heading);
    double segY = waypointPrev.xy.y + segS * sin(heading);

    double perpendicularHeading = anglePerpendicular(heading);

    double x = segX + f.d * cos(perpendicularHeading);
    double y = segY + f.d * sin(perpendicularHeading);

    // TODO fix, create object on heap
    return XYCoord(x, y);
}

const double Map::getMaxS() const {
    return maxS;
}
