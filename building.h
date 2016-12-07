#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <string>

class Building {
  public:
    virtual int getPrice() = 0;
    virtual int getRent() = 0;
    virtual std::string getName() = 0;
    virtual std::string getOwner() = 0;
    virtual int getPosition() = 0;
    virtual void setOwner(std::string owner) = 0;
    virtual std::string getType() = 0;
    virtual bool getMortgaged() = 0;
    virtual void setMortgaged(bool mortgaged) = 0;
    virtual ~Building(){};
};
#endif
