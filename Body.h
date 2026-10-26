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

        Body(string name, double x, double y, double trajectory_abs_x, double trajectory_abs_y, double mass, double radius): Point(name, x, y, trajectory_abs_x, trajectory_abs_y){

            this->mass = abs(mass);
            this->radius = abs(radius);

        }

        virtual string getType() override {
            return "body";
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

            if (this == b){
                return;
            }

            double* rel_vector = new double[2];
            double* body_coords = new double[2];
            double* this_coords = new double[2];
            double* cartesians = new double[2];
            
            b->getCoords(body_coords);
            this->getCoords(this_coords);

            Tools::vectorSub(this_coords, body_coords, rel_vector);

            double acceleration = Tools::calcAcceleration(this->getMass(), Tools::getModulus(rel_vector));
            double argument = Tools::getArgument(rel_vector);

            double polars[2] = {acceleration, argument};

            Tools::toCartesian(polars, cartesians);

            b->applyTrajectory(cartesians);

            delete rel_vector, body_coords, this_coords, cartesians;

        }

        void applyGravity(vector<Body*> bodies){

            for (Body* b : bodies){
                this->applyGravTo(b);
            }
            
        }

        void receiveGravity(vector<Body*> bodies){

            for (Body* b : bodies){
                b->applyGravTo(this);
            }

        }

        double getRadius(){
            return this->radius;
        }

        void setRadius(double radius){
            this->radius = abs(radius);
        }

        double getMass(){
            return this->mass;
        }

        void setMass(double mass){
            this->mass = abs(mass);
        }

        void inverse(){
            this->mass = (this->mass) * (-1);

            double* traj = new double[2];

            this->getTrajectory(traj);
            double dx = traj[0] * -1;
            double dy = traj[1] * -1;
            this->setTrajectory(dx, dy);

            delete traj;
        }

    private:

        double mass; // Tons
        double radius; // Kilometers
};