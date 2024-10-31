//NURAFRINA BATRISYIA BINTI NORDZAMAN 123130337 TT3L

#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <stdlib.h>
#include <time.h> 

#include "Battlefield.h"

using namespace std;

// Base abstract class for all robots
class Robot {
protected:
    battlefield* gameMap;
    string name;
    int pos[2];
    int kills = 0;
    bool detect = false;
    int detectPos[2] = {0};
    int index;
    ofstream& outputFile;
    
public:
 Robot(battlefield* field, string name, int x, int y, int i, ofstream& outputFile) 
        : gameMap(field), name(name), pos{x, y}, index(i), outputFile(outputFile) {}

/*
    Robot(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) {
        this->gameMap = field; 
        this->name = name;
        pos[0] = x;
        pos[1] = y;
        index = i;
        outputFile(outputFile);
    }
*/
    // Pure virtual function to be overridden by subclasses
    virtual void operate() = 0;
    
    void refreshpos(int x, int y) {
        pos[0] = x;
        pos[1] = y;
    }
    
    int checkkills() {
        return kills;
    }
    
    string showname() {
        return name;
    }
};

// Subclass for robots that can see
class SeeingRobot : virtual public Robot {
public:
    SeeingRobot(battlefield* field, string name, int x, int y, int i, ofstream& outputFile) : Robot(field, name, x, y, i,outputFile) {}

    void operate() override {
        outputFile << name << " is performing a look...\n";
        for (int i = pos[0] - 1; i <= pos[0] + 1; i++) {
            for (int j = pos[1] - 1; j <= pos[1] + 1; j++) {
                if (j < 0 || j >= gameMap->getHeight() || i < 0 || i >= gameMap->getWidth()) {
                    continue;
                } else if (i == pos[0] && j == pos[1]) {
                    continue;
                } else {
                    if (gameMap->checkoccupied(i, j)) {
                        detect = true;
                        outputFile << "Terminator is at " << pos[0] << " " << pos[1] << endl;
                        detectPos[0] = i;
                        detectPos[1] = j;
                        outputFile << "Target spotted at: " << detectPos[0] << "," << detectPos[1] << endl;
                        return;
                    }
                }
            }
        }
    }
};

// Subclass for robots that can shoot
class ShootingRobot : virtual public Robot {
protected:
    int rotation = 0;
public:
    ShootingRobot(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i,outputFile) {}

    void operate() override {
        int targetx;
        int targety;
        do {
            targetx = rand() % gameMap->getWidth();
            targety = rand() % gameMap->getHeight();
        } while ((targetx == pos[0] && targety == pos[1]) || targetx + targety > 10);
        outputFile << name << " has shot {" << targetx << ", " << targety << "} " << endl;

        if (gameMap->checkoccupied(targetx, targety)) {
            gameMap->destroyElem(targetx, targety);
            gameMap->refresh();
            outputFile << name << " has killed the robot at {" << targetx << ", " << targety << "} " << endl;
            kills += 1;
            outputFile << endl;
        }
        return;
    }

    void fireclockwise(ofstream& outputFile) {
        int targetx = pos[0];
        int targety = pos[1];

        do {
            switch (rotation) {
                case 0:
                    targety += 1;
                    rotation++;
                    break;
                case 1:
                    targetx += 1;
                    targety += 1;
                    rotation++;
                    break;
                case 2:
                    targetx += 1;
                    rotation++;
                    break;
                case 3:
                    targetx += 1;
                    targety -= 1;
                    rotation++;
                    break;
                case 4:
                    targety += -1;
                    rotation++;
                    break;
                case 5:
                    targetx -= 1;
                    targety -= 1;
                    rotation++;
                    break;
                case 6:
                    targetx -= 1;
                    rotation++;
                    break;
                case 7:
                    targetx -= 1;
                    targety += 1;
                    rotation = 0;
                    break;
                default:
                    break;
            }
            if (targetx < 0 || targetx >= gameMap->getWidth() || targety < 0 || targety >= gameMap->getHeight()) {
                targetx = pos[0];
                targety = pos[1];
                continue;
            }
            break;
        } while (1);
        outputFile << name << " has shot {" << targetx << ", " << targety << "} " << endl;
        if (gameMap->checkoccupied(targetx, targety)) {
            gameMap->destroyElem(targetx, targety);
            gameMap->refresh();
            outputFile << name << " has killed the robot at {" << targetx << ", " << targety << "} " << endl;
            kills += 1;
        }
    }

    void firesurround(ofstream& outputFile) {
        int random;
        int count = 0;
        int targetx;
        int targety;
        random = rand() % 8;
        for (int i = pos[0] - 1; i <= pos[0] + 1; i++) {
            for (int j = pos[1] - 1; j <= pos[1] + 1; j++) {
                if (j < 0 || j >= gameMap->getHeight() || i < 0 || i >= gameMap->getWidth()) {
                    continue;
                } else if (i == pos[0] && j == pos[1]) {
                    continue;
                } else {
                    if (count == random) {
                        targetx = i;
                        targety = j;
                        outputFile << name << " shoot at " << targetx << "," << targety << endl;
                        if (gameMap->checkoccupied(targetx, targety)) {
                            gameMap->destroyElem(targetx, targety);
                            gameMap->refresh();
                            outputFile << name << " has killed the robot at {" << targetx << ", " << targety << "} " << endl;
                            kills += 1;
                        }
                        break;
                    }
                }                        
            }
        }
    }
};

// Subclass for robots that can move
class MovingRobot : virtual public Robot {
public:
    MovingRobot(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i, outputFile) {}

    void operate() override {
        int targetx = pos[0];
        int targety = pos[1];
        int moves;
        do {
            moves = rand() % 8;
            switch (moves) {
                case 0:
                    targety++; // Up
                    break;
                case 1:
                    targetx--; // Left
                    break;
                case 2:
                    targety--; // Down
                    break;
                case 3:
                    targetx++; // Right
                    break;
                case 4:
                    targetx--; // Up left
                    targety++;
                    break;
                case 5:
                    targetx++; // Up right
                    targety++;
                    break;
                case 6:
                    targetx--; // Down left
                    targety--;
                    break;
                case 7:
                    targetx++; // Down right
                    targety--;
                    break;
                default:
                    break;
            }
            if (targetx < 0 || targetx >= gameMap->getWidth() || targety < 0 || targety >= gameMap->getHeight() || gameMap->checkoccupied(targetx, targety) == true) {
                targetx = pos[0];
                targety = pos[1];
                continue;
            }
            break;
        } while (1);
        detectPos[0] = targetx;
        detectPos[1] = targety;
        pos[0] = targetx;
        pos[1] = targety;
        outputFile <<name << " Move to - " << pos[0] << ", " << pos[1] << endl;
        gameMap->changecoords(index, pos);
    }
};

class SteppingRobot : virtual public Robot {
public:
    SteppingRobot(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i, outputFile) {}

    void operate() override {
        if (detect) {
            gameMap->destroyElem(detectPos[0], detectPos[1]);
            outputFile << name << " has terminated robot at " << detectPos[0] << "," << detectPos[1] << endl;
            kills += 1;
            gameMap->changecoords(index, detectPos);
            pos[0] = detectPos[0];
            pos[1] = detectPos[1];
            detect = 0;
            gameMap->refresh();    
        } else {
            int targetx = pos[0];
            int targety = pos[1];
            int moves;
            do {
                moves = rand() % 8;
                switch (moves) {
                    case 0:
                        targety++; // Up
                        break;
                    case 1:
                        targetx--; // Left
                        break;
                    case 2:
                        targety--; // Down
                        break;
                    case 3:
                        targetx++; // Right
                        break;
                    case 4:
                        targetx--; // Up left
                        targety++;
                        break;
                    case 5:
                        targetx++; // Up right
                        targety++;
                        break;
                    case 6:
                        targetx--; // Down left
                        targety--;
                        break;
                    case 7:
                        targetx++; // Down right
                        targety--;
                        break;
                    default:
                        break;
                }
                if (targetx < 0 || targetx >= gameMap->getWidth() || targety < 0 || targety >= gameMap->getHeight() || gameMap->checkoccupied(targetx, targety) == true) {
                    targetx = pos[0];
                    targety = pos[1];
                    continue;
                }
                break;
            } while (1);
            detectPos[0] = targetx;
            detectPos[1] = targety;
            pos[0] = targetx;
            pos[1] = targety;
            outputFile << name << "Move to -  " << pos[0] << ", " << pos[1] << endl;
            gameMap->changecoords(index, pos);
        }
    }
};

// Example of a robot that can look once then shoot
class Robocop : public MovingRobot, public ShootingRobot {
public:
    Robocop(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) 
        : Robot(field, name, x, y, i,outputFile), ShootingRobot(field, name, x, y, i,outputFile), MovingRobot(field, name, x, y, i,outputFile) {}

    void operate() override {
        MovingRobot::operate();
        ShootingRobot::operate();
        ShootingRobot::operate();
        ShootingRobot::operate();
    }
};

class Terminator : public SeeingRobot, public SteppingRobot {
    int count;

public:
    Terminator(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i,outputFile), SeeingRobot(field, name, x, y, i,outputFile), SteppingRobot(field, name, x, y, i,outputFile), count(0) {}

    void operate() override {
        SeeingRobot::operate();
        SteppingRobot::operate();
    }
};

class BlueThunder : public ShootingRobot {
    int count;

public:
    BlueThunder(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i,outputFile), ShootingRobot(field, name, x, y, i,outputFile), count(0) {}

    void operate() override {
        ShootingRobot::fireclockwise(outputFile);
    }
};

class TerminatorRoboCop : public ShootingRobot, public SeeingRobot, public SteppingRobot {
public:
    TerminatorRoboCop(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i,outputFile), ShootingRobot(field, name, x, y, i,outputFile), SeeingRobot(field, name, x, y, i, outputFile), SteppingRobot(field, name, x, y, i,outputFile) {}

    void operate() override {
        SeeingRobot::operate();
        SteppingRobot::operate();
        ShootingRobot::operate();
        ShootingRobot::operate();
        ShootingRobot::operate();
    }
};

class Madbot : public ShootingRobot {
public:
    Madbot(battlefield* field, string name, int x, int y, int i, ofstream& outputFile) : Robot(field, name, x, y, i, outputFile), ShootingRobot(field, name, x, y, i,outputFile) {}

    void operate() override {
        ShootingRobot::firesurround(outputFile);  
    }
};

class RoboTank : public ShootingRobot {
public:
    RoboTank(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i, outputFile), ShootingRobot(field, name, x, y, i, outputFile) {}

    void operate() override {
        ShootingRobot::operate();
    }
};

class UltimateRobot : public ShootingRobot, public SeeingRobot, public SteppingRobot {
public:
    UltimateRobot(battlefield* field, string name, int x, int y, int i,ofstream& outputFile) : Robot(field, name, x, y, i,outputFile), ShootingRobot(field, name, x, y, i, outputFile), SeeingRobot(field, name, x, y, i,outputFile), SteppingRobot(field, name, x, y, i,outputFile) {}

    void operate() override {
        SeeingRobot::operate();
        SteppingRobot::operate();
        ShootingRobot::operate();
        ShootingRobot::operate();
        ShootingRobot::operate();
    
    }
};

class FrogRobot : public SteppingRobot {
public:
    FrogRobot(battlefield* field, string name, int x, int y, int i,ofstream& outputFile)
        : Robot(field, name, x, y, i,outputFile), SteppingRobot(field, name, x, y, i, outputFile) {}

    void operate() override {
        int jumpX = pos[0] + (rand() % 3 - 1); // Random jump in x direction (-1, 0, or 1)
        int jumpY = pos[1] + (rand() % 3 - 1); // Random jump in y direction (-1, 0, or 1)
        step(jumpX, jumpY, outputFile);
    }

    void step(int jumpX, int jumpY,ofstream& outputFile) {
        if (jumpX >= 0 && jumpX < gameMap->getWidth() && jumpY >= 0 && jumpY < gameMap->getHeight()) {
            gameMap->changecoords(index, pos);  // Update the gameMap coordinates
            pos[0] = jumpX;
            pos[1] = jumpY;

            if (gameMap->checkoccupied(jumpX, jumpY)) {
                gameMap->destroyElem(jumpX, jumpY);
                kills += 1;
                outputFile << name << " jumped to (" << jumpX << "," << jumpY << "). " << "It stepped on a robot and now has " << kills << " kills." << endl;
            } else {
                outputFile << "No robot at (" << jumpX << "," << jumpY << ")" ;
            }
        }
    }
};

#endif
