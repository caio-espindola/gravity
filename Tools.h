#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include "Data.h"

using namespace std;

namespace Tools{

    void vectorSum(double v1[], double v2[], double* sum);
    void vectorSub(double v1[], double v2[], double* sub);
    double getModulus(double vec[]);
    double getArgument(double vec[]);
    void toCartesian(double polar[], double* cartesian);
    void toPolar(double cartesian[], double* polar);
    string coordsToString(double coords[]);
    void absoluteVector(double coords[], double rel_vector[], double* abs_vec);
    double calcAcceleration(double mass, double dist);
    double TONtoKG(double tons);
    double KMtoM(double kms);
    void split(string origin, string* target);
    void clearScreen();

    enum ObjectType {
        POINT,
        BODY,
        PLANET
    };

    /* Receives two vectors and return their sum
       @param v1 A cartesian vector
       @param v2 A cartesian vector
       @returns sum - A pointer to an array of double to receive the sum of the two vectors
       @throws invalid_argument(param) If any input parameters are null
       @throws invalid_argument("double* sum") If the output parameter is null
    */ 
    void vectorSum(double v1[], double v2[], double* sum){

        if (v1 == nullptr || v2 == nullptr){
            throw invalid_argument("double v1[]");
        } else if (v2 == nullptr){
            throw invalid_argument("double v2[]");
        } else if (sum == nullptr){
            throw invalid_argument("double* sum");
        }

        sum[0] = v1[0] + v2[0];
        sum[1] = v1[1] + v2[1];

    }

    /* Receives two vectors and returns their difference
       @param v1 A cartesian vector
       @param v2 A cartesian vector
       @return sub - A pointer to an array of double to receive the difference of the two vectors
       @throws invalid_argument(param) If any input parameters are null
       @throws invalid_argument("double* sub") If the output parameter is null
    */
    void vectorSub(double v1[], double v2[], double* sub){

        if (v1 == nullptr){
            throw invalid_argument("double v1[]");
        } else if (v2 == nullptr){
            throw invalid_argument("double v2[]");
        } else if (sub == nullptr){
            throw invalid_argument("double* sub");
        }

        sub[0] = v1[0] - v2[0];
        sub[1] = v1[1] - v2[1];

    }

    /* Receives a vector and returns its modulus / hipothenuse
       @param vec A cartesian vector
       @return The vector's distance to the origin
       @throws invalid_argument("double vec[]") if the input parameter is null
    */
    double getModulus(double vec[]){

        if (vec == nullptr){
            throw invalid_argument("double vec[]");
        }

        double squares_sum = (vec[0] * vec[0]) + (vec[1] * vec[1]);
        return sqrt(squares_sum);
    }

    /* Receives a vector and returns its argument / angle in radians
       @param vec A cartesian vector
       @return The vector's angle relative to the x-axis
       @throws invalid_argument("double vec[]") if the input parameter is null
    */
    double getArgument(double vec[]){
        if (vec == nullptr){
            throw invalid_argument("double vec[]");
        }

        double x = vec[0];
        double y = vec[1];

        if (x == 0 && y > 0){ // 90 deg

            return Data::PI / 2;

        } else if (x == 0 && y < 0){ // 270 deg

            return 3 * Data::PI / 2;

        } else if (x < 0){ // 2nd and 3rd quadrants

            return atan(y/x) + Data::PI;

        } else if (x > 0 && y <= 0){ // 4th quadrant and 360 deg

            return atan(y/x) + (2 * Data::PI);

        } else if (x == 0 && y == 0){ // (0, 0)

            return 0;
            
        }

        return atan(y/x); // 1st quadrant
    }

    /* Receives a polar vector and returns its cartesian equivalent
       @param polar A (rho, theta) polar vector
       @returns cartesian - A (x, y) cartesian vector
       @throws invalid_argument("double polar[]") If the input parameter is null
       @throws invalid_argument("double* cartesian") If the output parameter is null
    */
    void toCartesian(double polar[], double* cartesian){

        if (polar == nullptr){
            throw invalid_argument("double polar[]");
        } else if (cartesian == nullptr){
            throw invalid_argument("double* cartesian");
        }

        double rho = polar[0];
        double theta = polar[1];

        double x = cos(theta) * rho;
        double y = sin(theta) * rho;

        cartesian[0] = x;
        cartesian[1] = y;

    }

    /* Receives a cartesian vector and returns its polar equivalent
       @param cartesian An (x, y) cartesian vector
       @returns polar - An (rho, theta) polar vector
       @throws invalid_argument("double cartesian[]") if the input parameter is null
       @throws invalid_argument("double* polar") if the output parameter is null
    */
    void toPolar(double cartesian[], double* polar){

        if (cartesian == nullptr){
            throw invalid_argument("double cartesian[]");
        } else if (polar == nullptr){
            throw invalid_argument("double* polar");
        }

        double rho = getModulus(cartesian);
        double theta = getArgument(cartesian);

        polar[0] = rho;
        polar[1] = theta;

    }

    /* Returns a vector as a formatted string for use in CSV files
       @param coords A cartesian vector
       @returns A semicolon-separated string
       @throws invalid_argument("double coords[]") if the input parameter is null
    */
    string coordsToString(double coords[]){

        if (coords == nullptr){
            throw invalid_argument("double coords[]");
        }

        string line = to_string(coords[0]) + ";" + to_string(coords[1]) + "\n";
        return line;
    }

    /* Receives a vector originating in a point and returns an equivalent vector originating in (0, 0)
       @param coords A set of cartesian coordinates
       @param rel_vector A cartesian vector
       @returns abs_vec - A cartesian vector
       @throws invalid_argument(param) If any of the input parameters are null
       @throws invalid_argument("double* abs_vec") If the output parameter is null
    */
    void absoluteVector(double coords[], double rel_vector[], double* abs_vec){

        if (coords == nullptr){
            throw invalid_argument("double coords[]");
        } else if (rel_vector == nullptr){
            throw invalid_argument("double rel_vector[]");
        } else if (abs_vec == nullptr){
            throw invalid_argument("double* abs_vec");
        }

        vectorSub(rel_vector, coords, abs_vec);

    }

    /* Receives mass and distance and returns gravitational acceleration
       @param mass An object's mass in Tons
       @param dist The distance between the two objects in Kilometers
       @returns Gravitational acceleration in km/min
       @throws domain_error("Division by zero") If distance is zero
    */
    double calcAcceleration(double mass, double dist){

        if (dist == 0){
            throw domain_error("Division by zero");
        }

        double accel = (Data::G * TONtoKG(mass)) / (pow(KMtoM(dist), 2));
        accel *= 60; // apply for 60 seconds
        accel *= 0.06; // m/s to km/min

        return accel;
    }

    /* Converts metric tons to kilograms
       @param tons Mass in Tons
       @returns Mass in Kilograms
    */
    double TONtoKG(double tons){
        return tons*1000;
    }

    /* Converts kilometers to meters
       @param kms Distance in kilometers
       @returns Distance in Meters
    */
    double KMtoM(double kms){
        return kms*1000;
    }

    /* Takes as string and tokenizes it with whitespaces as separators
       @param origin The origin string
       @returns target - A string array
       @throws invalid_argument("string origin") If the input parameter is empty
       @throws invalid_argument("string* target") If the output parameter is null
    */
    void split(string origin, string* target){

        if (origin.empty()){
            throw invalid_argument("string origin");
        } else if (target == nullptr){
            throw invalid_argument("string* target");
        }

        int separator_index = 0;
        int target_index = 0;
        for (int i = 0; i < origin.length(); i++){
            if (origin[i] == ' '){
                for (int k = separator_index; k < i; k++){
                    target[target_index][k - separator_index] = origin[k];
                }

                target_index++;
                separator_index = i+1;
            }
        }

        if (target_index < 3){
            for (int k = separator_index; k < origin.length(); k++){
                target[target_index][k - separator_index] = origin[k];
            }
        }

    }

    /* Clears the terminal screen
    */
    void clearScreen(){

        cout << "\033[2J\033[1;1H";

    }
}