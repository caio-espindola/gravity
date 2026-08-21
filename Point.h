#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Tools.h"

class Point {
    public:
        double x, y;
        double coords[2];
        double trajectory_abs[2];
        
        Point(double x, double y){
            this->x = x;
            this->y = y;
            coords[0] = x;
            coords[1] = y;
        }

        Point(double x, double y, double trajectory_abs_x, double trajectory_abs_y){
            this->x = x;
            this->y = y;
            coords[0] = x;
            coords[1] = y;
            trajectory_abs[0] = trajectory_abs_x;
            trajectory_abs[1] = trajectory_abs_y;
        }

        void applyForce(double vector[]){ // Receives a vector and updates this point's absolute trajectory as the sum of the previous trajectory and the new parameter
            // cout << "earth.applyForce()\n" << trajectory_abs[0] << "\n" << trajectory_abs[1] << "\n";
            
            trajectory_abs[0] = Tools::vectorSum(trajectory_abs, vector)[0];
            trajectory_abs[1] = Tools::vectorSum(trajectory_abs, vector)[1];

            cout << trajectory_abs[0] << "\n" << trajectory_abs[1] << "\n";
        }

        double getSpeed(){ // Returns the sbsolute speed of the point in kilometers per minute
            return Tools::getModulus(trajectory_abs);
        }

        double getSpeed(Point p){ // Receives a second point and returns this point's speed relative to it
            double* rel_trajectory = Tools::vectorSub(this->getTrajectory(), p.getTrajectory());
            return Tools::getModulus(rel_trajectory);
        }

        double* getTrajectory(){ // Returns the current absolute trajectory
            return trajectory_abs;
        }
        
        double* getCoords(){
            double* coordinates = new double[2];

            coordinates[0] = this->coords[0];
            coordinates[1] = this->coords[1];

            // cout << "sun.getCoords:\n" << coordinates;
            return coordinates;
        }

        double getDistance(Point p){
            // cout << "sun.getDistance(earth)\n";

            double* rel_vector = Tools::vectorSub(this->getCoords(), p.getCoords());
            return Tools::getModulus(rel_vector);
        }
};