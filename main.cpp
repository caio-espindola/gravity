#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include "Planet.cpp"
#include "Tools.cpp"

using namespace std;

int main(){
    double aphelion = 152.1 * pow(10, 6);

    Planet* p_sun = new Planet(0, 0, 0, 0, Tools::SOLARMASS, 695700);
    Planet* p_earth = new Planet(aphelion, 0, 0, 1757.4, Tools::EARTHMASS, 6371);

    Planet sun = *p_sun;
    Planet earth = *p_earth;

    sun.addChild(earth);

    ofstream file("log.txt");

    for (int i = 0; i < 525600; i++){
        file << Tools::coordsToString(earth.getCoords());

        sun.applyGravTo(earth);
    }

    file.close();
}