#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>

#include "Planet.h"
#include "Body.h"
#include "Point.h"
#include "Tools.h"
#include "Data.h"

using namespace std;

class Simulation {
    public:

        string time(string mode, string val){

            if (mode == "get"){

                return "Current time: " + to_string(getTime());

            }

            long value = abs(stol(val));
            
            if (mode == "pass"){

                return "New current time: " + to_string(passTime(value));

            } else if (mode == "save"){

                return saveTime(value);

            } else if (mode == "load"){

                return loadTime(value);

            } else {

                throw invalid_argument(mode);

            }

        }

        string configLog(string mode, string name){

            if (mode == "toggle"){

                return toggleLog();

            } else if (mode == "verbose"){

                return verboseLog();

            } else if (mode == "file"){

                return fileLog(name);

            } else {

                throw invalid_argument(mode);

            }

        }

        string make(string mode){

            string msg;

            if (mode == "point"){

                return makePoint();

            } else if (mode == "body"){

                return makeBody();

            } else if (mode == "planet"){

                return makePlanet();

            } else {

                throw invalid_argument(mode);

            }
        }

        string deleteObject(string name){

            Point* p_obj = getObject(name);

            if (p_obj == nullptr){
                return "Object \"" + name + "\" not found";
            } else {
                delete p_obj;
                return "Object \"" + name + "\" deleted";
            }

        }

        string edit(string name){

            Point* obj = getObject(name);

            if (obj == nullptr){
                return "Object \"" + name + "\" not found";
            }

            if (obj->getType() == Tools::POINT){

                return editPoint(obj);

            } else if (obj->getType() == Tools::BODY){

                return editBody((Body*) obj);

            } else if (obj->getType() == Tools::PLANET){

                return editPlanet((Planet*) obj);

            } else {
                return "Unknown type: " + obj->getType();
            }

        }

        string getData(string name, string attribute){

            Point* obj = getObject(name);

            if (obj == nullptr){
                return "Object \"" + name + "\" not found";
            }
            
            if (obj->getType() == Tools::POINT){

                return getDataPoint(obj, attribute);

            } else if (obj->getType() == Tools::BODY){

                return getDataBody((Body*) obj, attribute);

            } else if (obj->getType() == Tools::PLANET){

                return getDataPlanet((Planet*) obj, attribute);

            } else {
                return "Unknown type: " + obj->getType();
            }

        }

        Point* getObject(string name){

            for (Point* obj : universe){
                if (obj->getName() == name){
                    return obj;
                }
            }

            return nullptr;

        }

    private:

        vector<Body*> universe;
        long sim_time = 0;
        bool logging = true;
        bool verbose = false;

        bool addToUniverse(Body* b){
            if (getObject(b->getName()) == nullptr){
                this->universe.push_back(b);
                return (universe.back() == b);
            } else {
                return false;
            }
        }

        long getTime(){

            return sim_time;

        }

        long passTime(long time){

            const long start_time =  sim_time;
            const int n = universe.size();

#           pragma omp parallel num_threads(n) private(time)
            {
                Body* obj = universe[omp_get_thread_num()];

                for (long t = start_time; t < start_time + time; t++){

#                   pragma omp barrier
                    obj->receiveGravity(universe);
#                   pragma omp barrier
                    obj->updatePosition();
#                   pragma omp barrier

                    if (omp_get_thread_num() == 0){
                        log();
                        sim_time++;
                    }
                    
                }
            }

            return sim_time;

        }

        void log(){

            double* coords = new double[2];
            double* traj = new double[2];

            if (logging && !verbose){
                for (Body* obj : universe){

                    obj->getCoords(coords);

                    string name = obj->getName();
                    string str_coords = Tools::coordsToString(coords);

                    clog << sim_time << ";" << name << ";" << str_coords << "\n";
                }
            }

            if (logging && verbose){
                for (Body* obj : universe){

                    obj->getCoords(coords);
                    obj->getTrajectory(traj);

                    string name = obj->getName();
                    string str_coords = Tools::coordsToString(coords);
                    string str_traj = Tools::coordsToString(traj);

                    clog << sim_time << ";" << name << ";" << str_coords << ";" << str_traj << "\n";
                }
            }

            delete coords, traj;
        }

        string saveTime(int slot){

        }

        string loadTime(int slot){

        }

        string toggleLog(){
            if (logging){
                logging = false;
                return "Logging is now disabled";
            } else {
                logging = true;
                return "Logging is now enabled";
            }
        }

        string verboseLog(){
            if (verbose){
                verbose = false;
                return "Verbose logging is now disabled";
            } else {
                verbose = true;
                return "Verbose logging is now enabled";
            }
        }

        string fileLog(string name){
            ofstream log(name);
            clog.rdbuf(log.rdbuf());
            return "Logs redirected to file " + name;
        }

        string getDataPoint(Point* obj, string attribute){

            if (attribute == "coords" || attribute == "coordinates" || attribute == "pos" || attribute == "position"){

                double* coords = new double[2];
                obj->getCoords(coords);
                string str_coords = Tools::coordsToString(coords);
                delete coords;
                return str_coords;

            } else if (attribute == "x"){

            } else if (attribute == "y"){

            } else if (attribute == "trajectory" || attribute == "traj"){

            } else if (attribute == "trajx" || attribute == "xtraj"){

            } else if (attribute == "trajy" || attribute == "ytraj"){

            } else if (attribute == "speed" || attribute == "velocity" || attribute == "vel"){

            } else {

                return "No attribute with name \"" + attribute + "\" for type Point";

            }

        }

        string getDataBody(Body* obj, string attribute){

        }

        string getDataPlanet(Planet* obj, string attribute){

        }

        string editPoint(Point* obj){

        }

        string editBody(Body* obj){

        }

        string editPlanet(Planet* obj){

        }

        string makePoint(){

            string name, str_x, str_y, str_traj_x, str_traj_y;
            double x, y, traj_x, traj_y;

            cout << "Point name: ";
            getline(cin, name);

            cout << "Point x position: ";
            getline(cin, str_x);

            cout << "Point y position";
            getline(cin, str_y);

            cout << "Point x trajectory: ";
            getline(cin, str_traj_x);

            cout << "Point y trajectory: ";
            getline(cin, str_traj_y);

            x = stod(str_x);
            y = stod(str_y);

            try {

                traj_x = stod(str_traj_x);
                traj_y = stod(str_traj_y);

            } catch (exception e){

                traj_x = traj_y = 0;

            }

            Point* p_point = new Point(name, x, y, traj_x, traj_y);

        }

        string makeBody(){

            string name, str_x, str_y, str_traj_x, str_traj_y, str_mass, str_radius;
            double x, y, traj_x, traj_y, mass, radius;

            cout << "Body name: ";
            getline(cin, name);

            cout << "Body x position: ";
            getline(cin, str_x);

            cout << "Body y position";
            getline(cin, str_y);

            cout << "Body x trajectory: ";
            getline(cin, str_traj_x);

            cout << "Body y trajectory: ";
            getline(cin, str_traj_y);

            cout << "Body mass: ";
            getline(cin, str_mass);

            cout << "Body radius: ";
            getline(cin, str_radius);

            x = stod(str_x);
            y = stod(str_y);
            traj_x = stod(str_traj_x);
            traj_y = stod(str_traj_y);

            mass = stod(str_mass);
            radius = stod(str_radius);

            Body* p_body = new Body(name, x, y, traj_x, traj_y, mass, radius);

            if (addToUniverse(p_body)){
                return "Body \"" + name + "\" created";
            } else {
                delete p_body;
                return "An object with this name already exists";
            }
        }

        string makePlanet(){

            string name, str_x, str_y, str_traj_x, str_traj_y, str_mass, str_radius;
            double x, y, traj_x, traj_y, mass, radius;

            cout << "Planet name: ";
            getline(cin, name);

            cout << "Planet x position: ";
            getline(cin, str_x);

            cout << "Planet y position";
            getline(cin, str_y);

            cout << "Planet x trajectory: ";
            getline(cin, str_traj_x);

            cout << "Planet y trajectory: ";
            getline(cin, str_traj_y);

            cout << "Planet mass: ";
            getline(cin, str_mass);

            cout << "Planet radius: ";
            getline(cin, str_radius);

            x = stod(str_x);
            y = stod(str_y);
            traj_x = stod(str_traj_x);
            traj_y = stod(str_traj_y);

            mass = stod(str_mass);
            radius = stod(str_radius);

            Planet* p_planet = new Planet(name, x, y, traj_x, traj_y, mass, radius);

            if (addToUniverse(p_planet)){
                return "Planet \"" + name + "\" created";
            } else {
                delete p_planet;
                return "An object with this name already exists";
            }

        }
};