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

            bool success = false;

            if (mode.compare("point")){

                success = makePoint();

            } else if (mode.compare("body")){

                success = makeBody();

            } else if (mode.compare("planet")){

                success = makePlanet();

            }

            if (success){

                return mode + "created";

            } else {

                return mode + "not created: check data";

            }

        }

        string deleteObject(string name){

        }

        string edit(string name){

        }

        string getData(string name, string attribute){

        }

        Point* getObject(string name){

        }

    private:

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

        bool makePoint(){

        }

        bool makeBody(){

        }

        bool makePlanet(){

        }

        bool editObject(string attribute, double* value){

        }

        vector<Body*> universe;
        long sim_time;

};