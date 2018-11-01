//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"

void MapBuilder::addCoord(const MapCoord &coord) {
    map.coords.push_back(coord);
}

Map &MapBuilder::build() {
    Map &built = map;
    map = Map(); // start new map
    return built;
}

MapBuilder::MapBuilder(const double maxS) {
    map = Map(maxS);
}
