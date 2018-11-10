//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"
#include <fstream>
#include <iostream>
// needed otherwise problem with istringstream(string) instantiation for iss problems
#include "../Eigen-3.3/Eigen/Core"

using namespace std;

Map &MapLoader::load(const std::string& file) {

    const double maxS = 6945.554;
    MapBuilder mapBuilder(6945.554);

    std::ifstream in_map_(file.c_str(), std::ifstream::in);

    string line;
    while (getline(in_map_, line)) {
        istringstream iss(line);
        double x;
        double y;
        float s;
        float d_x;
        float d_y;
        iss >> x;
        iss >> y;
        iss >> s;
        iss >> d_x;
        iss >> d_y;
    }

    /*
    std::string line;
    while (getline(in_map_, line)) {
        std::istringstream iss(line);
        double x;
        double y;
        float s;
        float dx;
        float dy;

        iss >> x;
        iss >> y;
        const XYCoord xy(x, y);

        iss >> s;
        iss >> dx;
        iss >> dy;

        // TODO what is dx and dy? I need only d.
        double d = sqrt(dx*dx + dy*dy);
        const FrenetCoord frenet(s, dx);

        MapCoord coord(xy, frenet);

        mapBuilder.addCoord(coord);
    }
     */

    return mapBuilder.build();
}
