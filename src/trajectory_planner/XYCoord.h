//
// Created by Marc Neumann on 29.10.18.
//

#ifndef PATH_PLANNING_XYCOORD_H
#define PATH_PLANNING_XYCOORD_H

#include <math.h>

class XYCoord {
public:

    XYCoord(double x, double y);
    XYCoord(const XYCoord &xy);

    /**
     * in meter
     */
    double x;

    /**
     * in meter
     */
    double y;

    double distanceTo(const XYCoord &c) const;

    double distanceToOrigin() const;

    double headingTo(const XYCoord &c) const;

    /**
     * Transform by a translation and then a rotation.
     * @param tx translation x
     * @param ty translation y
     * @param tyaw radians angle for rotation
     */
    void transform(double tx, double ty, double tyaw);

    XYCoord movedYawXY(double yaw, double distance);

    void cout() const;
};

#endif //PATH_PLANNING_XYCOORD_H
