#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Point.h"
#include "Tools.h"
#include "Data.h"

class Body: public Point {
    public:

        Body(double x, double y, double trajectory_abs_x, double trajectory_abs_y, double mass, double radius): Point(x, y, trajectory_abs_x, trajectory_abs_y){

            this->mass = mass;
            this->radius = radius;

        }

        bool colision(Body* b){
            double distance = this->getDistance(b);
            distance -= this->getRadius();
            distance -= b->getRadius();

            if (distance < 0){
                return true;
            } else {
                return false;
            }
        }

        void applyGravTo(Body* b){
            double dist = this->getDistance(b);
            double acceleration = (Data::G * Tools::TONtoKG(this->mass)) / (pow(Tools::KMtoM(dist), 2));
            acceleration *= 60; // apply for 60 seconds
            acceleration *= 0.06; // m/s to km/min

            double* rel_vector = new double[2];
            double* body_coords = new double[2];
            double* this_coords = new double[2];
            double* cartesians = new double[2];

            b->getCoords(body_coords);
            this->getCoords(this_coords);

            Tools::vectorSub(body_coords, this_coords, rel_vector);
            delete body_coords;
            delete this_coords;

            double argument = Tools::getArgument(rel_vector);
            delete rel_vector;

            double polars[2] = {acceleration, argument};

            Tools::toCartesian(polars, cartesians);

            b->applyTrajectory(cartesians);
            delete cartesians;
        }

        double getRadius(){
            return this->radius;
        }

        double getMass(){
            return this->mass;
        }

    private:
        double mass; // Tons
        double radius; // Kilometers
};