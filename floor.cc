#include "floor.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "character.h"
#include "chamber.h"
#include "enemy/dragon.h"
#include "enemy/goblin.h"
#include "enemy/vampire.h"
#include "enemy/merchant.h"
#include "enemy/phoenix.h"
#include "enemy/troll.h"
#include "enemy/werewolf.h"
#include "player/dwarf.h"
#include "player/elves.h"
#include "player/human.h"
#include "player/orc.h"
#include "item/gold.h"
#include "item/potion.h"

#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <utility>
#include <cstdlib>

using namespace std;


Floor::Floor(string filename):curFloor{1},Won{false},thePlayer{nullptr},numRow{25},numCol{79}{
	ifstream file {filename};
	string s;
	while (getline(file, s)) {
		vector <char> row;
		int len = (int)s.length();
		for (int i = 0; i < len; ++i) {
			row.emplace_back(s[i]);
		}
		theDisplay.emplace_back(row);
	}
	preTile = '.';
	barrierSpawn = (rand() % 5) + 1; // barrier spawns exactly once on a random floor
}

Floor::~Floor(){
	clearFloor();
}

bool Floor::isWon(){
	return Won;
}

void Floor::setWon() {
	Won = true;
}

int Floor::getCurFloor() {
	return curFloor;
}

void Floor::nextFloor() {
	++curFloor;
	ostringstream ss;
	ss << curFloor;
	thePlayer->setAction("Welcome to Floor " + ss.str() +"!");
}

int Floor::getnumRow() {
	return numRow;
}

int Floor::getnumCol() {
	return numCol;
}

int Floor::getgoldAmt() {
	return thePlayer->getAmt();
}

void Floor::setPlayer(std::shared_ptr<Player> p) {
	thePlayer = p;
}

void Floor::setFloor(){
	generateChamber();
	generatePlayer(); 
	showStair();
	generatePotion();
	generateGold();
	generateEnemy();
}

void Floor::clearFloor() {
	while((int)theEnemy.size() > 0){
		theDisplay[theEnemy[0]->getRow()][theEnemy[0]->getCol()] = '.';
		theEnemy.erase(theEnemy.begin());
	}
	while((int)theChamber.size() > 0){
		theChamber.erase(theChamber.begin());
	}
	while((int)thePotion.size() > 0){
		theDisplay[thePotion[0]->getRow()][thePotion[0]->getCol()] = '.';
		thePotion.erase(thePotion.begin());
	}
	while((int)theGold.size() > 0){
		theDisplay[theGold[0]->getRow()][theGold[0]->getCol()] = '.';
		theGold.erase(theGold.begin());
	}
	for(int i = 0; i < numRow; i++){ // clear stair
		for(int j = 0; j < numCol ; ++j){
			if(theDisplay[i][j] == '\\') {
				theDisplay[i][j] = '.';
			}
			if(theDisplay[i][j] == 'C') {
				theDisplay[i][j] = '.';
			}
		}
	}
	theDisplay[thePlayer->getRow()][thePlayer->getCol()] = '.';
	thePlayer->setAtk(thePlayer->getdeAtk());
	thePlayer->setDef(thePlayer->getdeDef());
	thePlayer->setCompass(false);
}


void Floor::showStair() {
	int id = rand() % 5;
	while(id == thePlayer->getChamberId()) { 
		id = rand() % 5;
	}
	theChamber[id]->generatePos('\\');
}


void Floor::generateChamber() {
	for (int i = 1; i <= 5; ++i) {
		auto newChamber = make_shared <Chamber> (i, theDisplay);
		theChamber.emplace_back(newChamber);
	}
}

void Floor::generatePlayer() {
	int id = rand() % 5; 
	thePlayer->setChamberId(id);
	vector<int> cor = theChamber[id]->generatePos('@');
	thePlayer->setRow(cor.front());
	thePlayer->setCol(cor.back());
	
}

void Floor::generateEnemy() {
	int dragonamt = 0;

	bool barrierHasSpawned = (barrierSpawn == curFloor);
	for (int i = 0; i < 10 + barrierHasSpawned; ++i) {
		if (theGold[i]->getQuantity() == 6 || theGold[i]->getQuantity() == 0) {
			theGold[i]->setPickable(false);
			int currRow, currCol;
			++dragonamt;

			while (true) {
				currRow = theGold[i]->getRow();
				currCol = theGold[i]->getCol();

				int where = rand() % 8; //determine dragon's pos
				char currTile;
				
				if (where == 0) {
					--currRow;
					--currCol;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				if (where == 1) {
					--currRow;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				if (where == 2) {
					--currRow;
					++currCol;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				if (where == 3) {
					--currCol;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				if (where == 4) {
					++currCol;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				if (where == 5) {
					++currRow;
					--currCol;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				if (where == 6) {
					++currRow;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
				
				if (where == 7) {
					++currRow;
					++currCol;
					currTile = getCharAt(currRow, currCol);
					if (currTile == '.') {
						setCharAt(currRow, currCol, 'D');
						break;
					}
				}
			}
			auto newDragon = make_shared<Dragon> (currRow, currCol, theGold[i]);
			theEnemy.emplace_back(newDragon);
		}
	}
	
	int restamt = 20 - dragonamt;
	
	for (int i = 0; i < restamt; ++i) {
		int id = rand() % 5; //
		int type = rand() % 18; // 0-3 reps Werewolf, 4-6 reps Vampire, 7-11 reps Goblin, 12-13 reps Troll, 14-15 reps Phoenix, 16-17 reps Merchant
		if (type == 0 || type == 1 || type == 2 || type == 3) {
			vector<int> cor = theChamber[id]->generatePos('W');
			auto newEnemy = make_shared<Werewolf> (cor.front(), cor.back());
			theEnemy.emplace_back(newEnemy);
			
		} else if (type == 4 || type == 5 || type == 6) {
			vector<int> cor = theChamber[id]->generatePos('V');
			auto newEnemy = make_shared<Vampire>(cor.front(), cor.back());
			theEnemy.emplace_back(newEnemy);
			
		} else if (type == 7 || type == 8 || type == 9 || type == 10 || type == 11) {
			vector<int> cor = theChamber[id]->generatePos('N');
			auto newEnemy = make_shared<Goblin> (cor.front(), cor.back());
			theEnemy.emplace_back(newEnemy);
			
		} else if (type == 12 || type == 13) {
			vector<int> cor = theChamber[id]->generatePos('T');
			auto newEnemy = make_shared<Troll> (cor.front(), cor.back());
			theEnemy.emplace_back(newEnemy);
			
		} else if (type == 14 || type == 15) {
			vector<int> cor = theChamber[id]->generatePos('X');
			auto newEnemy = make_shared<Phoenix> (cor.front(), cor.back());
			theEnemy.emplace_back(newEnemy);
			
		} else {
			vector<int> cor = theChamber[id]->generatePos('M');
			auto newEnemy = make_shared<Merchant> (cor.front(), cor.back());
			theEnemy.emplace_back(newEnemy);
		}
	}
	int id = rand() % 20;
	theEnemy[id]->setCompass(true);
}



void Floor::generateGold() {
	int id,type;
	for (int i = 0; i < 10; i++) {
		id = rand() % 5; //
		vector<int> cor = theChamber[id]->generatePos('G');
		type = rand() % 8; // 0-4 reps normal, 5-6 reps small, 7 reps dragon gold
		if (type == 0 || type == 1 || type == 2 || type == 3 || type == 4) {
			auto newGold = make_shared<Gold> (cor.front(), cor.back(), "normal", 'G', 1, true);
			theGold.emplace_back(newGold);
		} else if (type == 5 || type == 6) {
			auto newGold = make_shared<Gold> (cor.front(), cor.back(),"small", 'G', 2, true);
			theGold.emplace_back(newGold);
		} else {
			auto newGold = make_shared<Gold> (cor.front(), cor.back(), "dragon hoard", 'G', 6, false);
			theGold.emplace_back(newGold);
		}
	}
	// gold with 0 value represents the barrier suit
	if (barrierSpawn == curFloor) {
		id = rand() % 5;
		vector<int> cor = theChamber[id]->generatePos('B');
		auto newGold = make_shared<Gold> (cor.front(), cor.back(), "barrier", 'B', 0, false);
		theGold.emplace_back(newGold);
	}
}


void Floor::generatePotion() {
	int id,type;
	for (int i = 0; i < 10; i++) {
		id = rand() % 5;//
		vector<int> cor = theChamber[id]->generatePos('P');
		type = rand() % 6; // 0 reps RH, 1 reps BA, 2 reps BD, 3 reps PH, 4 reps WA, 5 reps WD
		if (type == 0) {
			auto newPotion = make_shared<Potion> (cor.front(), cor.back(),"RH", 'P', 10);
			thePotion.emplace_back(newPotion);
		} else if (type == 1) {
			auto newPotion = make_shared<Potion> (cor.front(), cor.back(),"BA", 'P', 5);
			thePotion.emplace_back(newPotion);
		} else if (type == 2) {
			auto newPotion = make_shared<Potion> (cor.front(), cor.back(),"BD", 'P', 5);
			thePotion.emplace_back(newPotion);
		} else if (type == 3) {
			auto newPotion = make_shared<Potion> (cor.front(), cor.back(),"PH", 'P', -10);
			thePotion.emplace_back(newPotion);
		} else if (type == 4) {
			auto newPotion = make_shared<Potion> (cor.front(), cor.back(),"WA", 'P', -5);
			thePotion.emplace_back(newPotion);
		} else {
			auto newPotion = make_shared<Potion> (cor.front(), cor.back(),"WD", 'P', -5);
			thePotion.emplace_back(newPotion);
		}
	}

}

void Floor::printFloor(){
	for (int i = 0; i < numRow; ++i) {
		for (int j = 0; j < numCol; ++j) {
			// print gold
			if(theDisplay[i][j]=='G'){
				cout << "\033[1;33m";
				cout << theDisplay[i][j];
				cout << "\033[0m";
			}
			// print potions
			else if(theDisplay[i][j]=='P'){
				cout << "\033[1;32m";
				cout << theDisplay[i][j];
				cout << "\033[0m";
			}
			// player
			else if(theDisplay[i][j]=='@'){
				// player with barrier is darker colour
				if (thePlayer->getBarrier()){
					cout << "\033[1;34m";
				} else {
					cout << "\033[1;96m";
				}
				cout << theDisplay[i][j];
				cout << "\033[0m";
			}
			// hostile enemies are red and non hostile are grey
			else if(theDisplay[i][j]=='M'||theDisplay[i][j]=='V'||theDisplay[i][j]=='W'||theDisplay[i][j]=='D'||theDisplay[i][j]=='T'||theDisplay[i][j]=='N'||theDisplay[i][j]=='X'){
				if (theEnemy[findEnemy(i, j)]->gethostile()) {
					cout << "\033[31m";
					cout << theDisplay[i][j];
					cout << "\033[0m";
				} else {
					cout << "\033[90m";
					cout << theDisplay[i][j];
					cout << "\033[0m";
				}
			}
			else if(theDisplay[i][j]=='#'||theDisplay[i][j]=='+'||theDisplay[i][j]=='-'||theDisplay[i][j]=='|'){
				cout << "\033[1;35m";
				cout << theDisplay[i][j];
				cout << "\033[0m";
			}
			// stairs only appear when map is found
			else if (theDisplay[i][j]=='\\'){
				if(!thePlayer->getCompass()){
					cout << "\033[1;37m";
					cout << ".";
					cout << "\033[0m";
				} else {
					cout << "\033[7;37m";
					cout << theDisplay[i][j];
					cout << "\033[0m";
				}
			}
			// barrier suit
			else if (theDisplay[i][j]=='B'){
				cout << "\033[1;34m";
				cout << theDisplay[i][j];
				cout << "\033[0m";
			}
			else {
				cout << "\033[1;37m";
				cout << theDisplay[i][j];
				cout << "\033[0m";
			}
		}
		cout << "\n\033[0m";
	}

	cout << "Race: " << thePlayer->getRace() << " Gold: " << thePlayer->getAmt();
	for (int i = 0; i < 50; ++i) {
		cout << " ";
	}
	cout << "Floor " << curFloor<<endl;
	cout << "HP: " << thePlayer->getHP() << endl;
	cout << "Atk: " << thePlayer->getAtk() << endl;
	cout << "Def: " << thePlayer->getDef() << endl;
	cout << "Action: " << thePlayer->getAction() << endl; 
}

// find index of potion in potion array
int Floor::findPotion(const int row, const int col) {
	for (int i = 0; i < (int)thePotion.size(); ++i) {
		if (thePotion[i]->getRow() == row && thePotion[i]->getCol() == col) return i;
	}
	return -1; 
}

// same as above but for enemies
int Floor::findEnemy(const int row, const int col) {
	for (int i = 0; i < (int)theEnemy.size(); ++i) {
		if (theEnemy[i]->getRow() == row && theEnemy[i]->getCol() == col) return i;
	}
	return -1; 
}


void Floor::consumePotion(string direction) {
	vector<int> pos = nextStep(direction);
	int ind = findPotion(pos.front(), pos.back());
	thePlayer->usePotion(thePotion[ind]);
	thePotion.erase(thePotion.begin()+ind);
	theDisplay[pos.front()][pos.back()] = '.';
}

void Floor::movePlayer(string direction){
	int prow = thePlayer->getRow(); //current row of player
	int pcol = thePlayer->getCol(); //current col of player
	vector<int> prePos = nextStep(direction);
	char temp = theDisplay[prePos.front()][prePos.back()]; // next char
	bool isItem = false;
	string tempAction;
	if (temp == 'G' || temp == 'B') {  // next step is Gold
		int ind;
		for (ind = 0; ind < (int)theGold.size(); ++ind) {
			if (theGold[ind]->getRow() == prePos.front() && theGold[ind]->getCol() == prePos.back()){
				break;
			}
		}
		thePlayer->useGold(theGold[ind]);
		theGold.erase(theGold.begin()+ind);
		temp = '.';
		isItem = true;
		tempAction = thePlayer->getAction();
	}

	if (temp == 'C') { // next step is Compass
		thePlayer->setCompass(true);
		temp = '.';
		isItem = true;
		tempAction = "PC has found the Compass, the staircase is now visible.";
	}
	
	theDisplay[prow][pcol] = preTile;
	thePlayer->movePos(direction);
	if (isItem) {
		thePlayer->setAction(thePlayer->getAction() + ' ' + tempAction);
	}
	theDisplay[thePlayer->getRow()][thePlayer->getCol()] = '@';
	preTile = temp;
}

bool Floor::isValid(string direction){
	return(checkPoint(direction,'.')||checkPoint(direction, '+') || checkPoint(direction, 'C')
	||checkPoint(direction, '#')||( checkPoint(direction, 'G') && isPickableGold(direction))
	||( checkPoint(direction, 'B') && isPickableGold(direction))); // only include pickable gold
}

bool Floor::isPickableGold(string direction){ // check if the next gold is pickable
	vector <int> ns = nextStep(direction);
	int i;
	for (i = 0; i < (int)theGold.size(); i++) {
		if (theGold[i]->getRow() == ns.front() && theGold[i]->getCol() == ns.back()) {
			break;
		}
	}
	return theGold[i]->isPickable();
}

bool Floor::checkPoint(string direction, char c) {
	vector<int> pos = nextStep(direction);
	if (c == theDisplay[pos.front()][pos.back()]) return true;
	return false;
}

void Floor::setCharAt(const int row, const int col, const char symbol){
	theDisplay[row][col]=symbol;
}

char Floor::getCharAt(const int row, const int col){
	return theDisplay[row][col];
}

vector<int> Floor::nextStep(string direction) { 
	int prow = thePlayer->getRow();
	int pcol = thePlayer->getCol();
	vector<int> pos;
	if (direction == "no") {
		--prow;
	} else if (direction == "so") {
		++prow;
	} else if (direction == "ea") {
		++pcol;
	} else if (direction == "we") {
		--pcol;
	} else if (direction == "ne") {
		--prow;
		++pcol;
	} else if (direction == "se") {
		++prow;
		++pcol;
	} else if (direction == "sw") {
		++prow;
		--pcol;
	} else{
		--prow;
		--pcol;    
	}
	pos.emplace_back(prow);
	pos.emplace_back(pcol);
	return pos;
}


void Floor::enemyMoveAndAttack() {
	// enemies move and attack every turn in a line by line fashion
	
	int enRow,enCol;  //row, col of enemy
	int pRow = thePlayer->getRow();
	int pCol = thePlayer->getCol();

	vector <shared_ptr<Enemy>> enemies;
	
	for (int i = 0; i < numRow; ++i) {  //line by line fashion
		for (int j = 0; j < numCol; ++j) {
			int enemyThere = findEnemy(i, j);
			if (enemyThere > -1) {
				enemies.emplace_back(theEnemy[enemyThere]);
			}
		}
	}

	for (int i = 0; i < (int)enemies.size(); ++i) {
		enRow = enemies[i]->getRow();
		enCol = enemies[i]->getCol();
		
		if (enemies[i]->getRace() == "Dragon") {
			shared_ptr<Gold> tempGold = enemies[i]->getHoard();
			enRow = tempGold->getRow();
			enCol = tempGold->getCol();
			// if beside hoard, dragon is hostile
			if ((enRow-1 <= pRow && enRow+1 >= pRow) && (enCol-1 <= pCol && enCol+1 >= pCol)) {
				enemies[i]->sethostile(true);
				cout<< "\033[1;31mYou are near the dragon hoard. Dragon is now hostile.\033[0m"<<endl;
			} else { // else dragon not hostile
				enemies[i]->sethostile(false);
			}
			enRow = enemies[i]->getRow();
			enCol = enemies[i]->getCol();
		}

		if (!enemies[i]->gethostile()){ //deal with merchant
			
			if (enemies[i]->getRace() == "Merchant") { // merchant who is not hostile and can only move
				theDisplay[enRow][enCol] = '.';
				
				while (true){
					enemies[i]->setRow(enRow);
					enemies[i]->setCol(enCol);
					enemies[i]->moveEnemy();
					if (theDisplay[enemies[i]->getRow()][enemies[i]->getCol()] == '.') break;
				}
				theDisplay[enemies[i]->getRow()][enemies[i]->getCol()]
				= enemies[i]->getSym();
			}
			
		}
		else  {  //deal with the rest of enemies and hostile merchant and dragon
			if ((enRow-1 <= pRow && enRow+1 >= pRow) && (enCol-1 <= pCol && enCol+1 >= pCol)) {
				enemies[i]->attack(thePlayer);
			} else { 
				if (enemies[i]->getRace() != "Dragon"){  // hostile dragon is not included, dragon never moves
					theDisplay[enRow][enCol] = '.';
					
					while (true){
						enemies[i]->setRow(enRow);
						enemies[i]->setCol(enCol);
						enemies[i]->moveEnemy();
						if (theDisplay[enemies[i]->getRow()][enemies[i]->getCol()] == '.') break;
					}
					theDisplay[enemies[i]->getRow()][enemies[i]->getCol()]
					= enemies[i]->getSym();
				}
			}
		}  
	}
}


void Floor::playerAttack(string direction) {
	vector<int> pos = nextStep(direction);
	int ind = findEnemy(pos.front(), pos.back());
	thePlayer->attack(theEnemy[ind]);
	if (theEnemy[ind]->getHP() <= 0) {
		if (theEnemy[ind]->getCompass()) {
			theDisplay[theEnemy[ind]->getRow()][theEnemy[ind]->getCol()] = 'C';
		} else {
			theDisplay[theEnemy[ind]->getRow()][theEnemy[ind]->getCol()] = '.';
		}
		theEnemy.erase(theEnemy.begin()+ind); 
	}
}

bool Floor::checkdie(){
	if(thePlayer->getHP() > 0) return false;
	return true;
}





