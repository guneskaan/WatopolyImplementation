#ifndef __ACADEMIC_H__
#define __ACADEMIC_H__

#include "building.h"
#include <string>
class Academic: public Building {
    std::string name;
    int price;
    int position;
    int improveCost;
    std::string block;
    int *tuitions;
    int improveCount;
    std::string owner;
    bool mortgaged = false;
  public:
    Academic(std::string name, int price, int position, int improveCost,
             std::string block, int *tuitions, int improveCount, std::string owner = "BANK"):
    name{name}, price{price}, position{position}, improveCost{improveCost},
    block{block}, tuitions{tuitions}, improveCount{improveCount}, owner{owner} {}
    int getPrice();
    int getRent();
    int getImproveCost();
    int getImproveCount();
    bool addImprovement();
    bool removeImprovement();
    std::string getName();
    std::string getBlock();
    int getPosition();
    std::string getOwner();
    void setOwner(std::string owner);
    std::string getType();
    bool getMortgaged();
    void setMortgaged(bool mortgaged);
    ~Academic();
};
#endif
