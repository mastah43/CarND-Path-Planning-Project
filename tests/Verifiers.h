//
// Created by Marc Neumann on 25.11.18.
//

#include "gtest/gtest.h"


#ifndef PATH_PLANNING_XYTESTUTIL_H
#define PATH_PLANNING_XYTESTUTIL_H

#define EXPECT_XY_EQ(xExpected, yExpected, actual)\
    EXPECT_NEAR(xExpected, actual.x, 0.001); \
    EXPECT_NEAR(yExpected, actual.y, 0.001);

#define EXPECT_FRENET_EQ(sExpected, dExpected, actual)\
    EXPECT_NEAR(sExpected, actual.s, 0.001); \
    EXPECT_NEAR(dExpected, actual.d, 0.001);

#endif //PATH_PLANNING_XYTESTUTIL_H
