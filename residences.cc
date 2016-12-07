#include <string>
#include "residences.h"

int Residence::getPrice() {
    return this->price;
}

int Residence::getRent() {
    return this->rent;
}

std::string Residence::getName() {
    return this->name;
}

int Residence::getPosition() { 
    return this->position;
}

std::string Residence::getOwner() {
    return this->owner;
}

void Residence::setOwner(std::string owner) {
    this->owner = owner;
}

std::string Residence::getType() {
    return "RESIDENCE";
}

bool Residence::getMortgaged() {
    return mortgaged;
}

void Residence::setMortgaged(bool mortgaged) {
    this->mortgaged = mortgaged;
}

Residence::~Residence(){}
