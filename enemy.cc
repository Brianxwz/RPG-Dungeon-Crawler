
#include "enemy.h"
#include "player.h"
#include <memory>
#include <cstdlib>
#include <cmath>
#include <sstream>

using namespace std;


Enemy::Enemy(int HP,int Atk,int Def,char sym,std::string race,int row,int col,int maxHP,int goldamt,bool ishostile):Character{HP,Atk,Def,sym,race,row,col,maxHP},goldamt{goldamt},ishostile{ishostile}{}

Enemy::~Enemy(){}

int Enemy::getamount(){
    return goldamt;
}

bool Enemy::gethostile(){
    return ishostile;
}

void Enemy::sethostile(bool hostile){
    ishostile=hostile;
}

void Enemy::attack(shared_ptr<Player> p){
    if(ishostile){ // if the Enemy is hostile
        int possibility = rand() % 2;
        if(possibility == 0){ // 0, then the enemy missed
            p->addAction(getRace() +" missed the target.");
            
        }
        else{ // possibility == 1;
            int Damage = (int) ceil((100 * getAtk())/(100 + p->getDef()));
            if(p->getBarrier()){
                Damage = ceil(Damage / 2.0);
            }
            ostringstream ss;
            ss << Damage;
            int newHP = p->getHP()-Damage;
            if(newHP>0){
                p->setHP(newHP);
                p->addAction(getRace() +" deals " + ss.str() +" damage to PC."); 
            }
            else{
                p->setHP(0); // PC dies.???
            }
        }
    }
}

void Enemy::moveEnemy(){ // move randomly CHECK VALID!!! // enemy can stay the same
    int possibility = rand() % 9;
    if(possibility == 0){  //0 for north
        addRow(-1);
    }
    else if(possibility == 1){  //1 for south
        addRow(1);
    }
    else if(possibility == 2){  //2 for east
        addCol(1);
    }
    else if(possibility == 3){  //3 for west
        addCol(-1);
    }
    else if(possibility == 4){  //4 for northeast
        addRow(-1);
        addCol(1);
    }
    else if(possibility == 5){  //5 for southeast
        addRow(1);
        addCol(1);
    }
    else if(possibility == 6){  //6 for southwest
        addRow(1);
        addCol(-1);
    }
    else if(possibility == 7){  //7 for northwest
        addRow(-1);
        addCol(-1);
    }
    else {}  //(possibility == 8)  //8 for stay the same
}

void Enemy::setHoardPickable() {}

std::shared_ptr<Gold> Enemy::getHoard() {
    return nullptr;
}
 































