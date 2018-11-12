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
    friend class MapBuilder;
private:
    MapCoord *nextCoord;
    MapCoord *prevCoord;

public:
    MapCoord(const XYCoord &xy, const FrenetCoord &f);

    const XYCoord xy;
    const FrenetCoord f;

    MapCoord& getNext() const;
    MapCoord& getPrev() const;
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

    MapCoord& getClosestWaypoint(const XYCoord &xy) const;
    MapCoord& getPrevWaypointByFrenetS(double s) const;
    MapCoord& getNextWaypoint(const XYCoord &xy, double theta) const;
    const FrenetCoord& getFrenet(const XYCoord &xy, double theta) const;
    const XYCoord& getXY(const FrenetCoord &f) const;

    /**
     * @return The max s value before wrapping around the track back to 0
     */
    const double getMaxS() const;
};

class MapBuilder {
private:
    Map map;
    double maxS;
public:
    explicit MapBuilder(double maxS);

public:
    void addCoord(MapCoord &coord);
    Map& build();
};

class MapLoader {
public:
    Map& load(const std::string &file);
};

#endif //PATH_PLANNING_MAP_H
