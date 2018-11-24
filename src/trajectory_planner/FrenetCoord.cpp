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

std::ostream & operator<<(std::ostream & str, FrenetCoord const & c) {
    str << "(s=" << c.s << ";d=" << c.d << ")";
    return str;
}


