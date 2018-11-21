//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"

MapBuilder::MapBuilder(const double maxS) : maxS(maxS), map(new Map(maxS)){
}

void MapBuilder::addCoord(MapCoord &coordAdd) {
    map->coords.push_back(coordAdd);
}

Map* MapBuilder::build() {
    Map* built = map;
    map = new Map(MapBuilder::maxS); // start new map
    return built;
}
