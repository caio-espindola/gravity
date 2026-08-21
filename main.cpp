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

using namespace std;

int main(){

    Planet* p_sun = new Planet(0, 0, 0, 0, Tools::SOLARMASS, 695700);
    Planet* p_earth = new Planet(Tools::APHELION, 0, 0, 1757.4, Tools::EARTHMASS, 6371);

    Planet sun = *p_sun;
    Planet earth = *p_earth;

    sun.addChild(earth);

    Body allBodies[] = {sun, earth};

    ofstream file("log.txt");

    for (int i = 0; i < 10; i++){ //525600
        file << Tools::coordsToString(earth.getCoords());

        sun.applyGravTo(p_earth);

        for (Body element: allBodies){

            element.recalculateTrajectory();

        }
    }

    file.close();
}