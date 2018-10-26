//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORY_H
#define PATH_PLANNING_TRAJECTORY_H

#include <vector>

/**
 * Describes a trajectory in points to reach after a fixed time interval
 * so two points being farther away then other two points means
 * that an object (e.g. ego vehicle) needs to follow.
 * @author Marc Neumann
 */
class Trajectory {
private:
    std::vector<double> x;
    std::vector<double> y;
public:
    Trajectory(const std::vector<double> &x, const std::vector<double> &y);

public:
    /**
     * @return the trajectory of x positions
     */
    const std::vector<double> &getX() const;

    /**
     * @return the trajectory of y positions
     */
    const std::vector<double> &getY() const;


};


#endif //PATH_PLANNING_TRAJECTORY_H
