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

        long getTime(){

        }

        bool make(double* coords){

        }

        bool make(double* coords, double mass, double radius, double* trajectory){

        }

        bool make(double* coords, double mass, double radius, double* trajectory, Planet* parents){

        }

        Point* getObject(string name){

        }

        long passTime(long time){

        }

        bool deleteObject(string name){

        }

        bool editObject(string attribute, double* value){

        }

        

    private:

        vector<Body*> universe;
        long sim_time;

};