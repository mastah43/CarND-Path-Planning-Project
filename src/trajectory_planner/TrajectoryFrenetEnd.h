//
// Created by Marc Neumann on 27.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORYFRENETEND_H
#define PATH_PLANNING_TRAJECTORYFRENETEND_H

#include "Trajectory.h"
#include "FrenetCoord.h"

class TrajectoryFrenetEnd : public Trajectory {
private:
    FrenetCoord end;

public:
    TrajectoryFrenetEnd(FrenetCoord &end);

    const FrenetCoord &getEnd() const;

    void setFrenetEnd(double s, double d);

};


#endif //PATH_PLANNING_TRAJECTORYFRENETEND_H
