//NURAFRINA BATRISYIA BINTI NORDZAMAN 123130337 TT3L

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Robot.h"
#include "Mapping.h"
#include "Battlefield.h"

using namespace std;

void endgame(ofstream& outputFile, string name) {
    outputFile << "*********************************************************************" << std::endl;
    outputFile << "*                                                                   *" << std::endl;
    outputFile << "*                      -------------------------                    *" << std::endl;
    outputFile << "*                      |        THE END        |                    *" << std::endl;
    outputFile << "*                      -------------------------                    *" << std::endl;
    outputFile << "*                                                                   *" << std::endl;
    outputFile << "*                       THE WINNER IS: " << name << "!                        *" << std::endl;
    outputFile << "*                                                                   *" << std::endl;
    outputFile << "*********************************************************************" << std::endl;
    exit(0);
}

void welcome(ofstream& outputFile, battlefield& game) {
    outputFile << " _____       _           _     __        __         _            " << std::endl;
    outputFile << "|  __ \\     | |         | |    \\ \\      / /        | |           " << std::endl;
    outputFile << "| |__) |__ _| |__   ___ | | __  \\ \\ /\\ / /__  _   _| |_ ___      " << std::endl;
    outputFile << "|  _  // _` | '_ \\ / _ \\| |/ /   \\ V  V / _ \\| | | | __/ _ \\     " << std::endl;
    outputFile << "| | \\ \\ (_| | |_) | (_) |   <     \\_/\\_/ (_) | |_| | ||  __/     " << std::endl;
    outputFile << "|_|  \\_\\__,_|_.__/ \\___/|_|\\_\\               |_|\\__,_|\\__\\___|   " << std::endl;


outputFile << "*********************************************************" << endl;
outputFile << "*               WELCOME TO ROBOT WAR GAME!               *" << endl;
outputFile << "*********************************************************" << endl << endl;
outputFile << "Prepare for epic clashes, upgrades, and surprises!" << endl << endl;
outputFile << "             Let the battle begin!" << endl << endl;
outputFile << "**********************************************************" << endl;
outputFile << "*                INITIALIZED ROBOTS                      *" << endl;
outputFile << "**********************************************************" << endl;


    for (int i = 0; i < game.get_elem_list_size(); ++i) {
    outputFile << "+-------------------------------------------------------+" << endl;
    outputFile << "| Robot " << i + 1 << ":                                              |" << endl;
    outputFile << "| Type: " << game.elem_list[i] << "                                               |" << endl;
    outputFile << "| Position: (" << game.getX(i) << ", " << game.getY(i) << ")                                      |" << endl;
    outputFile << "+-------------------------------------------------------+" << endl;
}
}

int main() {
    battlefield game;

    ifstream inputFile("Input.txt");

    if (!inputFile.is_open()) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

     ofstream outputFile("output.txt");

    if (!outputFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    game.accessfile(inputFile);
    int gc = 0;
    int buttonpressed = 0;
    int robotsleft = game.get_elem_list_size();

    int count = 0;
    Robot* robot_list[game.get_elem_list_size()] = {};

    for (int i = 0; i < game.get_elem_list_size(); i++) {
       if (game.elem_list[i] == 'R')
			{
				robot_list[i] = new Robocop(&game, "Robocop", game.getX(i), game.getY(i), i,outputFile);
			}
			else if (game.elem_list[i] == 'T')
			{	
				robot_list[i] = new Terminator(&game, "Terminator", game.getX(i), game.getY(i), i,outputFile);
			}
			else if (game.elem_list[i] == 'B')
			{
				robot_list[i] = new BlueThunder(&game, "BlueThunder", game.getX(i), game.getY(i), i,outputFile);
			} 
			else if (game.elem_list[i] == 'A')
			{	
				robot_list[i] = new TerminatorRoboCop(&game, "TerminatorRoboCop", game.getX(i), game.getY(i), i,outputFile);
			}
			else if (game.elem_list[i] == 'M')
			{
				robot_list[i] = new Madbot(&game, "Madbot", game.getX(i), game.getY(i), i,outputFile);
			} 
			else if (game.elem_list[i] == 'E')
			{	
				robot_list[i] = new RoboTank(&game, "RoboTank", game.getX(i), game.getY(i), i,outputFile);
			}
			else if (game.elem_list[i] == 'U')
			{
				robot_list[i] = new UltimateRobot(&game, "UltimateRobot", game.getX(i), game.getY(i), i,outputFile);
			}
            else if (game.elem_list[i] == 'F')
			{
				robot_list[i] = new FrogRobot(&game, "FrogRobot", game.getX(i), game.getY(i), i,outputFile);
			} 
    }

    welcome(outputFile,game);
    outputFile << "Finished instantiating robots!\n\nOriginal Presets: \n";
    game.printmap(outputFile);
    while (1) {
        gc++;
        outputFile << endl << endl;
        outputFile << "===========================================================" << endl;
        outputFile << "                         Turn " << gc << endl ;
        outputFile << "===========================================================" << endl;

        for (int i = 0; i < game.get_elem_list_size(); i++) {
            if (game.checkrevive(i) == 1) {
                game.revive(i);
                if (game.checklives(i) == 0) {
                    robotsleft -= 1;
                }
                if (game.checklives(i) != 0) {
                    outputFile << game.elem_list[i] << " is revived at " << game.getX(i) << "," << game.getY(i) << "!" << endl;
                    outputFile << "Lives left: " << game.checklives(i) << endl << endl;
                    robot_list[i]->refreshpos(game.getX(i), game.getY(i));
                }
            }
        }

        if (robotsleft == 1) {
            for (int i = 0; i < game.get_elem_list_size(); i++) {
                if (game.checklives(i) != 0) {
                    endgame(outputFile, game.get_name(i));
                }
            }
        }

        robotsleft = game.get_elem_list_size();

        for (int i = 0; i < game.get_elem_list_size(); i++) {
            if (game.checkrevive(i) == 0) {
                robot_list[i]->operate();
                    //outputFile << "+--------------------------------------------------------+" << endl;
                    //outputFile << "|      " << robot_list[i]->showname() << " has become " << robot_list[i]->showname()<<endl;
                    //outputFile << "+--------------------------------------------------------+" << endl;
                if (robot_list[i]->checkkills() >= 3) {  //to upgrade the robot 
                    //outputFile << robot_list[i]->showname();
                    if (game.elem_list[i] == 'R' || game.elem_list[i] == 'T') {
                        game.change_elem_list(i, 'A');
                        
                        robot_list[i] = new TerminatorRoboCop(&game, "TerminatorRoboCop", game.getX(i), game.getY(i), i,outputFile);
                        if (game.elem_list[i] == 'R'){
                           outputFile << "RoboCop has become " << robot_list[i]->showname()<<endl; 
                        }
                        else{
                            outputFile << "Terminator has become " << robot_list[i]->showname()<<endl;
                        }

                    } else if (game.elem_list[i] == 'A' || game.elem_list[i] == 'E') {
                        game.change_elem_list(i, 'U');
                        robot_list[i] = new UltimateRobot(&game, "UltimateRobot", game.getX(i), game.getY(i), i,outputFile);
                         if (game.elem_list[i] == 'A'){
                           outputFile << "TerminatorRobocop has become " << robot_list[i]->showname()<<endl; 
                        }
                        else{
                            outputFile << "Robotank has become " << robot_list[i]->showname()<<endl;
                        }

                    } else if (game.elem_list[i] == 'B') {
                        game.change_elem_list(i, 'M');
                        robot_list[i] = new Madbot(&game, "Madbot", game.getX(i), game.getY(i), i,outputFile);
                        outputFile << "BlueThunder has become " << robot_list[i]->showname()<<endl; 

                    } else if (game.elem_list[i] == 'M') {
                        game.change_elem_list(i, 'E');
                        robot_list[i] = new RoboTank(&game, "RoboTank", game.getX(i), game.getY(i), i,outputFile);
                        outputFile << "Madbot has become " << robot_list[i]->showname()<<endl; 

                    }else if (game.elem_list[i] == 'F') {
                        game.change_elem_list(i, 'M');
                        robot_list[i] = new Madbot(&game, "Madbot", game.getX(i), game.getY(i), i,outputFile);
                        outputFile << "FrogRobot has become " << robot_list[i]->showname()<<endl; 
                    }
                }
            }
        }
        game.printmap(outputFile);
    }
  
    inputFile.close();
    outputFile.close();
    outputFile.flush();
    return 0;
}