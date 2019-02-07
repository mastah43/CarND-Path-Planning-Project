#include "gtest/gtest.h"
#include "../src/trajectory_planner/VehicleState.h"
#include "../src/trajectory_planner/WorldConstants.h"
#include "Verifiers.h"
#include <math.h>

VehicleState createVehicleWithFrenetD(double d) {
    return VehicleState(0, XYCoord(0,0), FrenetCoord(0,d), XYVelocity(0, 0));
}

TEST(VehicleStateTest, GetLane) {
    EXPECT_EQ(0, createVehicleWithFrenetD(LANE_WIDTH*0.5).getLane());
    EXPECT_EQ(1, createVehicleWithFrenetD(LANE_WIDTH*1.5).getLane());
    EXPECT_EQ(2, createVehicleWithFrenetD(LANE_WIDTH*2.5).getLane());
    EXPECT_EQ(-1, createVehicleWithFrenetD(LANE_WIDTH*-0.5).getLane());
}
