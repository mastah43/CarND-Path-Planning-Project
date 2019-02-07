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

    XYCoord getLast() const;

    double getDurationSecs() const;

    /**
    * Transform to local coordinate system by using given new origin and orientation.
    * @param origin absolute position of local coordinate system in global coordinate system
    * @param yaw orientation of local coordinate system in absolute coordinate syste in radians
    * @return reference to self
    */
    void transformToLocal(XYCoord origin, double yaw);

    friend std::ostream & operator<<(std::ostream & Str, Trajectory const & t);
};


#endif //PATH_PLANNING_TRAJECTORY_H
