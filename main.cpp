#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <omp.h>

#include "Planet.h"
#include "Body.h"
#include "Point.h"
#include "Tools.h"
#include "Data.h"
#include "Simulation.h"

using namespace std;

Simulation sim;
string prompt();
void quickTest();

int main(){
    Tools::clearScreen();
    sim.configLog("file", "log.csv");

    while(true){
        cout << prompt() << "\n";
    }
}

void quickTest(){
   
    Planet* p_sun = new Planet("sun", 0, 0, 0, 0, Data::SOLARMASS, 695700);
    Planet* p_earth = new Planet("earth", Data::APHELION, 0, 0, 1757.4, Data::EARTHMASS, 6371);
    Planet* p_moon = new Planet("moon", Data::APHELION + Data::APOGEE, 0, 0, 1757.4 + 57.96, Data::LUNARMASS, 1737.4);

    p_sun->addChild(p_earth);
    p_sun->addChild(p_moon);
    p_earth->addChild(p_sun);
    p_earth->addChild(p_moon);
    p_moon->addChild(p_sun);
    p_moon->addChild(p_earth);

    vector<Body*> allBodies = {p_sun, p_earth, p_moon};

    ofstream file("log.txt");

    double* coords = new double[2];
    double* trajectory = new double[2];

    const int n = allBodies.size();

    if (n > omp_get_max_threads()){

        cout << "Object number greater than max threads" << "\n";

    }

#   pragma omp parallel num_threads(n)
{
        Body* obj = allBodies[omp_get_thread_num()];

        for (int i = 0; i < 525680; i++){ //525680

            if (omp_get_thread_num() == 0){
                p_sun->getCoords(coords);
                p_sun->getTrajectory(trajectory);

                file << Tools::coordsToString(coords);
            }

    #       pragma omp barrier
            obj->receiveGravity(allBodies);
    #       pragma omp barrier
            obj->updatePosition();
    #       pragma omp barrier

        }
}

    delete coords, p_sun, p_moon, p_earth;
    file.close(); 
}

string prompt(){

    cout << ">";
    string cmdfull;
    getline(cin, cmdfull);
    cout << "\n";

    string cmd[] = {"", "", ""};
    Tools::split(cmdfull, cmd);

    string result = "";

    if (cmd[0] == "time"){
        
        result = sim.time(cmd[1], cmd[2]);

    } else if (cmd[0] == "make"){

        result = sim.make(cmd[1]);

    } else if (cmd[0] == "delete"){

        result = sim.deleteObject(cmd[1]);

    } else if (cmd[0] == "edit"){

        result = sim.edit(cmd[1], cmd[2]);

    } else if (cmd[0] == "get"){

        result = sim.getData(cmd[1], cmd[2]);

    } else if (cmd[0] == "log"){

        result = sim.configLog(cmd[1], cmd[2]);

    } else if (cmd[0] == "quit"){

        exit(0);

    } else if (cmd[0] == "quick"){

        quickTest();

    }

    return result;
}