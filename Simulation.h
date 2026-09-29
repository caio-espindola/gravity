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

            long value;

            try {
                value = stol(val);
            } catch (invalid_argument e){
                throw "InvalidParameterException";
            }

            if (mode.compare("get")){

                return to_string(getTime());

            } else if (mode.compare("pass")){

                return to_string(passTime(value));

            } else if (mode.compare("save")){

                return saveTime(value);

            } else if (mode.compare("load")){

                return loadTime(value);

            } else {

                throw "InvalidParameterException";

            }

        }

        string configLog(string mode, string name){

            if (mode.compare("toggle")){

                return toggleLog();

            } else if (mode.compare("verbose")){

                return verboseLog();

            } else if (mode.compare("file")){

                return fileLog(name);

            } else {

                throw "InvalidParameterException";

            }

        }

        string make(string mode){

            string msg;

            if (mode.compare("point")){

                msg = makePoint();

            } else if (mode.compare("body")){

                msg = makeBody();

            } else if (mode.compare("planet")){

                msg = makePlanet();

            } else {

                throw "InvalidParameterException";

            }

            return msg;

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



        }

        string getData(string name, string attribute){

        }

        Point* getObject(string name){

            for (Point* obj : universe){
                if (obj->getName().compare(name)){
                    return obj;
                }
            }

            return nullptr;

        }

    private:

        vector<Body*> universe;
        long sim_time;

        long getTime(){

            return sim_time;

        }

        long passTime(long time){

        }

        string saveTime(int slot){

        }

        string loadTime(int slot){

        }

        string toggleLog(){

        }

        string verboseLog(){

        }

        string fileLog(string name){

        }

        string makePoint(){

        }

        string makeBody(){

        }

        string makePlanet(){

        }

        bool editObject(string attribute, double* value){

        }

};