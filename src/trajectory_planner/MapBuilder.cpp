//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"

MapBuilder::MapBuilder(const double maxS) : maxS(maxS), map(new Map(maxS)){
}

void MapBuilder::addCoord(MapCoord &coordAdd) {
    long sizeBeforeAdd = map->coords.size();

    MapCoord& coordPrev = map->coords.empty() ? coordAdd : map->coords[sizeBeforeAdd - 1];
    map->coords.push_back(coordAdd);
    coordPrev.nextCoord = &coordAdd;
    coordAdd.prevCoord = &coordPrev;

    MapCoord& coordFirst = map->coords[0];
    coordAdd.nextCoord = &coordFirst;
    coordFirst.prevCoord = &coordAdd;
}

Map* MapBuilder::build() {
    Map* built = map;
    map = new Map(MapBuilder::maxS); // start new map
    return built;
}
