//
// Created by Marc Neumann on 29.10.18.
//

#ifndef PATH_PLANNING_MAP_H
#define PATH_PLANNING_MAP_H

#include <vector>
#include "XYCoord.h"
#include "FrenetCoord.h"

// TODO split into multiple headers?

class MapBuilder;

class MapCoord {
public:

    MapCoord(int index, XYCoord xy, FrenetCoord f, double dx, double dy);
    const XYCoord xy;

    const FrenetCoord f;
    const double dx;
    const double dy;
    const int index;

    double distanceTo(const XYCoord &c) const;

    /**
     * Determines the heading angle in radians from this map coordinate
     * to the given coordinate.
     * @param xy
     * @return 0 to 2*PI
     */
    double headingTo(const XYCoord &xy) const;

    double headingTo(const MapCoord &c) const;

};

class Map {
    friend class MapBuilder;
private:
    std::vector<MapCoord> coords;
    double maxS;
public:
    explicit Map(double maxS);

    const MapCoord& getNext(const MapCoord& c) const;
    const MapCoord& getPrev(const MapCoord& c) const;
    const MapCoord& getClosestWaypoint(const XYCoord &xy) const;
    const MapCoord& getPrevWaypointByFrenetS(double s) const;
    const MapCoord& getNextWaypoint(const XYCoord &xy, double theta) const;
    const FrenetCoord getFrenet(const XYCoord &xy, double theta) const;
    const XYCoord getXY(const FrenetCoord &f) const;

    /**
     * @return The max s value before wrapping around the track back to 0
     */
    const double getMaxS() const;
};

class MapBuilder {
private:
    Map* map;
    double maxS;
public:
    explicit MapBuilder(double maxS);

public:
    void addCoord(MapCoord &coordAdd);
    Map* build();
};

class MapLoader {
public:
    Map* load(const std::string &file);
};

#endif //PATH_PLANNING_MAP_H
