//NURAFRINA BATRISYIA BINTI NORDZAMAN 123130337 TT3L

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "Mapping.h"

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

using namespace std;

class battlefield {
protected:
    int** coordinates; // List of all robot's coordinates
    string* elem_name_list; // List of all robot's name
    int elem_lives;
    int elem_list_size; // Size of robot_list

    int* revivecount;
    int* revivequeue;

    int x_axis; // Width of map
    int y_axis; // Height of map

    int target_steps; //Final Step
    int current_step; //current step

    char** map; // 2D array of the map

    string logs; // Logs
    string mapstring; // used to print the map

    string all_robot_types[8] = {"RoboCop","Terminator","BlueThunder","TerminatorRoboCop","Madbot","RoboTank","UltimateRobot","FrogRobot"};
    char all_robot_symbol[8] = {'R','T','B','A','M','E','U','F'};

    mapper<string,char> all_robot_symbol_type_map = mapper<string,char>(all_robot_types,all_robot_symbol,8);

public:
    char* elem_list; // List of all types of robots

    battlefield() {}

    void setxy(int x_axis,int y_axis){
        //Set x_axis n y_axis
        this->x_axis = x_axis;
        this->y_axis = y_axis;
    }

    ~battlefield() {
        //Deconstruct
        for (int i = 0; i < x_axis; ++i) {
            delete[] map[i];
        }
        delete[] map;

        for (int i = 0; i < elem_list_size; ++i) {
            delete[] coordinates[i];
        }
        delete[] coordinates;
    }

    void initialize_map(){
        // Allocate memory for the map
        map = new char*[x_axis];
        for (int i = 0; i < x_axis; ++i) {
            map[i] = new char[y_axis];
        }

        //

        // Initialize map with empty spaces
        for (int i = 0; i < x_axis; ++i) {
            for (int j = 0; j < y_axis; ++j) {
                map[i][j] = ' ';
            }
        }
    }

    void initialize_size_n_coords(int elem_list_size){ //and also revivequeue n revivecount
        this->elem_list_size = elem_list_size;

        coordinates = new int*[elem_list_size];
        for (int i = 0; i < elem_list_size; ++i) {
            coordinates[i] = new int[2];
        }

        revivecount = new int[elem_list_size];
        for (int i = 0; i < elem_list_size; ++i) {
            revivecount[i] = 3;
        }

        revivequeue = new int[elem_list_size];
        for (int i = 0; i < elem_list_size; ++i) {
            revivequeue[i] = 0;  //means no need revive
        }
    }

    void accessfile(ifstream& inputFile);

    // Randomly places elements on the battlefield, ensuring no duplicate coordinates
    void randomize(char* elem_list, string* elem_name_list) {

        this->elem_list = elem_list;
        this->elem_name_list = elem_name_list;

        // Initialize random engine and distribution
        srand(time(0));

        for (int i = 0; i < elem_list_size; ++i) {
            int temp1, temp2;
            bool unique;
            do {
                unique = true; // Assuming every coord is unique at the start
                temp1 = rand()% x_axis;
                temp2 = rand()% y_axis;

                // Check if the generated coordinates are already in use
                for (int j = 0; j < i; ++j) {
                    if (coordinates[j][0] == temp1 && coordinates[j][1] == temp2) {
                        unique = false;
                        break;
                    }
                }
            } while (!unique);

            coordinates[i][0] = temp1;
            coordinates[i][1] = temp2;
        }
    }

    // Initializes the battlefield map
    void setup() {
        // Give the map initial data
        for (int i = 0; i < x_axis; ++i) {
            for (int j = 0; j < y_axis; ++j) {
                map[i][j] = ' ';
            }
        }
        // Place elements on the map
        for (int i = 0; i < elem_list_size; ++i) {
            int x = coordinates[i][0];
            int y = y_axis - coordinates[i][1] -1;
            map[x][y] = elem_list[i];
        }
    }

    // Clears the map and refreshes the elements
    void refresh() {
        // Clear the map
        for (int i = 0; i < x_axis; ++i) {
            for (int j = 0; j < y_axis; ++j) {
                map[i][j] = ' ';
            }
        }

        // Place elements on the map
        for (int i = 0; i < elem_list_size; ++i) {
            if (checkrevive(i) == 0){
                int x = coordinates[i][0];
                int y = y_axis - coordinates[i][1] -1;
                map[x][y] = elem_list[i];
            }
        }
    }

    // Changes the coordinates of an element and refreshes the map
    void changecoords(int index, int* new_coords) {
        coordinates[index][0] = new_coords[0];
        coordinates[index][1] = new_coords[1];
        refresh();
    }

    // Prints the coordinates of a specific element
    void printcoords(int index, ofstream& outputFile) {
        outputFile << "Element " << elem_list[index] << " is at " << coordinates[index][0] << ", " << coordinates[index][1] <<  endl;
    }

    bool checkoccupied(int x, int y){
        if (map[x][y_axis - y - 1] != ' '){
            return true;
        } else {
            return false;
        }
    }

    // Prints the coordinates of all elements
    void logallelem(ofstream& outputFile) {
        for (int i = 0; i < elem_list_size; i++) {
            if (revivecount[i] > 0){
                logs += elem_name_list[i];
                if (coordinates[i][0] != -99){
                   logs += " is at ";
                   outputFile<<logs;
                   logs += to_string(coordinates[i][0]);
                    logs += ", ";
                    logs += to_string(coordinates[i][1]);
                    logs += "\n";
                } else {
                    logs += " is temporary dead\n";
                    outputFile<<logs;
                }
            }
        }
    }

    void printlogs(ofstream& outputFile) {
        outputFile<<logs;
        logs = "";
    }

    // Prints the battlefield map
    void printmap(ofstream& outputFile) {
        outputFile<< " " <<endl;
        mapstring += "+";
        for (int i = 0; i < x_axis; i++) {
            mapstring += "---+";
        }
        mapstring += "\n";
        for (int i = 0; i < y_axis; i++) {
            for (int j = 0; j < x_axis; j++) {
                mapstring += "| ";
                mapstring += map[j][i];
                mapstring += " ";
            }
            mapstring += "|\n";
            for (int j = 0; j < x_axis; j++) {
                mapstring += "+---";
            }
            mapstring += "+\n";
        }
        outputFile << mapstring << endl;  // Write to file
        mapstring = "";
    }

    void destroyElem(int x, int y){
        int index = findIndex(x, y);
        revivecount[index] -= 1;
        revivequeue[index] = 1;
        coordinates[index][0] = -99;
        coordinates[index][1] = -99;
    }

    int findIndex(int x, int y){
        for (int i = 0; i < elem_list_size; ++i)
        {
            if (coordinates[i][0] == x && coordinates[i][1] == y)
            {
                return i; // Return the row index if the pair (x, y) is found
            }
        }
        return -1;
    }
    // Getters
    int getX(int index){
        return coordinates[index][0];
    }

    int getY(int index){
        return coordinates[index][1];
    }

    int getWidth() const{
        return x_axis;
    }

    int getHeight() const {
        return y_axis;
    }

    int get_elem_list_size(){
        return elem_list_size;
    }

    bool checkrevive(int x){
        if (revivequeue[x] == 1){
            return true;
        }
        return false;
    }

    int checklives(int x){
        return revivecount[x];
    }

    void revive(int x){
        if (revivecount[x] > 0){
            revivequeue[x] = 0;
            int temp1, temp2;
            bool unique;
            do {
                unique = true; // Assuming every coord is unique at the start
                temp1 = rand()% x_axis;
                temp2 = rand()% y_axis;

                // Check if the generated coordinates are already in use
                for (int j = 0; j < elem_list_size; ++j) {
                    if (coordinates[j][0] == temp1 && coordinates[j][1] == temp2) {
                        unique = false;
                        break;
                    }
                }
            } while (!unique);

            coordinates[x][0] = temp1;
            coordinates[x][1] = temp2;
            refresh();
        }
    }

    void change_elem_list(int i, char x){
        elem_list[i] = x;
    }

    string get_name(int i){
        return elem_name_list[i];
    }
};

// Definition of accessfile function outside the class
void battlefield::accessfile(ifstream& inputFile) {
    string line;

    getline(inputFile, line);
    istringstream iss(line);
    string ignore;
    iss >> ignore >> ignore >> ignore >> ignore >> x_axis >> y_axis;

    getline(inputFile, line);
    iss.clear();
    iss.str(line);
    iss >> ignore >> target_steps;

    getline(inputFile, line);
    iss.clear();
    iss.str(line);
    iss >> ignore >> elem_list_size;

    //cout << "x_axis: " << x_axis << " and y_axis: " << y_axis << endl;
    //cout << "steps: " << target_steps << endl;
    //cout << "elem_list_size: " << elem_list_size << endl;

    this->elem_list = new char[elem_list_size];
    this->elem_name_list = new string[elem_list_size];
    initialize_map();
    initialize_size_n_coords(elem_list_size);

    string elem, name, x, y;
    for (int i = 0; i < elem_list_size; ++i) {
        getline(inputFile, line);
        istringstream iss(line);
        iss >> elem >> name >> x >> y;

        cout << "Robot " << i + 1 << ": " << elem << " " << name << " (" << x << ", " << y << ")" << endl;

        if (x == "random" || y == "random") {
            int temp1, temp2;
            bool unique;
            do {
                unique = true;
                temp1 = rand() % x_axis;
                temp2 = rand() % y_axis;
                for (int j = 0; j < i; ++j) {
                    if (coordinates[j][0] == temp1 && coordinates[j][1] == temp2) {
                        unique = false;
                        break;
                    }
                }
            } while (!unique);
            x = to_string(temp1);
            y = to_string(temp2);
        }

        elem_list[i] = all_robot_symbol_type_map.get(elem);
        elem_name_list[i] = name;
        coordinates[i][0] = stoi(x);
        coordinates[i][1] = stoi(y);
    }

    setup();
}

#endif
