#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "floor.h"
#include "chamber.h"
#include "character.h"
#include "player.h"
#include "player/human.h"
#include "player/dwarf.h"
#include "player/elves.h"
#include "player/orc.h"
#include <ctime>

using namespace std; 

int main() {
	srand(time(NULL));

	cout << "\033[1;33mWelcome to Dungeon Crawl!\033[0m" << endl;
	string s2;
	ifstream welcome{"welcome.txt"};
	while(getline(welcome,s2)){
		cout << "\033[1;35m";
		cout << s2 << endl;
		cout << "\033[0m";
	}



	while (true) {
		string command;
		
		// create player
		cout << "Please choose your character." << endl;
		cout << "\033[1;31mh\033[0m for human." << endl;
		cout << "\033[1;31md\033[0m for dwarf." << endl;
		cout << "\033[1;31me\033[0m for elves." << endl;
		cout << "\033[1;31mo\033[0m for orc." << endl;
		
		auto floor = make_shared<Floor>();
		string race;    
		cin >> command;
		while(true) {
			if (command == "h") {
				auto newPlayer = make_shared<Human> (-1, -1, -1);
				floor->setPlayer(newPlayer);
				break;
			} else if (command == "d") {
				auto newPlayer = make_shared<Dwarf> (-1, -1, -1);
				floor->setPlayer(newPlayer);
				break;
			} else if (command == "e") {
				auto newPlayer = make_shared<Elves> (-1, -1, -1);
				floor->setPlayer(newPlayer);
				break;
			} else if (command == "o") {
				auto newPlayer = make_shared<Orc> (-1, -1, -1);
				floor->setPlayer(newPlayer);
				break;
			}
			else {
				// not a valid character
				cout << "\033[1;31mPlease choose a valid player.\033[0m" << endl;
				cin>>command;
			}
		}

		race = command;

		// create floor
		floor->setFloor();
		floor->printFloor();

		// game begins
		cout << "Please enter command:" << endl;
		cout << "Directions: no, so, ea, we, ne, se, sw, nw" << endl;
		cout << "Use Potion: u <direction>" << endl;
		cout << "Attack: a <direction>" << endl;
		cout << "Restart: r" << endl;
		cout << "Quit: q" << endl;
		// check for command
		while (cin>>command) {
			if (command == "no" ||command ==  "so" ||command ==  "ea"
					||command ==  "we" ||command ==  "ne" ||command ==  "se" ||command ==  "sw" ||command ==  "nw") { // read direction
				
				// check startcase
				if (floor->checkPoint(command, '\\')) {
					// check if game is won
					if (floor->getCurFloor() == 5){
						floor->setWon();
						cout << "\033[1;33mCongratulations! You win the game!\033[0m" << endl;
						string s4;
						ifstream win{"win.txt"};
						while(getline(win,s4)){
							cout << "\033[1;33m";
							cout << s4 << endl;
							cout << "\033[0m";
						}

						// print final scores
						if(race == "d"){
							cout << "\033[1;36mYour final score is:\033[0m " << floor->getgoldAmt() * 2 << endl;
						}
						else if(race == "o"){
							cout << "\033[1;36mYour final score is:\033[0m " << floor->getgoldAmt()/2 << endl;
						}
						else{
							cout << "\033[1;36mYour final score is:\033[0m " << floor->getgoldAmt() << endl;
						}
						break;   
					} else {
						floor->nextFloor();
						floor->clearFloor();       
						floor->setFloor();
						floor->printFloor();
					}
				}

				// if not moving to staircase
				else if (floor->isValid(command)) {
					floor->movePlayer(command);
					floor->enemyMoveAndAttack();
					if(floor->checkdie()) break;// PC dies
					floor->printFloor();
				} else {
					if (floor->checkPoint(command, 'G')) {
						cout<<"\033[1;31mThe gold is guarded by the Dragon. Kill it to pick it up.\033[0m" << endl;
					} else if (floor->checkPoint(command, 'B')) {
						cout<<"\033[1;31mThe barrier suit is guarded by the Dragon. Kill it to pick it up.\033[0m" << endl;
					} else {
						cout<<"\033[1;31mThere is no way to go.\033[0m"<<endl;
					}
				}
			}

			else if (command == "u") {  // use Potion
				cin >> command;  // read direction
				if (floor->checkPoint(command, 'P')) { //checkPotion checks if there's a potion on the direction command
					floor->consumePotion(command);  // usePotion is in Player class
					if(floor->checkdie()) break;
					floor->enemyMoveAndAttack();
					if(floor->checkdie()) break;// PC dies
					floor->printFloor();
				} else {
					cout << "\033[1;31mThere's nothing there.\033[0m" << endl;
				}
			}
			
			else if (command == "a") {  // attack enemy
				cin >> command;  // read direction
				if (floor->checkPoint(command, 'V') || floor->checkPoint(command, 'W') || floor->checkPoint(command, 'N') ||
						floor->checkPoint(command, 'M') || floor->checkPoint(command, 'D') || floor->checkPoint(command, 'X') ||
						floor->checkPoint(command, 'T')) {  // checks if there's enemy on the direction command
					floor->playerAttack(command);  // attack is in player class
					floor->enemyMoveAndAttack();
					if(floor->checkdie()) break;// PC dies
					floor->printFloor();
				} else {
					cout << "\033[1;31mThere's no enemy in the direction you pointed.\033[0m" << endl;
				}
			}
			
			else if (command == "r") {
				floor->clearFloor(); // reset the game, means clearing and deleting the floor including unique ptrs, in floor class
				break;
			} else if (command == "q") {
				break;
			} else {
				cout << "\033[1;31mPlease enter a valid command.\033[0m" << endl;
			}
		}
		
		
		
		if (command == "q") {
			cout << "\033[1;33mThank you for playing Dungeon Crawl. Have a good day!\033[0m" << endl;
			string s5;
			ifstream byebye{"byebye.txt"};
			while(getline(byebye,s5)){
				cout << "\033[1;35m";
				cout << s5 << endl;
				cout << "\033[0m";
			}
			break;

		}

		if(floor->checkdie()){
			string s3;
			ifstream lose{"lose.txt"};
			while(getline(lose,s3)){
				cout << "\033[1;34m";
				cout << s3 << endl;
				cout << "\033[0m";
			}

		}

		if (floor->isWon() || floor->checkdie()) {
			string c;
			cout << "\033[1;33mDo you want to play the game again?\033[0m y/n" << endl;
			while(cin >> c) {
				if (c == "n") {
					cout << "\033[1;33mThank you for playing Dungeon Crawl. Have a good day!\033[0m" << endl;
					string s6;
					ifstream byebye{"byebye.txt"};
					while(getline(byebye,s6)){
						cout << "\033[1;35m";
						cout << s6 << endl;
						cout << "\033[0m";
					}
					break;
				} else if (c == "y") {
					break;
				} else {
					cout << "\033[1;31mPlease enter a valid command.\033[0m y(es)/n(o)" << endl;
				}
			}

			if(c=="n"){
				break;
			}
		}
		
		
	}
}





