//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"

void MapBuilder::addCoord(MapCoord &coord) {
    MapCoord& coordPrev = map.coords.empty() ? coord : map.coords[map.coords.size() - 1];
    coordPrev.nextCoord = &coord;
    coord.prevCoord = &coordPrev;
    coord.nextCoord = &map.coords[0];
    map.coords.push_back(coord);
}

Map &MapBuilder::build() {
    Map &built = map;
    map =  Map(MapBuilder::maxS); // start new map
    return built;
}

MapBuilder::MapBuilder(const double maxS) : maxS(maxS), map(Map(maxS)){
}
