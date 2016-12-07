#ifndef __BOARD_H__
#define __BOARD_H__

#include "player.h"
#include "building.h"
// #include "nonProperty.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

// key changes
// i think map will be better solutions to building.

class Board {
    int turn, doubleCount, rollCount;
    std::vector<Player> players;
    std::map<int, Building*> buildings;
    int numPlayer;
    bool test;
    int totalTims;
    int startedPlayers;
    
  public:
    Board(bool load, bool test, std::string filename);
    void processCmd(std::string cmd);
    void roll(); // adds players position
    void roll(int dice1, int dice2);
    // then executes command within that board
    void next();
    void improve(std::string building, std::string action);
    void save(const std::string filename);
    void assetsHelper();
    void all();
    void mortgage(std::string building);
    void unmortgage(std::string building);
    void bankrupt(std::string creditor);
    void bankruptToBank(Player &loser);
    void trade(std::string offered, int given, std::string recieved);
    void trade(std::string offered, std::string given, std::string recieved);
    void trade(std::string offered, std::string given, int recieved);
    int numPlayers();
    ~Board();
  private:
    bool prisonRoll(int dice1, int dice2);
    void playerToPrison(Player &p);
    void slc();
    void needles();
    void askForPay(int payment, std::string creditor, Player &p);
    bool isBuilding(std::string name);
    void assets(Player &p);
    bool isBuilding(int pos);
    bool canBuy(Player &p, int cost);
    void buy(int pos);
    void move(int dice1, int dice2);
    void auction(int pos);
    void payRent(int position);
    void runNonProp(int position);
    int getBuildingPos(std::string name);
    int findPlayer(std::string name);
};
#endif
