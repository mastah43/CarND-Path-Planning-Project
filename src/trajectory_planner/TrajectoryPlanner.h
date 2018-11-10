//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNER_H
#define PATH_PLANNING_TRAJECTORYPLANNER_H


#include "EgoVehicleState.h"
#include "Trajectory.h"
#include "TrajectoryFrenetEnd.h"

class TrajectoryPlanner {
public:
    const Trajectory planTrajectory(const EgoVehicleState &egoState, const TrajectoryFrenetEnd &trajectorPrevious);
};

#endif //PATH_PLANNING_TRAJECTORYPLANNER_H
