//
// Created by Marc Neumann on 10.11.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNERFOLLOWLANE_H
#define PATH_PLANNING_TRAJECTORYPLANNERFOLLOWLANE_H

#include "TrajectoryPlanner.h"

class TrajectoryPlannerFollowLane : TrajectoryPlanner {

public:
    explicit TrajectoryPlannerFollowLane(Map &map);
    const Trajectory planTrajectory(const EgoVehicleState &egoState, const TrajectoryFrenetEnd &trajectorPrevious);

};


#endif //PATH_PLANNING_TRAJECTORYPLANNERFOLLOWLANE_H
