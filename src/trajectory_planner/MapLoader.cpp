//
// Created by Marc Neumann on 01.11.18.
//

#include "Map.h"
#include <fstream>
#include <iostream>
// needed otherwise problem with istringstream(string) instantiation for iss problems
#include "../Eigen-3.3/Eigen/Core"

Map* MapLoader::load(const std::string& file) {

    const double maxS = 6945.554;
    MapBuilder mapBuilder(6945.554);

    std::ifstream in_map_(file.c_str(), std::ifstream::in);

    std::string line;
    int nextCoordId = 1;
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

        const FrenetCoord frenet(s, 0);

        MapCoord coord(nextCoordId++, xy, frenet, dx, dy);

        mapBuilder.addCoord(coord);
    }

    return mapBuilder.build();
}
