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


