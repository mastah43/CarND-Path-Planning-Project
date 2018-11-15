//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNER_H
#define PATH_PLANNING_TRAJECTORYPLANNER_H

#include "Map.h"
#include "EgoVehicleState.h"
#include "Trajectory.h"
#include "SensorFusionResult.h"
#include "TrajectoryFrenetEnd.h"

class TrajectoryPlanner {
private:
    Map map;
protected:
    Map getMap();

public:
    explicit TrajectoryPlanner(Map &map);

    virtual const Trajectory planTrajectory(const EgoVehicleState &egoState,
                                            const SensorFusionResult &sensorFusion,
                                            const TrajectoryFrenetEnd &trajectorPrevious);
};

#endif //PATH_PLANNING_TRAJECTORYPLANNER_H
