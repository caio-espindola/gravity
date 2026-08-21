#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

namespace Tools{
    const double G = 6.674 * pow(10, -11); // Gravitational Constant in m3/kg*s2
    const double SOLARMASS = 1.989 * pow(10, 27); // Mass of the sun in metric tons
    const double EARTHMASS = 5.972 * pow(10, 21); // Mass of the earth in metric tons
    const double APHELION = 152.1 * pow(10, 6); // Aphelion of the earth's orbit in kilometers

    double* vectorSum(double v1[], double v2[]){ // Receives two vectors and return their sum
        double x, y;
        double* vector = new double[2];

        x = v1[0] + v2[0];
        y = v1[1] + v2[1];

        vector[0] = x;
        vector[1] = y;

        return vector;
    }

    double* vectorSub(double v1[], double v2[]){ // Receives two vectors and returns their difference
        // cout << "vectorSub(sun->getCoords(), earth.getCoords())\n";

        double x, y;
        double* vector = new double[2];

        x = v1[0] - v2[0];
        y = v1[1] - v2[1];

        vector[0] = x;
        vector[1] = y;

        return vector;
    }

    double getModulus(double vec[]){ // Receives a vector and returns its modulus / hipothenuse
        double squares_sum = (vec[0] * vec[0]) + (vec[1] * vec[1]);
        return sqrt(squares_sum);
    }

    double getArgument(double vec[]){ // Receives a vector and returns its argument / angle in radians
        return atan(vec[1]/vec[0]);
    }

    double* toCartesian(double polar[]){ // Receives a polar vector (rho, theta) and returns its cartesian equivalent (x, y)
        double rho = polar[0];
        double theta = polar[1];

        double x = cos(theta) * rho;
        double y = sin(theta) * rho;

        double* coords = new double[2];
        coords[0] = x;
        coords[1] = y;

        return coords;
    }

    string coordsToString(double coords[]){ // Returns a vector as a formated semicolon-separated string for use in CSV files
        string line = to_string(coords[0]) + ";" + to_string(coords[1]) + "\n";
        return line;
    }

    double* absoluteVector(double coords[], double rel_vector[]){ // Receives a set of coordinates and a vector originating in that point and returns an equivalent vector originating in (0, 0)
        double* abs_vec = new double[2];
        double* result = new double[2];

        result[0] = vectorSub(rel_vector, coords)[0];
        result[1] = vectorSub(rel_vector, coords)[1];

        abs_vec[0] = result[0];
        abs_vec[1] = result[1];

        return abs_vec;
    }

    double TONtoKG(double tons){ // Converts metric tons to kilograms
        return tons*1000;
    }

    double KMtoM(double kms){ // Converts kilometers to meters
        return kms*1000;
    }
}