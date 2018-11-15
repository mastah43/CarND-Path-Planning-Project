//
// Created by Marc Neumann on 10.11.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNERSTRAIGHT_H
#define PATH_PLANNING_TRAJECTORYPLANNERSTRAIGHT_H

#include "TrajectoryPlanner.h"

class TrajectoryPlannerStraight : TrajectoryPlanner {
public:

    const Trajectory planTrajectory(const EgoVehicleState &egoState,
                                    const SensorFusionResult &sensorFusion,
                                    const TrajectoryFrenetEnd &trajectorPrevious) override;
};


#endif //PATH_PLANNING_TRAJECTORYPLANNERSTRAIGHT_H
