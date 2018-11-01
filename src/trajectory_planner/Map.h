//
// Created by Marc Neumann on 29.10.18.
//

#ifndef PATH_PLANNING_MAP_H
#define PATH_PLANNING_MAP_H

#include <vector>
#include "XYCoord.h"
#include "FrenetCoord.h"

class MapCoord {
public:
    MapCoord(const XYCoord &xy, const FrenetCoord &f);

    const XYCoord xy;
    const FrenetCoord f;
};

class MapBuilder;

class Map {
    friend class MapBuilder;
private:
    std::vector<MapCoord> coords;
    double maxS;
public:
    explicit Map(double maxS);

    const MapCoord getClosestWaypoint(const XYCoord &xy) const;
    const MapCoord getNextWaypoint(const XYCoord &xy, double theta) const;
    const FrenetCoord getFrenet(const XYCoord &xy, double theta) const;
    const XYCoord getXY(const FrenetCoord &f) const;

    /**
     * @return The max s value before wrapping around the track back to 0
     */
    const double getMaxS() const;
};

class MapBuilder {
private:
    Map map;
public:
    explicit MapBuilder(double maxS);

public:
    void addCoord(const MapCoord &coord);
    Map& build();
};

class MapLoader {
public:
    Map& load(const std::string &file);
};

#endif //PATH_PLANNING_MAP_H
