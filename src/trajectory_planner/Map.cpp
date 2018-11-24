//
// Created by Marc Neumann on 29.10.18.
//

#include "Map.h"

#include <memory>
#include "../Trigonometry.h"


double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

Map::Map(double maxS) : maxS(maxS) {}

const MapCoord& Map::getClosestWaypoint(const XYCoord &xy) const {

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

const MapCoord &Map::getNextWaypoint(const XYCoord &xy, double theta) const {
    const MapCoord& closestWaypoint = getClosestWaypoint(xy);
    double heading = closestWaypoint.headingTo(xy);
    double angle = angleRadDiff(theta, heading);
    if (angle > pi() / 4) {
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
    double proj_norm = (x_x*n_x+x_y*n_y)/(n_x*n_x+n_y*n_y);
    double proj_x = proj_norm*n_x;
    double proj_y = proj_norm*n_y;

    double frenet_d = distance(x_x,x_y,proj_x,proj_y);

    //see if d value is positive or negative by comparing it to a center point

    double center_x = 1000-prevWP.xy.x;
    double center_y = 2000-prevWP.xy.y;
    double centerToPos = distance(center_x,center_y,x_x,x_y);
    double centerToRef = distance(center_x,center_y,proj_x,proj_y);

    if(centerToPos <= centerToRef)
    {
        frenet_d *= -1;
    }

    // calculate s value
    double frenet_s = 0;
    for(int i = 0; i < prevWP.index; i++)
    {
        frenet_s += coords[i].distanceTo(coords[i+1].xy);
    }

    frenet_s += distance(0,0,proj_x,proj_y);

    return FrenetCoord(frenet_s,frenet_d);

}

const MapCoord& Map::getPrevWaypointByFrenetS(double s) const {
    // TODO optimize performance by implementing binary search via s of given frenet coord
    const MapCoord* waypointNext = &Map::coords[0];
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

    double heading = waypointPrev.headingTo(waypointNext);
    double segS = f.s - waypointPrev.f.s;
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
    return coords[(c.index -1) % coords.size()];
}
