#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Point.cpp"
#include "Tools.cpp"
#include "Body.cpp"

class Planet: public Body {
    public:
        vector<Planet> children;

        Planet(double x, double y, double trajectory_abs_x, double trajectory_abs_y, double mass, double radius): Body(x, y, trajectory_abs_x, trajectory_abs_y, mass, radius){
            this->x = x;
            this->y = y;
            this->mass = mass;
            this->radius = radius;
            coords[0] = x;
            coords[1] = y;
            trajectory_abs[0] = trajectory_abs_x;
            trajectory_abs[1] = trajectory_abs_y;
        }

        void addChild(Planet p){
            this->children.push_back(p);
        }

};