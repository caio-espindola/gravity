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

        void applyForce(double vector[]){
            double* result = Tools::vectorSum(trajectory_abs, vector);
            trajectory_abs[0] = result[0];
            trajectory_abs[1] = result[1];
        }

        double getSpeed(){
            return Tools::getModulus(trajectory_abs);
        }

        double getSpeed(Point p){
            double* rel_trajectory = Tools::vectorSub(this->getTrajectory(), p.getTrajectory());
            return Tools::getModulus(rel_trajectory);
        }

        double* getTrajectory(){
            return trajectory_abs;
        }
        
        double* getCoords(){
            return this->coords;
        }

        double getDistance(Point p){
            double* rel_vector = Tools::vectorSub(this->getCoords(), p.getCoords());
            return Tools::getModulus(rel_vector);
        }
};