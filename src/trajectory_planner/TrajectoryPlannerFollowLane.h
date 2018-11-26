//
// Created by Marc Neumann on 10.11.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNERFOLLOWLANE_H
#define PATH_PLANNING_TRAJECTORYPLANNERFOLLOWLANE_H

#include "TrajectoryPlanner.h"

class TrajectoryPlannerFollowLane : TrajectoryPlanner {

    EgoVehicleState applyPreviousTrajectory(EgoVehicleState &egoState, const TrajectoryFrenetEnd &trajectoryPrevious,
                                Trajectory &trajectory) const;
public:

    explicit TrajectoryPlannerFollowLane(const Map &map);

    const Trajectory planTrajectory(EgoVehicleState &egoState,
                                    const SensorFusionResult &sensorFusion,
                                    const TrajectoryFrenetEnd &trajectorPrevious) override;


};


#endif //PATH_PLANNING_TRAJECTORYPLANNERFOLLOWLANE_H
