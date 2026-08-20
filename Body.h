#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Point.h"
#include "Tools.h"

class Body: public Point {
    public:
        double mass; // Tons
        double radius; // Kilometers

        Body(double x, double y, double trajectory_abs_x, double trajectory_abs_y, double mass, double radius): Point(x, y, trajectory_abs_x, trajectory_abs_y){
            this->x = x;
            this->y = y;
            this->mass = mass;
            this->radius = radius;
            coords[0] = x;
            coords[1] = y;
            trajectory_abs[0] = trajectory_abs_x;
            trajectory_abs[1] = trajectory_abs_y;
        }

        bool colision(Body b){
            double distance = this->getDistance(b);
            distance -= this->getRadius();
            distance -= b.getRadius();

            if (distance < 0){
                return true;
            } else {
                return false;
            }
        }

        void applyGravTo(Body b){
            double dist = this->getDistance(b);
            double acceleration = (Tools::G * Tools::TONtoKG(this->mass)) / (pow(Tools::KMtoM(dist), 2));
            acceleration *= 60; // apply for 60 seconds
            acceleration *= 0.06; // m/s to km/min

            double* rel_vector = new double[2];

            rel_vector[0] = Tools::vectorSub(b.getCoords(), this->getCoords())[0];
            rel_vector[1] = Tools::vectorSub(b.getCoords(), this->getCoords())[1];

            double argument = Tools::getArgument(rel_vector);
            delete rel_vector;

            double polars[2] = {acceleration, argument};
            double* cartesians = new double[2];

            cartesians[0] = Tools::toCartesian(polars)[0];
            cartesians[1] = Tools::toCartesian(polars)[1];

            double vector[2];
            vector[0] = cartesians[0];
            vector[1] = cartesians[1];
            delete cartesians;

            b.applyForce(vector);
        }

        double getRadius(){
            return radius;
        }
};