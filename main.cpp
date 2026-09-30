#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
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

    Body* allBodies[] = {p_sun, p_earth, p_moon};

    ofstream file("log.txt");

    double* coords = new double[2];
    double* trajectory = new double[2];

    int i;

    for (i = 0; i < 525680; i++){ //525680
        p_earth->getCoords(coords);
        p_earth->getTrajectory(trajectory);

        file << Tools::coordsToString(coords);

        p_sun->applyGravity();
        p_earth->applyGravity();
        p_moon->applyGravity();

        for (Body* element: allBodies){

            element->updatePosition();

        }
    }

    delete coords;
    file.close(); 
}

string prompt(){

    cout << ">";
    string cmdfull;
    cin >> cmdfull;
    cout << "\n";

    string cmd[] = {"", "", ""};
    Tools::split(cmdfull, cmd);

    string result = "";

    if (cmd[0].compare("time")){
        
        result = sim.time(cmd[1], cmd[2]);

    } else if (cmd[0].compare("make")){

        result = sim.make(cmd[1]);

    } else if (cmd[0].compare("delete")){

        result = sim.deleteObject(cmd[1]);

    } else if (cmd[0].compare("edit")){

        result = sim.edit(cmd[1]);

    } else if (cmd[0].compare("get")){

        result = sim.getData(cmd[1], cmd[2]);

    } else if (cmd[0].compare("log")){

        result = sim.configLog(cmd[1], cmd[2]);

    } else if (cmd[0].compare("quit")){

        exit(0);

    } else if (cmd[0].compare("quick")){

        quickTest();

    }

    return result;
}