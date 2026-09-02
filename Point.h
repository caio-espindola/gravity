#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Tools.h"

class Point {
    public:
        
        Point(double x, double y){
            coords[0] = x;
            coords[1] = y;
        }

        Point(double x, double y, double trajectory_abs_x, double trajectory_abs_y){
            coords[0] = x;
            coords[1] = y;
            trajectory_abs[0] = trajectory_abs_x;
            trajectory_abs[1] = trajectory_abs_y;
        }

        void updatePosition(){ // Alters the point's position based on its current trajectory
            double* coords = new double[2];
            double* trajectory = new double[2];

            this->getTrajectory(trajectory);
            this->getCoords(coords);

            Tools::vectorSum(coords, trajectory, coords);

            this->setCoords(coords[0], coords[1]);

            delete coords;
            delete trajectory;
        }

        void applyTrajectory(double vector[]){ // Receives a vector and updates this point's absolute trajectory as the sum of the previous trajectory and the new parameter

            Tools::vectorSum(this->trajectory_abs, vector, this->trajectory_abs);

        }

        double getSpeed(){ // Returns the absolute speed of the point in kilometers per minute
            return Tools::getModulus(this->trajectory_abs);
        }

        double getSpeed(Point* p){ // Receives a second point and returns this point's speed relative to it
            double* rel_trajectory = new double[2];
            double* this_trajectory = new double[2];
            double* point_trajectory = new double[2];

            this->getTrajectory(this_trajectory);
            p->getTrajectory(point_trajectory);
            
            Tools::vectorSub(this_trajectory, point_trajectory, rel_trajectory);
            double speed = Tools::getModulus(rel_trajectory);

            delete rel_trajectory;
            delete this_trajectory;
            delete point_trajectory;

            return speed;
        }

        void getTrajectory(double* trajectory){ // Returns the current absolute trajectory

            trajectory[0] = this->trajectory_abs[0];
            trajectory[1] = this->trajectory_abs[1];

        }
        
        void getCoords(double* coordinates){

            coordinates[0] = this->coords[0];
            coordinates[1] = this->coords[1];

        }

        void setCoords(double x, double y){
            this->coords[0] = x;
            this->coords[1] = y;
        }

        double getDistance(Point* p){

            double* rel_vector = new double[2];
            double* this_coords = new double[2];
            double* point_coords = new double[2];

            this->getCoords(this_coords);
            p->getCoords(point_coords);

            Tools::vectorSub(this_coords, point_coords, rel_vector);
            double distance = Tools::getModulus(rel_vector);

            delete rel_vector;
            delete this_coords;
            delete point_coords;

            return distance;
        }

    private:
        
        double coords[2];
        double trajectory_abs[2];
};