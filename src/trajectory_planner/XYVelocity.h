//
// Created by Marc Neumann on 13.11.18.
//

#ifndef PATH_PLANNING_XYVELOCITY_H
#define PATH_PLANNING_XYVELOCITY_H


class XYVelocity {
public:
    double x;
    double y;

    XYVelocity(double x, double y);

    double getSpeed();
};


#endif //PATH_PLANNING_XYVELOCITY_H
