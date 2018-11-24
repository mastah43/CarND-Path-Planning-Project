//
// Created by Marc Neumann on 20.10.18.
//

#include "TrajectoryPlanner.h"

TrajectoryPlanner::TrajectoryPlanner(Map &map) : map(map) {
}

Map TrajectoryPlanner::getMap() const {
    return map;
}

const Trajectory
TrajectoryPlanner::planTrajectory(EgoVehicleState &egoState, const SensorFusionResult &sensorFusion,
                                  const TrajectoryFrenetEnd &trajectorPrevious) {
    return Trajectory();
}


