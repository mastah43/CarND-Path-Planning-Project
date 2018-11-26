#include "gtest/gtest.h"
#include "../src/trajectory_planner/Map.h"
#include "Verifiers.h"

TEST(Map, GetFrenetAndGetXY) {
    Map map = *MapLoader().load("../data/highway_map.csv");

    XYCoord c(909.48,1128.67);
    double yaw = c.headingTo(XYCoord(912,1128.67));

    FrenetCoord f = map.getFrenet(c, yaw);
    EXPECT_FRENET_EQ(124.834, 6.16483, f);

    XYCoord c2 = map.getXY(f);
    EXPECT_XY_EQ(c.x, c.y, c2);
}




