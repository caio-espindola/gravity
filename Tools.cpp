#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

namespace Tools{
    const double G = 6.674 * pow(10, -11);
    const double SOLARMASS = 1.989 * pow(10, 27);
    const double EARTHMASS = 5.972 * pow(10, 21);

    double* vectorSum(double v1[], double v2[]){
        double x, y;
        double* vector;

        x = v1[0] + v2[0];
        y = v1[1] + v2[1];

        vector[0] = x;
        vector[1] = y;

        return vector;
    }

    double* vectorSub(double v1[], double v2[]){
        double x, y;
        double* vector;

        x = v1[0] - v2[0];
        y = v1[1] - v2[1];

        vector[0] = x;
        vector[1] = y;

        return vector;
    }

    double getModulus(double vec[]){
        double squares_sum = (vec[0] * vec[0]) + (vec[1] * vec[1]);
        return sqrt(squares_sum);
    }

    double getArgument(double vec[]){
        return atan(vec[1]/vec[0]);
    }

    double* toCartesian(double polar[]){
        double rho = polar[0];
        double theta = polar[1];

        double x = cos(theta) * rho;
        double y = sin(theta) * rho;

        double* coords;
        coords[0] = x;
        coords[1] = y;

        return coords;
    }

    string coordsToString(double coords[]){
        string line = to_string(coords[0]) + ";" + to_string(coords[1]) + "\n";
        return line;
    }

    double* absoluteVector(double coords[], double rel_vector[]){
        double* abs_vec;
        double* result = vectorSub(rel_vector, coords);

        abs_vec[0] = result[0];
        abs_vec[1] = result[1];

        return abs_vec;
    }

    double TONtoKG(double tons){
        return tons*1000;
    }

    double KMtoM(double kms){
        return kms*1000;
    }
}