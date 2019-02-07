//
// Created by Marc Neumann on 28.11.18.
//

#ifndef PATH_PLANNING_MAPTESTDATA_H
#define PATH_PLANNING_MAPTESTDATA_H

#include "../src/trajectory_planner/Map.h"

Map buildMapStraight(double distance, int waypointCount);

Map buildMapCircle(double radius, int waypointCount);

#endif //PATH_PLANNING_MAPTESTDATA_H
