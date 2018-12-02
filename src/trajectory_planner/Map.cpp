//
// Created by Marc Neumann on 29.10.18.
//

#include "Map.h"

#include <cmath>
#include <memory>
#include "../Trigonometry.h"
#include "WorldConstants.h"

double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

Map::Map(double maxS) : maxS(maxS) {}

const MapCoord &Map::getClosestWaypoint(const XYCoord &xy) const {

    double closestLen = 100000; // large number

    int closestIndex = -1;
    for (MapCoord c : Map::coords) {
        double dist = c.distanceTo(xy);
        if (dist < closestLen) {
            closestLen = dist;
            closestIndex = c.index;
        }
    }

    if (closestIndex < 0) {
        throw std::out_of_range("map contains no coordinates");
    }

    return coords[closestIndex];
}

const MapCoord &Map::getNextWaypoint(const XYCoord &xy, double yaw) const {
    const MapCoord &closestWaypoint = getClosestWaypoint(xy);
    double heading = xy.headingTo(closestWaypoint.xy);
    double angle = angleRadDiff(yaw, heading);
    if (angle > M_PI / 4) {
        return getNext(closestWaypoint);
    } else {
        return closestWaypoint;
    }
}

const FrenetCoord Map::getFrenet(const XYCoord &xy, double theta) const {
    const MapCoord &nextWP = Map::getNextWaypoint(xy, theta);
    const MapCoord &prevWP = Map::getPrev(nextWP);

    double n_x = nextWP.xy.x - prevWP.xy.x;
    double n_y = nextWP.xy.y - prevWP.xy.y;
    double x_x = xy.x - prevWP.xy.x;
    double x_y = xy.y - prevWP.xy.y;

    // find the projection of x onto n
    double proj_norm = (x_x * n_x + x_y * n_y) / (n_x * n_x + n_y * n_y);
    double proj_x = proj_norm * n_x;
    double proj_y = proj_norm * n_y;

    double frenet_d = distance(x_x, x_y, proj_x, proj_y);
    if (prevWP.headingTo(xy) >= prevWP.headingTo(nextWP)) {
        frenet_d *= -1;
    }

    // calculate s value
    double frenet_s = 0;
    for (int i = 0; i < prevWP.index; i++) {
        frenet_s += coords[i].distanceTo(coords[i + 1].xy);
    }

    frenet_s += distance(0, 0, proj_x, proj_y);

    return FrenetCoord(frenet_s, frenet_d);

}

const MapCoord &Map::getPrevWaypointByFrenetS(double s) const {
    // TODO optimize performance by implementing binary search via s of given frenet coord
    s = normalizeFrenetS(s);
    const MapCoord *waypointNext = &Map::coords[0];
    for (const MapCoord &c : Map::coords) {
        if (s > c.f.s) {
            waypointNext = &c;
        }
    }

    return *waypointNext;
}

const XYCoord Map::getXY(const FrenetCoord &f) const {
    const MapCoord &waypointPrev = getPrevWaypointByFrenetS(f.s);
    const MapCoord &waypointNext = getNext(waypointPrev);
// TODO use a spline between map positions
    double heading = waypointPrev.headingTo(waypointNext);

    double segS = normalizeFrenetS(f.s - waypointPrev.f.s);
    double segX = waypointPrev.xy.x + segS * cos(heading);
    double segY = waypointPrev.xy.y + segS * sin(heading);

    double perpendicularHeading = anglePerpendicular(heading);

    double x = segX + f.d * cos(perpendicularHeading);
    double y = segY + f.d * sin(perpendicularHeading);

    return XYCoord(x, y);
}

const double Map::getMaxS() const {
    return maxS;
}

const MapCoord &Map::getNext(const MapCoord &c) const {
    return coords[(c.index + 1) % coords.size()];
}

const MapCoord &Map::getPrev(const MapCoord &c) const {
    return coords[(c.index - 1) % coords.size()];
}

const MapCoord &Map::getWaypointAt(int index) const {
    return coords[index];
}

const double Map::normalizeFrenetS(double frenetS) const {
    return std::fmod(frenetS, maxS);
}

unsigned int Map::getWaypointCount() const {
    return (unsigned int)coords.size();
}

double Map::getFrenetDeviationForLane(int lane) {
    return LANE_WIDTH/2 + (LANE_WIDTH * lane);
}
