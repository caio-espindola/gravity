#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

namespace Tools{

    void vectorSum(double v1[], double v2[], double* sum){ // Receives two vectors and return their sum

        sum[0] = v1[0] + v2[0];
        sum[1] = v1[1] + v2[1];

    }

    void vectorSub(double v1[], double v2[], double* sub){ // Receives two vectors and returns their difference

        sub[0] = v1[0] - v2[0];
        sub[1] = v1[1] - v2[1];

    }

    double getModulus(double vec[]){ // Receives a vector and returns its modulus / hipothenuse
        double squares_sum = (vec[0] * vec[0]) + (vec[1] * vec[1]);
        return sqrt(squares_sum);
    }

    double getArgument(double vec[]){ // Receives a vector and returns its argument / angle in radians
        return atan(vec[1]/vec[0]);
    }

    void toCartesian(double polar[], double* cartesian){ // Receives a polar vector (rho, theta) and returns its cartesian equivalent (x, y)

        double rho = polar[0];
        double theta = polar[1];

        double x = cos(theta) * rho;
        double y = sin(theta) * rho;

        cartesian[0] = x;
        cartesian[1] = y;

    }

    void toPolar(double cartesian[], double* polar){

    }

    string coordsToString(double coords[]){ // Returns a vector as a formated semicolon-separated string for use in CSV files
        string line = to_string(coords[0]) + ";" + to_string(coords[1]) + "\n";
        return line;
    }

    void absoluteVector(double coords[], double rel_vector[], double* abs_vec){ // Receives a set of coordinates and a vector originating in that point and returns an equivalent vector originating in (0, 0)

        vectorSub(rel_vector, coords, abs_vec);

    }

    double TONtoKG(double tons){ // Converts metric tons to kilograms
        return tons*1000;
    }

    double KMtoM(double kms){ // Converts kilometers to meters
        return kms*1000;
    }
}