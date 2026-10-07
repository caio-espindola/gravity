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

        bool removeChild(Planet* p){
            for (int i = 0; i < children.size(); i++){
                if (p->equals((children.at(i)))){

                    children.erase(children.begin() + i);
                    return true;

                }
            }

            return false;
        }

        void clearChildren(){
            children.clear();
        }

        Planet* getChild(int i){
            return this->children.at(i);
        }

        string childList(){
            string names = "";

            for (int i = 0; i < children.size() - 1; i++){
                names += children.at(i)->getName();
                names += ", ";
            }

            names += children.back()->getName();

            return names;
        }

        void applyGravity(){

            for (Planet* p_child : this->children){
                this->applyGravTo(p_child);
            }

        }

    private:

        Tools::ObjectType obj_type = Tools::PLANET;
        vector<Planet*> children;

};