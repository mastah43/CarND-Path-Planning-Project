//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"

MapBuilder::MapBuilder(const double maxS) : maxS(maxS), indexNext(0), map(new Map(maxS)){
}

void MapBuilder::addCoord(XYCoord xy, FrenetCoord f, double dx, double dy) {
    map->coords.emplace_back(MapCoord(indexNext++, xy, f, dx, dy));
}

Map* MapBuilder::build() {
    Map* built = map;
    map = new Map(MapBuilder::maxS); // start new map
    indexNext = 0;
    return built;
}

void MapBuilder::addCoord(XYCoord xy, double dx, double dy) {
    // The given highway map seems to have been built using frenet s coordinates
    // which does not take into account the curved road distance but only strait connections
    // between waypoints. So we do the same here.
    double s = 0;
    for (int i=1; i<map->coords.size(); i++) {
        s += map->coords[i-1].distanceTo(map->coords[i].xy);
    }
    addCoord(xy, FrenetCoord(s, 0), dx, dy);
}


