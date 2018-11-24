//
// Created by Marc Neumann on 01.11.18.
//

#include "FrenetCoord.h"
#include <iostream>

FrenetCoord::FrenetCoord(double s, double d) : s(s), d(d) {

}

void FrenetCoord::incS(double d) {
    FrenetCoord::s += d;
}

void FrenetCoord::cout(std::string msg) const {
    std::cout << msg << "(s=" << s << ";d=" << d << ")";
}


