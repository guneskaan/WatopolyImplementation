#ifndef __RESIDENCE_H__
#define __RESIDENCE_H__

#include <string>
#include "building.h"

class Residence: public Building {
    std::string name;
    int price, rent;
    int position;
    std::string owner;
    bool mortgaged;
  public:
    Residence(std::string name, int price, int rent, int position, std::string owner = "BANK"):
    name{name}, price{price}, rent{rent}, position{position}, owner{owner} {}
    int getPrice();
    int getRent();
    std::string getName();
    int getPosition();
    std::string getOwner();
    void setOwner(std::string owner);
    std::string getType();
    bool getMortgaged();
    void setMortgaged(bool mortgaged);
    ~Residence();
};
#endif
