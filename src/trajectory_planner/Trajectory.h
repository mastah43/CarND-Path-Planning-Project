//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORY_H
#define PATH_PLANNING_TRAJECTORY_H

#include <vector>
#include "XYCoord.h"

/**
 * Describes a trajectory in points to reach after a fixed time interval
 * so two points being farther away then other two points means
 * that an object (e.g. ego vehicle) needs to follow.
 * @author Marc Neumann
 */
class Trajectory {
private:
    std::vector<XYCoord> xy;
public:

    // TODO static const double intervalSecs = 0.02;

    Trajectory();
    virtual ~Trajectory() = default;

public:
    /**
     * @return the trajectory of x positions
     */
    const std::vector<double> getX() const;

    /**
     * @return the trajectory of y positions
     */
    const std::vector<double> getY() const;

    /**
     * @return the trajectory xy positions
     */
    const std::vector<XYCoord> &getXY() const;

    /**
     * Appends a way point
     * @param x
     * @param y
     */
    void append(double x, double y);

    void append(XYCoord xy);

    unsigned int size() const;

    XYCoord getAt(int index) const;

    void transform(double tx, double ty, double tyaw);

    void cout(std::string msg) const;
};


#endif //PATH_PLANNING_TRAJECTORY_H
