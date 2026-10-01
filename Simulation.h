#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
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
            this->universe.push_back(b);
            return (universe.back() == b);
        }

        long getTime(){

            return sim_time;

        }

        long passTime(long time){

            for (long t = sim_time; t < sim_time + time; t++){
                for (Body* obj : universe){
                    obj->applyGravity(universe);
                    obj->updatePosition();
                }
                log();
                sim_time++;
            }

            return time + sim_time;

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

        }

        string fileLog(string name){
            ofstream log(name);
            clog.rdbuf(log.rdbuf());
            return "Logs redirected to file " + name;
        }

        string getDataPoint(Point* obj, string attribute){

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

        }

        string makeBody(){

        }

        string makePlanet(){

        }
};