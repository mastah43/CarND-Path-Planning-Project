//
// Created by Marc Neumann on 28.10.18.
//

#ifndef PATH_PLANNING_FRENETCOORD_H
#define PATH_PLANNING_FRENETCOORD_H

class FrenetCoord {
public:
    FrenetCoord(double s, double d);

    /**
     * distance in meter along the road
     */
    double s;

    /**
     * deviation lateral in meter
     */
    double d;
};


#endif //PATH_PLANNING_FRENETCOORD_H
