#pragma once

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <omp.h>
#include <chrono>

#include "Planet.h"
#include "Body.h"
#include "Point.h"
#include "Tools.h"
#include "Data.h"

using namespace std;

class Simulation {
    public:

        /* Calls simulation time operations
           @param mode The operation to be called
           @param val The value for pass, save or load
           @returns Operation results
           @throws Returns "Invalid argument:" and the offending parameter
        */
        string time(string mode, string val){

            if (mode == "get"){

                return "Current time: " + to_string(getTime());

            }

            long value;

            try {

                value = stol(val);

            } catch (invalid_argument){

                return "Invalid Argument: " + val;

            }
            
            
            if (mode == "pass"){

                return "New current time: " + to_string(passTime(value));

            } else if (mode == "save"){

                return saveTime(value);

            } else if (mode == "load"){

                return loadTime(value);

            } else {

                return "Invalid argument: " + mode;

            }

        }

        /* Calls simulation log operations
           @param mode The operation to be called
           @param name The filename for log redirection
           @returns Operation results
           @throws Returns "Invalid argument:" and the offending parameter
        */
        string configLog(string mode, string name){

            if (mode == "toggle"){

                return toggleLog();

            } else if (mode == "verbose"){

                return verboseLog();

            } else if (mode == "file"){

                return fileLog(name);

            } else {

                return "Invalid argument: " + mode;

            }

        }

        /* Calls simulation make operations
           @param mode The type of object to be created
           @returns Operation results
           @throws Returns "Invalid argument:" if type is invalid
        */
        string make(string mode){

            if (mode == "point" || mode == "body" || mode == "planet"){

                return makePoint(mode);

            } else {

                return "Invalid argument: " + mode;

            }
        }

        /* Finds an object by name and removes it from the simulation
           @param name The object to be deleted
           @return The result of the operation
           @throws Returns "Object <name> not found" if no such object exists
        */
        string deleteObject(string name){

            Point* p_obj = getObject(name);

            if (p_obj == nullptr){
                return "Object \"" + name + "\" not found";
            }

            for (int i = 0; i < universe.size(); i++){
                if (universe.at(i)->getName() == name){

                    universe.erase(universe.begin() + i);
                    delete p_obj;
                    return "Object \"" + name + "\" deleted";

                }
            }

            for (int i = 0; i < points.size(); i++){
                if (points.at(i)->getName() == name){

                    points.erase(points.begin() + i);
                    delete p_obj;
                    return "Object \"" + name + "\" deleted";

                }
            }

            return "Object \"" + name + "\" not found";

        }

        /* Calls the simulation edit operations
           @param name The object to be edited
           @param attribute The field to be modified
           @returns Operation results
           @throws "Not found", "Unknown type", "No attribute", "Unknown command"
        */
        string edit(string name, string attribute){

            Point* obj = getObject(name);

            if (obj == nullptr){
                return "Object \"" + name + "\" not found";
            }

            if (obj->getType() == "point"){

                return editPoint(obj, attribute);

            } else if (obj->getType() == "body"){

                return editBody((Body*) obj, attribute);

            } else if (obj->getType() == "planet"){

                return editPlanet((Planet*) obj, attribute);

            } else {
                return "Unknown type: " + obj->getType();
            }

        }

        /* Calls the simulation get operations
           @param name The object to be inspected
           @param attribute The field to be accessed
           @returns Operation results
           @throws "Not found", "Unknown type", "No attribute"
        */
        string getData(string name, string attribute){

            Point* obj = getObject(name);

            if (obj == nullptr){
                return "Object \"" + name + "\" not found";
            }
            
            if (obj->getType() == "point"){

                return getDataPoint(obj, attribute);

            } else if (obj->getType() == "body"){

                return getDataBody((Body*) obj, attribute);

            } else if (obj->getType() == "planet"){

                return getDataPlanet((Planet*) obj, attribute);

            } else {

                return "Unknown type: " + obj->getType();
            }

        }

        /* Closes the log filestream
        */
        void closeLog(){
            clog.rdbuf(originalclog);
            logstream->close();
        }

        /* Initializes the log filestream to "log.csv"
        */
        void setupLog(){

            logstream = new ofstream("log.csv");
            originalclog = clog.rdbuf(logstream->rdbuf());

        }

    private:

        vector<Body*> universe;
        vector<Point*> points;
        long sim_time = 0;

        ofstream* logstream;
        streambuf* originalclog;
        bool logging = true;
        bool verbose = false;

        // Universe:

        /* Return a pointer to the object with the given name in universe[] or points[]
           @param name The name of the object to be retrieved
           @returns A Point pointer to the object
           @throws Returns nullptr if object not found
        */
        Point* getObject(string name){

            for (Body* obj : universe){
                if (obj->getName() == name){
                    return obj;
                }
            }

            for (Point* p : points){
                if (p->getName() == name){
                    return p;
                }
            }

            return nullptr;

        }

        /* Adds an object to the universe array
           @param b A Body object
           @returns True if the addition was succesful
        */
        bool addToUniverse(Body* b){
            if (getObject(b->getName()) == nullptr){
                this->universe.push_back(b);
                return (universe.back()->equals(b));
            } else {
                return false;
            }
        }


        /* Adds an object to the points array
           @param b A Point object
           @returns True if the addition was succesful
        */
        bool addToPoints(Point* p){
            if (getObject(p->getName()) == nullptr){
                points.push_back(p);
                return (points.back()->equals(p));
            } else {
                return false;
            }
        }

        // Time

        /* Returns the current sim time
        */
        long getTime(){

            return sim_time;

        }


        /* Passes time and calculates gravity and trajectories
           @param time The amount of time in minutes to be passed
           @returns The new current time
        */
        long passTime(long time){

            const long start_time =  sim_time;
            const int n = universe.size();
            const long finish_time = start_time + time;

            if (n == 0){
                return start_time + abs(time);
            }

            clock_t start = clock();

            if (time >= 0){

#               pragma omp parallel num_threads(n)
                {
                    Body* obj = universe.at(omp_get_thread_num());

                    for (long t = start_time; t < finish_time; t++){

                        obj->receiveGravity(universe);
#                       pragma omp barrier

                        obj->updatePosition();
#                       pragma omp barrier

                        if (omp_get_thread_num() == 0){
                            log();
                            sim_time++;
                        }
                        
                    }
                }

            } else {

//                 for (Body* b : universe){
//                     b->inverse();
//                 }

// #               pragma omp parallel num_threads(n)
//                 {
//                     Body* obj = universe.at(omp_get_thread_num());

//                     for (long t = start_time; t > finish_time; t--){

// #                       pragma omp barrier
//                         obj->receiveGravity(universe);
// #                       pragma omp barrier
//                         obj->updatePosition();
// #                       pragma omp barrier

//                         if (omp_get_thread_num() == 0){
//                             log();
//                             sim_time--;
//                         }
                        
//                     }
//                 }

//                 for (Body* b : universe){
//                     b->inverse();
//                 }

            }

            clock_t end = clock();
            clock_t duration = end - start;
            duration = (float) duration / CLOCKS_PER_SEC;

            cout << "delta-t: " << to_string(duration) << "\n";

            return sim_time;

        }

        string saveTime(int slot){

            string name = "slot" + to_string(slot) + ".sv";

            ofstream save_file(name);

            double* coords = new double[2];
            double* traj = new double[2];

            for (Body* o : universe){

                o->getCoords(coords);
                o->getTrajectory(traj);

                save_file << o->getName()
                << " " << to_string(coords[0])
                << " " << to_string(coords[1])
                << " " << to_string(traj[0])
                << " " << to_string(traj[1])
                << " " << to_string(o->getMass())
                << " " << to_string(o->getRadius())
                << "\n";
            }

            save_file.close();

            return "State saved into file " + name;

        }

        string loadTime(int slot){

            string name = "slot" + to_string(slot) + ".sv";
            string file_line;

            vector<string*> data;

            ifstream load_file(name);

            while(getline(load_file, file_line)){

                string* word_array = new string[7];
                for (int i = 0; i < 7; i++) {word_array[i] = "";}

                Tools::split(file_line, word_array, 7);
                data.push_back(word_array);

            }

            universe.clear();

            for (string* s : data){

                string name = s[0];
                double x  = stod(s[1]);
                double y = stod(s[2]);
                double dx = stod(s[3]);
                double dy = stod(s[4]);
                double mass = stod(s[5]);
                double radius = stod(s[6]);

                Body* b = new Body(name, x, y, dx, dy, mass, radius);

                universe.push_back(b);

            }

            for (string* s : data){
                delete s;
            }

            load_file.close();

            return "State loaded from file " + name;

        }

        // Log:

        /* Writes the current position of all objects to the current log file
        */
        void log(){

            if (!logging){
                return;
            }

            double* coords = new double[2];
            double* traj = new double[2];

            if (!verbose){

                clog << sim_time << ";";

                for (Body* obj : universe){

                    obj->getCoords(coords);

                    string name = obj->getName();
                    string str_coords = Tools::coordsToString(coords);

                    clog << name << ";" << str_coords << ";";
                }

                clog << "\n";
            }

            if (verbose){

                clog << sim_time << ";";

                for (Body* obj : universe){

                    obj->getCoords(coords);
                    obj->getTrajectory(traj);

                    string name = obj->getName();
                    string str_coords = Tools::coordsToString(coords);
                    string str_traj = Tools::coordsToString(traj);

                    clog << name << ";" << str_coords << ";" << str_traj << ";";
                }

                clog << "\n";
            }

            delete coords, traj;
        }

        /* Toggles logging of objects
           @returns The new logging status
        */
        string toggleLog(){
            if (logging){
                logging = false;
                return "Logging is now disabled";
            } else {
                logging = true;
                return "Logging is now enabled";
            }
        }

        /* Toggles logging of object trajectories
           @returns The new verbosity status
        */
        string verboseLog(){
            if (verbose){
                verbose = false;
                return "Verbose logging is now disabled";
            } else {
                verbose = true;
                return "Verbose logging is now enabled";
            }
        }

        /* Changes the file stream for logging output
           @param name The name of the new log file
           @returns The result of the operation
        */
        string fileLog(string name){
            logstream->close();
            logstream = new ofstream(name);
            clog.rdbuf(logstream->rdbuf());
            return "Logs redirected to file " + name;
        }

        // Get:

        /* Inspects an object for one of its fields
           @param obj A pointer to the object to be inspected
           @param attribute The field to be accessed
           @returns The resulting data
           @throws "No attribute"
        */
        string getDataPoint(Point* obj, string attribute){

            if (attribute == "coords" || attribute == "coordinates" || attribute == "pos" || attribute == "position"){

                double* coords = new double[2];
                obj->getCoords(coords);
                string str_coords = Tools::coordsToString(coords);
                delete coords;
                return str_coords;

            } else if (attribute == "x"){

                double* coords = new double[2];
                obj->getCoords(coords);
                string str_x = to_string(coords[0]);
                delete coords;
                return str_x;

            } else if (attribute == "y"){

                double* coords = new double[2];
                obj->getCoords(coords);
                string str_y = to_string(coords[1]);
                delete coords;
                return str_y;

            } else if (attribute == "trajectory" || attribute == "traj"){

                double* traj = new double[2];
                obj->getTrajectory(traj);
                string str_traj = Tools::coordsToString(traj);
                delete traj;
                return str_traj;

            } else if (attribute == "trajx" || attribute == "xtraj"){

                double* traj = new double[2];
                obj->getTrajectory(traj);
                string str_traj_x = to_string(traj[0]);
                delete traj;
                return str_traj_x;

            } else if (attribute == "trajy" || attribute == "ytraj"){

                double* traj = new double[2];
                obj->getTrajectory(traj);
                string str_traj_y = to_string(traj[1]);
                delete traj;
                return str_traj_y;

            } else if (attribute == "speed" || attribute == "velocity" || attribute == "vel"){

                double speed = obj->getSpeed();
                return to_string(speed);

            } else if (attribute == "type"){
            
                return obj->getType();

            } else {

                return "No attribute with name \"" + attribute + "\" for this object";

            }

        }

        /* Inspects an object for one of its fields
           @param obj A pointer to the object to be inspected
           @param attribute The field to be accessed
           @returns The resulting data
           @throws Calls getDataPoint() if attribute is not exclusive to Body
        */
        string getDataBody(Body* obj, string attribute){

            if (attribute == "mass"){

                double mass = obj->getMass();
                return to_string(mass);

            } else if (attribute == "radius" || attribute == "size"){

                double radius = obj->getRadius();
                return to_string(radius);

            } else {

                return getDataPoint(obj, attribute);

            }

        }

        /* Inspects an object for one of its fields
           @param obj A pointer to the object to be inspected
           @param attribute The field to be accessed
           @returns The resulting data
           @throws Calls getDataBody() if attribute is not exclusive to Planet
        */
        string getDataPlanet(Planet* obj, string attribute){

            if (attribute == "children" || attribute == "child" || attribute == "satellites" || attribute == "moon"){

                return obj->childList();

            } else {

                return getDataBody(obj, attribute);

            }

        }

        // Edit:

        string editPoint(Point* obj, string attribute){

            if (attribute == "coords" || attribute == "coordinates" || attribute == "pos" || attribute == "position"){

                double* coords = new double[2];
                double x, y;
                string str_x, str_y;
                obj->getCoords(coords);
                cout << "Current " + obj->getName() + " coordinates: " + Tools::coordsToString(coords) << "\n";

                cout << "New X value: ";
                getline(cin, str_x);
                cout << "\n";

                cout << "New Y value: ";
                getline(cin, str_y);
                cout << "\n";

                x = stod(str_x);
                y = stod(str_y);

                obj->setCoords(x, y);
                
                delete coords;

                return "Coordinates set";

            } else if (attribute == "x"){

                double* coords = new double[2];
                double x, y;
                string str_x;
                obj->getCoords(coords);
                cout << "Current " + obj->getName() + " coordinates: " + Tools::coordsToString(coords) << "\n";

                cout << "New X value: ";
                getline(cin, str_x);
                cout << "\n";

                x = stod(str_x);
                y = coords[1];

                obj->setCoords(x, y);

                delete coords;

                return "Coordinates set";

            } else if (attribute == "y"){

                double* coords = new double[2];
                double x, y;
                string str_y;
                obj->getCoords(coords);
                cout << "Current " + obj->getName() + " coordinates: " + Tools::coordsToString(coords) << "\n";

                cout << "New Y value: ";
                getline(cin, str_y);
                cout << "\n";

                x = coords[0];
                y = stod(str_y);

                obj->setCoords(x, y);

                delete coords;

                return "Coordinates set";

            } else if (attribute == "trajectory" || attribute == "traj"){

                double* traj = new double[2];
                double new_traj[2];
                string str_x, str_y;
                obj->getTrajectory(traj);
                cout << "Current " + obj->getName() + " trajectory: " + Tools::coordsToString(traj) << "\n";

                cout << "New dX value: ";
                getline(cin, str_x);
                cout << "\n";

                cout << "New dY value: ";
                getline(cin, str_y);
                cout << "\n";

                new_traj[0] = stod(str_x);
                new_traj[1] = stod(str_y);

                obj->setTrajectory(new_traj);
                
                delete traj;

                return "Trajectory set";

            } else if (attribute == "trajx" || attribute == "xtraj"){

                double* traj = new double[2];
                double new_traj[2];
                string str_x;
                obj->getTrajectory(traj);
                cout << "Current " + obj->getName() + " trajectory: " + Tools::coordsToString(traj) << "\n";

                cout << "New dX value: ";
                getline(cin, str_x);
                cout << "\n";

                new_traj[0] = stod(str_x);
                new_traj[1] = traj[1];

                obj->setTrajectory(new_traj);

                delete traj;

                return "Trajectory set";

            } else if (attribute == "trajy" || attribute == "ytraj"){

                double* traj = new double[2];
                double new_traj[2];
                string str_y;
                obj->getTrajectory(traj);
                cout << "Current " + obj->getName() + " trajectory: " + Tools::coordsToString(traj) << "\n";

                cout << "New dY value: ";
                getline(cin, str_y);
                cout << "\n";

                new_traj[0] = traj[0];
                new_traj[1] = stod(str_y);

                obj->setTrajectory(new_traj);

                delete traj;

                return "Trajectory set";

            } else if (attribute == "name"){

                string oldname, newname;
                oldname = obj->getName();

                cout << "New name for " + oldname + ": ";
                getline(cin, newname);
                cout << "\n";

                obj->setName(newname);

                return oldname + " is now called \"" + newname + "\"";

            } else {

                return "No attribute with name \"" + attribute + "\" for this object";

            }
        }

        string editBody(Body* obj, string attribute){

            if (attribute == "mass"){

                double mass;
                string str_mass;
                cout << "Current " + obj->getName() + " mass: " + to_string(obj->getMass()) << "\n";

                cout << "New mass value: ";
                getline(cin, str_mass);
                cout << "\n";

                mass = stod(str_mass);

                obj->setMass(mass);

                return "Mass set";

            } else if (attribute == "radius" || attribute == "size"){
            
                double rad;
                string str_rad;
                cout << "Current " + obj->getName() + " radius: " + to_string(obj->getRadius()) << "\n";

                cout << "New radius value: ";
                getline(cin, str_rad);
                cout << "\n";

                rad = stod(str_rad);

                obj->setRadius(rad);

                return "Radius set";

            } else {

                return editPoint(obj, attribute);

            }
        }

        string editPlanet(Planet* obj, string attribute){

            if (attribute == "children" || attribute == "child" || attribute == "satellites" || attribute == "moon"){

                cout << "Current children: " << obj->childList() << "\n";

                string cmd, name;
                cout << "Add, Remove or Clear? ";
                getline(cin, cmd);
                cout << "\n";

                for (char c : cmd){ c = tolower(c); }

                if (cmd == "add"){

                    cout << "Name of the object to be added: ";
                    getline(cin, name);
                    cout << "\n";

                    Point* child = getObject(name);

                    if (child == nullptr){

                        return "Object \"" + name + "\" not found";

                    }

                    obj->addChild((Planet*) child);

                    return "Object \"" + name + "\" added";

                } else if (cmd == "remove"){

                    cout << "Name of the object to be removed: ";
                    getline(cin, name);
                    cout << "\n";

                    Point* child = getObject(name);

                    if (child == nullptr){

                        return "Object \"" + name + "\" not found";

                    }

                    if (obj->removeChild((Planet*) child)){
                        return "Object \"" + name + "\" removed";
                    } else {
                        return "Object \"" + name + "\" is not a child of " + obj->getName();
                    }

                } else if (cmd == "clear"){

                    obj->clearChildren();
                    return "All children of \"" + obj->getName() + "\" removed";

                } else {

                    return "Unknown command: " + cmd;

                }

            } else {

                return editBody(obj, attribute);

            }
        }

        // Make:

        string makePoint(string type){

            string name, str_x, str_y, str_traj_x, str_traj_y;
            double x, y, traj_x, traj_y;

            cout << type + " name: ";
            getline(cin, name);

            cout << type + " x position: ";
            getline(cin, str_x);

            cout << type + " y position: ";
            getline(cin, str_y);

            cout << type + " x trajectory: ";
            getline(cin, str_traj_x);

            cout << type + " y trajectory: ";
            getline(cin, str_traj_y);

            x = stod(str_x);
            y = stod(str_y);

            try {

                traj_x = stod(str_traj_x);
                traj_y = stod(str_traj_y);

            } catch (exception e){

                traj_x = traj_y = 0;

            }

            if (type == "point"){

                Point* p_point = new Point(name, x, y, traj_x, traj_y);
                if (addToPoints(p_point)){
                    return "Point \"" + name + "\" created";
                } else {
                    delete p_point;
                    return "A point with this name already exists";
                }

            } else {

                return makeBody(type, name, x, y, traj_x, traj_y);

            }

        }

        string makeBody(string type, string name, double x, double y, double traj_x, double traj_y){

            string str_mass, str_radius;
            double mass, radius;

            cout << type + " mass: ";
            getline(cin, str_mass);

            cout << type + " radius: ";
            getline(cin, str_radius);

            mass = stod(str_mass);
            radius = stod(str_radius);

            if (type == "body"){
                Body* p_body = new Body(name, x, y, traj_x, traj_y, mass, radius);

                if (addToUniverse(p_body)){
                    return "Body \"" + name + "\" created";
                } else {
                    delete p_body;
                    return "An object with this name already exists";
                }
            } else {
                return makePlanet(type, name, x, y, traj_x, traj_y, mass, radius);
            }
        }

        string makePlanet(string type, string name, double x, double y, double traj_x, double traj_y, double mass, double radius){

            Planet* p_planet = new Planet(name, x, y, traj_x, traj_y, mass, radius);

            if (addToUniverse(p_planet)){
                return "Planet \"" + name + "\" created";
            } else {
                delete p_planet;
                return "An object with this name already exists";
            }
        }
};