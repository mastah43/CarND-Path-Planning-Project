#include "gtest/gtest.h"
#include "../src/trajectory_planner/XYCoord.h"

TEST(TestTest, FirstTest) {
    EXPECT_EQ(0., XYCoord(0,0).x);
};
