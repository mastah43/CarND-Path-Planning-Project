//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"

void MapBuilder::addCoord(const MapCoord &coord) {
    MapCoord& coordPrev = map.coords[map.coords.size() - 1];
    coordPrev.nextCoord.make_shared(coord);
    coord.prevCoord.make_shared(coordPrev);
    coord.nextCoord.make_shared(map.coords[0]);
    map.coords.push_back(coord);
}

Map &MapBuilder::build() {
    Map &built = map;
    map =  Map(MapBuilder::maxS); // start new map
    return built;
}

MapBuilder::MapBuilder(const double maxS) : maxS(maxS), map(Map(maxS)){
}
