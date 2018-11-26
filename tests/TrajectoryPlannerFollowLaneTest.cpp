#include "gtest/gtest.h"
#include "../src/trajectory_planner/TrajectoryPlannerFollowLane.h"
#include "../src/Trigonometry.h"
#include "Verifiers.h"
#include <math.h>

Map buildMapStraight()  {
    MapBuilder mapBuilder(1000);
    for (int i=0; i<10; i++) {
        double s = i*10;
        mapBuilder.addCoord(XYCoord(s,0), FrenetCoord(s,0), 0, -1);
    }
    Map map = *mapBuilder.build();
    return map;
}

TEST(TrajectoryPlannerFollowLane, PlanStraight) {

    Map map = buildMapStraight();
    TrajectoryPlannerFollowLane planner(map);
    SensorFusionResult sensorFusionResult;
    EgoVehicleState egoState;
    FrenetCoord egoPosF(0,0);
    egoState.setPos(map.getXY(egoPosF), egoPosF);
    TrajectoryFrenetEnd trajectoryPrevious(egoState.getFrenet());
    Trajectory trajectory = planner.planTrajectory(egoState, sensorFusionResult, trajectoryPrevious);
    double lastS = -1;
    for (int i=0; i<trajectory.size(); i++) {
        const XYCoord &c = trajectory.getAt(i);
        const FrenetCoord &f = map.getFrenet(c, egoState.getYaw());
        EXPECT_NEAR(0, f.d, 0.001);
        EXPECT_LT(lastS, f.s);
        lastS = f.s;
    }
    EXPECT_EQ(50, trajectory.size());
}



