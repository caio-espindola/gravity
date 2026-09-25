#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Point.h"
#include "Tools.h"
#include "Body.h"

class Planet: public Body {
    public:

        Planet(string name, double x, double y, double trajectory_abs_x, double trajectory_abs_y, double mass, double radius): Body(name, x, y, trajectory_abs_x, trajectory_abs_y, mass, radius){}

        void addChild(Planet* p){
            this->children.push_back(p);
        }

        Planet* getChild(int i){
            return this->children.at(i);
        }

        void applyGravity(){
            Planet* p_child;

            for (Planet* p_child : this->children){
                this->applyGravTo(p_child);
            }
        }

    private:

        vector<Planet*> children;

};