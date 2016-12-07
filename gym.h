#ifndef __GYM_H__
#define __GYM_H__

#include <string>
#include "building.h"
class Gym: public Building {
    std::string name;
    int price;
    int position;
    std::string owner;
    bool mortgaged = false;
  public:
    Gym(std::string name, int price, int position, std::string owner = "BANK"):
    name{name}, price{price}, position{position}, owner{owner} {}
    int getPrice();
    int getRent();
    std::string getName();
    int getPosition();
    std::string getOwner();
    void setOwner(std::string owner);
    std::string getType();
    bool getMortgaged();
    void setMortgaged(bool mortgaged);
    ~Gym();
};
#endif
