#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

namespace Data{
    const double G = 6.674 * pow(10, -11); // Gravitational Constant in m3/kg*s2
    const double SOLARMASS = 1.989 * pow(10, 27); // Mass of the sun in metric tons
    const double EARTHMASS = 5.972 * pow(10, 21); // Mass of the earth in metric tons
    const double APHELION = 152.1 * pow(10, 6); // Aphelion of the earth's orbit in kilometers
    const double PI = M_PI;
}