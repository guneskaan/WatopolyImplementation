#include <cstdlib>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "board.h"
#include "gym.h"
#include "academic.h"
#include "residences.h"
#include "player.h"

using namespace std;


Board::Board(bool load, bool test, string filename) {
    this->test = test;
    turn = doubleCount = rollCount = totalTims = 0;
    
    // useful variables
    int input;
    int cost;
    int position;
    int improveCost;
    int *tuition;
    int indicator;
    int improveCount;
    string name;
    string block;
    string owner;
    string skip;
    char piece;
    bool flag=true;
    const int defaultImprove = 0;
    string defaultOwner = "BANK";
    ifstream file("database.txt");
    
    
    if (load) {
        ifstream in(filename);
        
        // initialize player
        in >> numPlayer;
        for (int i = 0; i < numPlayer; ++i) {
            
            
            in >> name >> piece >> input;
            players.emplace_back(Player(name,piece));
            players.at(i).setTims(input);
            in >> input;
            players.at(i).setMoney(input);
            in >> position;
            if (position == 10) {
                in >> input;
                if (input) {
                    in >> input;
                    input = 3 - input;
                    players.at(i).setPrison(input);
                }
            }
            players.at(i).setPosition(position);
        }
        
        // initialize buildings
        for (int i = 0; i < 30; ++i) {
            file >> indicator;
            if (indicator == 0)  {
                file >> name >> cost >> position >> improveCost >> block;
                in >> skip >> owner >> improveCount;
                tuition = new int[6];
                for (int k = 0; k < 6; ++k) {
                    file >> tuition[k];
                }
                buildings.insert(pair<int, Building*>(position, new Academic(name, cost, position, improveCost, block, tuition, improveCount, owner)));
                for(Player &p:players){
                    if(p.getName()==owner){
                        p.attachBuilding(position);
                        break;
                    }
                }
            } else if (indicator == 1) {
                file >> name >> cost >> position;
                in >> skip >> owner >> improveCount;
                buildings.insert(pair<int, Building*>(position, new Gym(name, cost, position, owner)));
                for(Player &p:players){
                    if(p.getName()==owner){
                        p.addGym();
                        break;
                    }
                }
            } else if (indicator == 2) {
                file >> name >> input >> cost >> position;
                in >> skip >> owner >> improveCount;
                buildings.insert(pair<int, Building*>(position, new Residence(name, input, cost, position, owner)));
                for(Player &p:players){
                    if(p.getName()==owner){
                        p.addRes();
                        break;
                    }
                }
            }
        }
        
    } else {
        
        // initialize player
        bool playerFlag = false;
        while (!playerFlag) {
            cout << "How many players are there?" << endl;
            cin >> numPlayer;
            if (numPlayer < 2 || numPlayer > 6) {
                cout << "Please enter player # between 2 and 6 inclusive." << endl;
                playerFlag = false;
            } else {
                playerFlag = true;
            }
        }
        for (int i = 0; i < numPlayer; ++i) {
            cout << "Player " << i+1 << " name?" << endl;
            flag = true;
            while(flag){
                flag = false;
                cin >> name;
                
                for(auto &p:players){
                    if(p.getName()==name || name==defaultOwner){
                        
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    
                    cout<<"The name is not available. (name cannot be repeated or be equal to BANK)"<<endl;
                }
            }
            
            
            cout << "Player " << i+1 << " character?" << endl;
            flag = true;
            while(flag){
                flag = false;
                cin >> piece;
                
                for(auto &p:players){
                    if(p.getPiece()==piece){
                        
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    
                    cout<<"The piece is not unique"<<endl;
                }
            }
            
            players.emplace_back(Player(name, piece));
        }
        
        // initialize buildings
        for (int i = 0; i < 30; ++i) {
            file >> indicator;
            if (indicator == 0)  {
                file >> name >> cost >> position >> improveCost >> block;
                tuition = new int[6];
                for (int k = 0; k < 6; ++k) {
                    file >> tuition[k];
                }
                buildings.insert(pair<int, Building*>(position, new Academic(name, cost, position, improveCost, block, tuition, defaultImprove, defaultOwner)));
            } else if (indicator == 1) {
                file >> name >> cost >> position;
                buildings.insert(pair<int, Building*>(position, new Gym(name, cost, position, defaultOwner)));
            } else if (indicator == 2) {
                file >> name >> input >> cost >> position;
                buildings.insert(pair<int, Building*>(position, new Residence(name, input, cost, position, defaultOwner)));
            }
        }
    }
    startedPlayers = numPlayer;
    cout << "Done initialization!\n";
}


void Board::move(int dice1, int dice2) {
    if (dice1 > 0 && dice2 > 0)
        cout << "You rolled " << dice1 << ", " << dice2 << endl;
    int newPos = players.at(turn).getPosition() + dice1 + dice2;
    // moves player
    if(newPos >= 40){
        players.at(turn).setMoney(players.at(turn).getMoney() + 200);
        newPos-=40;
    }
    
    players.at(turn).setPosition(newPos);
    // cout << "You landed on " << buildings.find(newPos)->second->getName() << endl;
    // hardcode private function bool isBuilding()
    // if property
    if (isBuilding(newPos) && !buildings.find(newPos)->second->getMortgaged()) {
        cout << "You landed on " << buildings.find(newPos)->second->getName() << endl;
        if (buildings.find(newPos)->second->getOwner() == "BANK") {
            std::cout << "Would you like to buy " << buildings.find(newPos)->second->getName() <<
            " for $" << buildings.find(newPos)->second->getPrice() << "?" << std::endl;
            this->buy(newPos);
        } else {
            this->payRent(newPos);
        }
    } else {
        this->runNonProp(newPos);
    }
}

void Board::roll(int dice1, int dice2) {
    bool rolled = true;
    // checks testing mode
    if (!test) {
        std::cout << "Error: the game is not in testing mode." << std::endl;
        return;
    }
    
    if (rollCount == 1 || doubleCount > 2) {
        cout << "You already rolled" << endl;
        return;
    } else if (doubleCount == 2 && dice1 == dice2) {
        cout << "You rolled 3 doubles!" << endl;
        ++doubleCount;
        runNonProp(30);
        return;
    }
    ++rollCount;
    
    if (players.at(turn).getPrison()) {
        rolled = prisonRoll(dice1, dice2);
    } else if (dice1 == dice2) {
        cout << "You rolled a double! You can roll again!\n";
        ++doubleCount;
        rollCount = 0;
    }
    
    if (rolled) move(dice1, dice2);
}

void Board::trade(string offered, int given, string recieved){
    string input;
    int playerIndex;
    bool foundplayer=false;
    for(int i = 0; i < startedPlayers; ++i){
        if(players.at(i).getName()==offered && players.at(i).isAlive() && players.at(turn).getName()!=offered){
            playerIndex = i;
            foundplayer=true;
            break;
        }
    }
    if(!foundplayer){
        cout << "Invalid player name." << endl;
        return;
    }
    if(given < 0 || given > players.at(turn).getMoney()){
        cout << "Invalid money." << endl;
        return;
    }
    int i = getBuildingPos(recieved);
    if(i < 0 || buildings.find(i)->second->getOwner() != offered){
        cout << "Invalid building." << endl;
        return;
    }
    
    cout << "Player: " << players.at(turn).getName() << " wants to trade $" << given << " for " << recieved << "." << endl;
    while (true) {
        cout << "Say 'yes' or 'no'" << endl;
        cin >> input;
        if (input == "yes") {
            players.at(turn).setMoney(players.at(turn).getMoney() - given);
            players.at(turn).attachBuilding(i);
            players.at(playerIndex).detachBuilding(i);
            players.at(playerIndex).setMoney(players.at(playerIndex).getMoney() + given);
            buildings.find(i)->second->setOwner(players.at(turn).getName());
            break;
        } else if (input == "no") {
            break;
        }
    }
}

void Board::trade(string offered, string given, string recieved){
    string input;
    int playerIndex;
    bool foundplayer=false;
    for(int i = 0; i < startedPlayers; ++i){
        if(players.at(i).getName()==offered && players.at(i).isAlive() && players.at(turn).getName()!=offered){
            playerIndex = i;
            foundplayer=true;
            break;
        }
    }
    if(!foundplayer){
        cout << "Invalid player name." << endl;
        return;
    }
    int givenPos=getBuildingPos(given);
    if(givenPos < 0 || buildings.find(givenPos)->second->getOwner() != players.at(turn).getName()){
        cout << "Invalid building." << endl;
        return;
    }
    
    int i = getBuildingPos(recieved);
    if(i < 0 || buildings.find(i)->second->getOwner() != offered){
        cout << "Invalid building." << endl;
        return;
    }
    
    cout << "Player: " << players.at(turn).getName() << " wants to trade " << given << " for " << recieved << "." << endl;
    while (true) {
        cout << "Say 'yes' or 'no'" << endl;
        cin >> input;
        if (input == "yes") {
            players.at(turn).attachBuilding(i);
            players.at(playerIndex).detachBuilding(i);
            players.at(turn).detachBuilding(givenPos);
            players.at(playerIndex).attachBuilding(givenPos);
            buildings.find(i)->second->setOwner(players.at(turn).getName());
            buildings.find(givenPos)->second->setOwner(offered);
            break;
        } else if (input == "no") {
            break;
        }
    }
}

void Board::trade(string offered, string given, int recieved){
    string input;
    int playerIndex;
    bool foundplayer=false;
    for(int i = 0; i < startedPlayers; ++i){
        if(players.at(i).getName()==offered && players.at(i).isAlive() && players.at(turn).getName()!=offered){
            playerIndex = i;
            foundplayer=true;
            break;
        }
    }
    if(!foundplayer){
        cout << "Invalid player name." << endl;
        return;
    }
    int givenPos=getBuildingPos(given);
    if(givenPos < 0 || buildings.find(givenPos)->second->getOwner() != players.at(turn).getName()){
        cout << "Invalid building." << endl;
        return;
    }
    if(recieved < 0 || recieved > players.at(playerIndex).getMoney()){
        cout << "Invalid money." << endl;
        return;
    }
    cout << "Player: " << players.at(turn).getName() << " wants to trade " << given << " for $" << recieved << "." << endl;
    while (true) {
        cout << "Say 'yes' or 'no'" << endl;
        cin >> input;
        if (input == "yes") {
            players.at(turn).setMoney(players.at(turn).getMoney() + recieved);
            players.at(playerIndex).setMoney(players.at(playerIndex).getMoney() - recieved);
            players.at(turn).detachBuilding(givenPos);
            players.at(playerIndex).attachBuilding(givenPos);
            buildings.find(givenPos)->second->setOwner(offered);
            break;
        } else if (input == "no") {
            break;
        }
    }
}

bool Board::prisonRoll(int dice1, int dice2) {
    string input;
    cout << "You are still at DC Tims Line!" << endl;
    if (dice1 == dice2) {
        cout << "You rolled a double! Exiting DC Tims Line." << endl;
        players.at(turn).setPrison(0);
    } else {
        while (true) {
            cout << "You failed to roll a double. Do you wish to pay $50 (command '50') or Roll Up the Rim up? (rollup) or pass the turn (pass)" << endl;
            cin >> input;
            if (input == "50") {
                askForPay(50, "BANK", players.at(turn));
                players.at(turn).setPrison(0);
                return true;
            } else if (input == "rollup" && players.at(turn).getTims() > 0) {
                cout << "Rolling tims cup" << endl;
                players.at(turn).setPrison(0);
                players.at(turn).remTims();
                --totalTims;
                return true;
                // roll up tims
            } else if (input == "rollup") {
                cout << "You do not own any Tims cups." << endl;
            } else if (input == "pass" && players.at(turn).getPrison() != 1) {
                cout << "Passing roll" << endl;
                players.at(turn).setPrison(players.at(turn).getPrison() - 1);
                return false;
            } else if (input == "pass" && players.at(turn).getPrison() == 1) {
                cout << "This is your third turn in DC Tims Line. You must pay $50 or using rollup" << endl;
            }
        }
    }
    return true;
}

void Board::roll() {
    string input;
    bool rolled = true;
    srand (time(NULL));
    
    int dice1 = rand() % 6 + 1;
    int dice2 = rand() % 6 + 1;
    // if already rolled
    if (rollCount == 1 || doubleCount > 2) {
        cout << "You already rolled" << endl;
        return;
    } else if (doubleCount == 2 && dice1 == dice2) {
        cout << "You rolled 3 doubles!" << endl;
        ++doubleCount;
        runNonProp(30);
        return;
    }
    
    // if not done in main

    ++rollCount;
    if (players.at(turn).getPrison()) {
        rolled = prisonRoll(dice1, dice2);
    } else if (dice1 == dice2) {
        cout << "You rolled a double! You can roll again!\n";
        ++doubleCount;
        rollCount = 0;
    }
    
    // move
    if (rolled) move(dice1, dice2);
    
}

void Board::next() {
    
    // check if player paid rent
    if (rollCount != 1 && doubleCount != 3) {
        std::cout << "You can still roll" << std::endl;
        return;
    }
    std::cout << players.at(turn).getName() << "'s turn ended." << std::endl;
    if (turn >= startedPlayers - 1) {
        turn -= startedPlayers;
    }
    ++turn;
    
    if (!players.at(turn).isAlive()) next();
    doubleCount = 0;
    rollCount = 0;
    std::cout << "It is now " << players.at(turn).getName() << "'s turn. " << std::endl;
}

int Board::getBuildingPos(std::string name) {
    for (auto it: buildings) {
        if (it.second->getName() == name)
            return it.second->getPosition();
    }
    return -1;
}

bool Board::isBuilding(int pos) {
    return buildings.count(pos);
}

// requires fix
void Board::buy(int pos) {
    // try...catch
    std::string ans;
    while (true) {
        std::cin >> ans;
        if (ans == "Yes" && canBuy(players.at(turn), buildings.find(pos)->second->getPrice())) {
            buildings.find(pos)->second->setOwner(players.at(turn).getName());
            players.at(turn).setMoney(players.at(turn).getMoney() -
                                      buildings.find(pos)->second->getPrice());
            if (buildings.find(pos)->second->getType() == "ACADEMIC")
                players.at(turn).attachBuilding(pos);
            else if (buildings.find(pos)->second->getType() == "RESIDENCE")
                players.at(turn).addRes();
            else
                players.at(turn).addGym();
            break;
        } else if (ans == "Yes") {
            cout << "You don't have enough money to buy this property.\n";
            
        } else if (ans == "No") {
            this->auction(pos);
            break;
        } else if (ans == "Mortgage") {
            string building;
            // this->mortage(building);
        } else {
            std::cout << "Please enter Yes or No or Mortgage\n";
        }
    }
}


void Board::auction(int pos) {
    std::cout << "Auction has started! First player gets to bet first.\n";
    std::vector<int> inAuction;
    int currentBid = 0;
    int tempTurn = 0;
    int tempBid = 0;
    int deadCount = 0;
    std::string withDraw; // keeps track of who withdrew
    std::istringstream convert;
    
    // initialize inAuction, vector of players still in the aution
    int size = players.size();
    for (int i = 0; i < size; ++i) {
        if (players.at(i).isAlive())
            inAuction.emplace_back(i);
        else
            ++deadCount;
    }
    
    size -= deadCount;
    std::cout << "Auction for " << buildings.find(pos)->second->getName() << " has begun.\n";
    // run the auction
    while (inAuction.size() != 1) {
        std::cout << "Player: " << players.at(inAuction.at(tempTurn)).getName() << std::endl;
        std::cout << "Please enter a bid or withdraw\n";
        std::cin >> withDraw;
        
        // if input is string
        if (withDraw == "withdraw") {
            //remove player from inAuction
            inAuction.erase(inAuction.begin() + tempTurn);
            if (tempTurn >= inAuction.size()) tempTurn = 0;
            continue;
        } else if (withDraw[0] < 48 || withDraw[0] > 57) {
            std::cout << "Please enter a valid bid or command.\n";
            continue;
        }
        
        // if input is integer
        convert.clear();
        convert.str(withDraw);
        convert >> tempBid;
        if (tempBid > currentBid && tempBid <= players.at(inAuction.at(tempTurn)).getMoney()) {
            std::cout << "A new bid has been set to " << tempBid << std::endl;
            currentBid = tempBid;
            ++tempTurn;
            if (tempTurn >= inAuction.size()) tempTurn = 0;
        } else {
            std::cout << "Please enter a bid that is higher than current bid and less than what you own.\n";
        }
    }
    
    // take out money, give building to player
    int playerInd = inAuction.at(0);
    std::cout << "Player: " << players.at(playerInd).getName() << " wins this auction.\n";
    players.at(inAuction.at(0)).setMoney(players.at(playerInd).getMoney() - currentBid);
    buildings.find(pos)->second->setOwner(players.at(playerInd).getName());
    string typeBuild = buildings.find(pos)->second->getType();
    if (typeBuild == "ACADEMIC")
        players.at(inAuction.at(0)).attachBuilding(pos);
    else if (typeBuild == "GYM")
        players.at(inAuction.at(0)).addGym();
    else
        players.at(inAuction.at(0)).addRes();
}


void Board::payRent(int position) {
    int rent;
    string player = buildings.find(position)->second->getOwner();
    
    rent = buildings.find(position)->second->getRent();
    // checks if player can rent
    askForPay(rent, player, players.at(turn));
    
    
    // paying process
    if (buildings.find(position)->second->getType() == "GYM") {
        string gymOwner = buildings.find(position)->second->getOwner();
        int gymNum = 0;
        for(Player &p:players){
            if(p.getName()==gymOwner){
                gymNum=p.getGymNum();
                break;
            }
        }
        if(gymNum==2)rent = (rent * 2.5);
        
    } else if (buildings.find(position)->second->getType() == "RESIDENCE") {
        string resOwner = buildings.find(position)->second->getOwner();
        int resNum = 0;
        for(Player &p:players){
            if(p.getName()==resOwner){
                resNum=p.getResNum();
                break;
            }
        }
        for (int i = 1; i < resNum; ++i) {
            rent = rent * 2;
        }
    }
    int playerIndex = this->findPlayer(buildings.find(position)->second->getOwner());
    players.at(playerIndex).setMoney(players.at(playerIndex).getMoney() + rent);
    // switch pay rent to true for player
}


int Board::findPlayer(std::string name) {
    int len = players.size();
    for (int i = 0; i < len; ++i) {
        if (players.at(i).getName() == name) return i;
    }
    return -1;
}

void Board::improve(string building,string action){
    int pos = getBuildingPos(building);
    if (pos == -1) {
        cout << "There is no such building. Please try again\n";
        return;
    } else if (buildings.find(pos)->second->getType() != "ACADEMIC") {
        cout << "You cannot add improvements to this building\n";
        return;
    } else if (buildings.find(pos)->second->getMortgaged()) {
        cout << "This property has been mortgaged.\n";
        return;
    } else if (buildings.find(pos)->second->getOwner() != players.at(turn).getName()) {
        cout << "You don't own this building.\n";
        return;
    }
    while (true) {
        if (static_cast<Academic*>(buildings.find(pos)->second)->getImproveCost() > players.at(turn).getMoney()) {
            cout << "You cannot afford this improvement." << endl;
            return;
        }
        if(action=="buy") {
            if (!static_cast<Academic*>(buildings.find(pos)->second)->addImprovement()) {
                cout << "You already have maximum number of improvements\n";
            } else {
                players.at(turn).setMoney(players.at(turn).getMoney()
                                          - static_cast<Academic*>(buildings.find(pos)->second)->getImproveCost());
                cout << "You have improved " << building << endl;
                cout << "This building has " << static_cast<Academic*>(buildings.find(pos)->second)->getImproveCount() << " improvements.\n";
            }
            break;
        } // needs fix
        else if(action=="sell") {
            if (!static_cast<Academic*>(buildings.find(pos)->second)->removeImprovement()) {
                cout << "You have no improvements.\n";
            } else {
                players.at(turn).setMoney(players.at(turn).getMoney()
                                          + static_cast<Academic*>(buildings.find(pos)->second)->getImproveCost());
                cout << "You have sold an improvement from " << building << endl;
            }
            break;
        } // needs fix
        else {
            cout << "Please enter either 'buy' or 'sell'\n";
        }
    }
    
}

void Board::assetsHelper() {
    assets(players.at(turn));
}
//prints all the assets of the given player
void Board::assets(Player &p){
    vector<int> assets = p.getAssets();
    cout << p.getName() << " has $" << p.getMoney() << " and owns the buildings:" << endl;
    for(int building:p.getAssets()){
        cout << buildings.find(building)->second->getName() << endl;
    }
    cout << p.getResNum() << " residences, and " << p.getGymNum() << " gyms." << endl;
}


// saves the data to given filename as described in watopoly.pdf
// fix data according to datat type
void Board::save(const string filename){
    ofstream out;
    out.open(filename);
    out << players.size() << endl;
    for(auto p:players){
        out << p.getName() << " " << p.getPiece() << " " << p.getTims() << " " << p.getMoney() << " " << p.getPosition();
        if (p.getPosition() == 10 && p.getPrison()) {
            out << ' ' << 1 << ' ' << 3-p.getPrison();
        } else if (p.getPosition() == 10) {
            out << ' ' << 0;
        }
        out << endl;
    }
    for(auto b:buildings){
        if (b.second->getType() == "ACADEMIC") {
            out << b.second->getName() << " " << b.second->getOwner() << " " << static_cast<Academic*>(b.second)->getImproveCount() << endl;
        } else {
            out << b.second->getName() << " " << b.second->getOwner() << " " << 0 << endl;
        }
    }
}


//goes through the players vector to print their assets.
void Board::all(){
    for(Player p:players){
        assets(p);
    }
}

void Board::runNonProp(int position) {
    string ans;
    int tenPercent = 0;
    switch(position) {
        case 0: case 40:
            cout << "You landed on OSAP. You collected $200." << endl;
            break;
        case 10:
            cout << "Visiting DC Tim's Line\n"; break;
        case 2: case 17: case 33:
            slc();
            break;
        case 4:
            while (true) {
                cout << "You landed on tuition. Choose either pay $300 or 10% of your total worth.\n";
                cin >> ans;
                if (ans == "10") {
                    tenPercent = players.at(turn).getMoney();
                    for (int position: players.at(turn).getAssets()) {
                        tenPercent += buildings.find(position)->second->getPrice();
                        if (buildings.find(position)->second->getType() == "ACADEMIC") {
                            tenPercent += static_cast<Academic*>(buildings.find(position)->second)->getImproveCount()
                            * static_cast<Academic*>(buildings.find(position)->second)->getImproveCost();
                        }
                    }
                    tenPercent = tenPercent/10;
                    askForPay(tenPercent, "BANK", players.at(turn));
                    break;
                } else if (ans == "300") {
                    askForPay(300, "BANK", players.at(turn));
                    break;
                } else {
                    cout << "Please enter either '300' or '10'" << endl;
                }
            }
            break;
        case 7: case 22: case 36:
            needles();
            break;
        case 20:
            cout << "You got attacked by geese!\n";
            break;
        case 30:
            cout << "GO TO TIMS\n";
            players.at(turn).setPosition(10);
            playerToPrison(players.at(turn));
            break;
            // prison;
        case 38:
            
            askForPay(150, "BANK", players.at(turn));
            break;
        default: break;
    }
}

bool Board::canBuy(Player &p, int cost) {
    return p.getMoney() >= cost;
}

void Board::mortgage(std::string building){
    
    
    Player &p = players.at(turn);
    int pos = getBuildingPos(building);
    string typeBuild = buildings.find(pos)->second->getType();
    if(typeBuild == "ACADEMIC"
       && static_cast<Academic*>(buildings.find(pos)->second)->getImproveCount()) {
        cout << "Sell the improvements first.\n";
        return;
    } else if (buildings.find(pos)->second->getMortgaged()==true){
        cout << "This building is already morgaged" << endl;
        return;
    } else if (buildings.find(pos)->second->getOwner() != p.getName()) {
        cout << "You do not own this building.\n";
        return;
    }
    
    if (typeBuild == "GYM") {
        players.at(turn).remGym();
    } else if (typeBuild == "RESIDENCE") {
        players.at(turn).remRes();
    }
    int price=0;
    
    for(auto &b: buildings) {
        if( b.second->getName()==building ){
            price = b.second->getPrice();
            break;
        }
    }
    p.setMoney(p.getMoney()+price/2);
    buildings.find(pos)->second->setMortgaged(true);
}

void Board::unmortgage(std::string building){
    Player &p = players.at(turn);
    int pos = getBuildingPos(building);
    string typeBuild = buildings.find(pos)->second->getType();
    
    if(buildings.find(pos)->second->getMortgaged()==false){
        cout << "This building is already unmorgaged" << endl;
        return;
    }
    
    if (buildings.find(pos)->second->getOwner() != p.getName()) {
        cout << "You do not own this building.\n";
        return;
    }
    
    if (typeBuild == "GYM") {
        players.at(turn).addGym();
    } else if (typeBuild == "RESIDENCE") {
        players.at(turn).addRes();
    }
    
    int price=0;
    
    for(auto &b: buildings) {
        if( b.second->getName()==building ){
            price = b.second->getPrice();
            break;
        }
    }
    price = price*6/10;
    if(p.getMoney()<price){
        cout<<"You do not have enough money!"<<endl;
        return;
    }
    
    p.setMoney(p.getMoney()-price);
    buildings.find(pos)->second->setMortgaged(false);
}

void Board::bankrupt(string creditor) {
    int newOwner;
    for(int i = 0; i < numPlayer; ++i){
        if(players.at(i).getName()==creditor){
            newOwner=i;
        }
    }
    int mortgagePay = 0;
    for (int buildingPos:players.at(turn).getAssets()){
        if (buildings.find(buildingPos)->second->getMortgaged())
            mortgagePay += buildings.find(buildingPos)->second->getPrice() * 0.1;
        buildings.find(buildingPos)->second->setOwner(creditor);
        players.at(newOwner).attachBuilding(buildingPos);
    }
    players.at(newOwner).setMoney(players.at(turn).getMoney() + players.at(newOwner).getMoney());
    players.at(newOwner).setGym(players.at(turn).getGymNum() + players.at(newOwner).getGymNum());
    players.at(newOwner).setRes(players.at(turn).getResNum() + players.at(newOwner).getResNum());
    askForPay(mortgagePay, "BANK", players.at(newOwner));
    --numPlayer;
    cout << "Player: " << players.at(turn).getName() << " has declared bankruptcy.\n";
    totalTims -= players.at(turn).getTims();
    players.at(turn).bankrupt();
    if (numPlayer == 1) {
        cout << "Player: " << creditor << " won!\n" << endl;
    }
}

void Board::bankruptToBank(Player &loser) {
    --numPlayer;
    totalTims -= loser.getTims();
    loser.bankrupt();
    // all building owner -> bank
    for (int buildingPos: loser.getAssets()) {
        buildings.find(buildingPos)->second->setOwner("BANK");
        buildings.find(buildingPos)->second->setMortgaged(false);
        auction(buildingPos);
    }
    
    // check if anyone won (run through the players)
    if (numPlayer == 1) {
        for (auto &p: players) {
            if (p.isAlive()) {
                cout << "Player: " << p.getName() << " won!\n" << endl;
                break;
            }
        }
    }
}

int Board::numPlayers() {
    return this->numPlayer;
}

void Board::askForPay(int payment, string creditor, Player &p) {
    cout << "You need to pay " << payment << " to " << creditor << "." <<endl;
    while (payment > p.getMoney()) {
        cout << "You do not have enough money. Please choose an option." << endl;
        string ans;
        string ans2;
        string ans3;
        cin >> ans;
        if (ans == "mortgage") {
            cin >> ans;
            int pos = getBuildingPos(ans);
            if (pos == -1) {
                cout << "That is not a building\n";
            } else if (players.at(turn).getName() != buildings.find(pos)->second->getOwner()) {
                cout << "That is not your building.\n";
            } else {
                cout << "Being mortgaged\n";
                mortgage(buildings.find(pos)->second->getName());
            }
        } else if (ans == "bankrupt") {
            if (creditor == "BANK")
                bankruptToBank(p);
            else
                bankrupt(creditor);
            return;
        } else if (ans == "assets") {
            assets(p);
        } else if (ans == "sellImprovement") {
            cin >> ans2;
            improve(ans2, "sell");
        } else {
            cout << "Please enter either: mortgage <building>, sell improvements <sellImprovement building_name>none\n";
        }
    }
    p.setMoney(p.getMoney() - payment);
}

void Board::slc() {
    
    cout << "You landed on SLC" << endl;
    srand(time(NULL));
    int r = rand() % 24;
    int tims = rand() % 100;
    if (tims == 1 && totalTims < 4) {
        cout << "You won a tims card" << endl;
        players.at(turn).addTims();
        ++totalTims;
        return;
    }
    
    if(r>=0 && r<=2){
        cout<< "You have moved 3 squares back" << endl;
        move(2,-5);
    } else if(r>=3 && r<=6){
        cout<<"You have moved 2 squares back" << endl;
        move(3, -5);
    } else if(r>=7 && r<=10){
        cout<<"You have moved 1 square back" << endl;
        move(4,-5);
    } else if(r>=11 && r<=13){
        cout<<"You have moved 1 square forward" << endl;
        move(-3,4);
    } else if(r>=14 && r<=17){
        cout<<"You have moved 2 squares forward" << endl;
        move(-3,5);
    } else if(r>=18 && r<=21){
        cout<<"You have moved 3 squares forward" << endl;
        move(-1,4);
    } else if(r==22){
        cout<<"You have been moved to DC Tims Line" << endl;
        players.at(turn).setPosition(10);
        // prison;
    } else {
        cout<<"You have been moved to Collect OSAP" << endl;
        players.at(turn).setPosition(40);
    }
}

void Board::needles() {
    
    srand(time(NULL));
    int r = rand() % 18;
    int tims = rand() % 100;
    int change = 0;
    if (tims == 1 && totalTims < 4) {
        cout << "You won a tims card" << endl;
        players.at(turn).addTims();
        ++totalTims;
        return;
    }
    
    if( r>=0 && r<=0  )      change = -200;
    else if( r>=1 && r<=2  )      change = -100;
    else if( r>=3 && r<=5  )      change = -50;
    else if( r>=6 && r<=11 )      change = 25;
    else if( r>=12 && r<=14 )     change = 50;
    else if( r>=15 && r<=16 )     change = 100;
    else if( r>=17 && r<=17 )     change = 200;
    
    cout << "You landed on Needles Hall. Your change is " << change << endl;
    if (change < 0)
        askForPay((-1*change), "BANK", players.at(turn));
    else
        players.at(turn).setMoney(players.at(turn).getMoney() + change);
}

void Board::playerToPrison(Player &p) {
    p.setPrison(3);
}

Board::~Board() {
    for (auto &b: buildings) {
        delete b.second;
    }
}
