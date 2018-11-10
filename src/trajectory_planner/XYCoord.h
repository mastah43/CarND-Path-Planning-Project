//
// Created by Marc Neumann on 29.10.18.
//

#ifndef PATH_PLANNING_XYCOORD_H
#define PATH_PLANNING_XYCOORD_H

#include <math.h>

class XYCoord {
public:

    XYCoord(double x, double y);

    /**
     * in meter
     */
    double x;

    /**
     * in meter
     */
    double y;

    double distanceTo(const XYCoord &c) const;
};

#endif //PATH_PLANNING_XYCOORD_H
