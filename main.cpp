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

int main(){

    Planet* p_sun = new Planet(0, 0, 0, 0, Data::SOLARMASS, 695700);
    Planet* p_earth = new Planet(Data::APHELION, 0, 0, 1757.4, Data::EARTHMASS, 6371);

    Planet sun = *p_sun;
    Planet earth = *p_earth;

    sun.addChild(p_earth);

    Body* allBodies[] = {p_sun, p_earth};

    ofstream file("log.txt");

    double* coords = new double[2];

    for (int i = 0; i < 10; i++){ //525600
        earth.getCoords(coords);

        file << Tools::coordsToString(coords);

        sun.applyGravity();

        for (Body* element: allBodies){

            element->updatePosition();

        }
    }

    delete coords;
    file.close();
}