//
// Created by Marc Neumann on 20.10.18.
//

#include "TrajectoryPlanner.h"

TrajectoryPlanner::TrajectoryPlanner(Map &map) : map(map) {
}

Map TrajectoryPlanner::getMap() {
    return map;
}

const Trajectory
TrajectoryPlanner::planTrajectory(const EgoVehicleState &egoState, const TrajectoryFrenetEnd &trajectorPrevious) {
    return Trajectory();
}

