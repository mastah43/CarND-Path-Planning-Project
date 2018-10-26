//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNER_H
#define PATH_PLANNING_TRAJECTORYPLANNER_H


#include "EgoVehicleState.h"
#include "Trajectory.h"

class TrajectoryPlanner {
public:
    const Trajectory& planTrajectory(const EgoVehicleState &ego_vehicle_state);
};


#endif //PATH_PLANNING_TRAJECTORYPLANNER_H
