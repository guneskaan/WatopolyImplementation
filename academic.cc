#include "academic.h"
#include <string>


int Academic::getPrice() {
    return this->price;
}

int Academic::getRent() {
    return this->tuitions[improveCount];
}

int Academic::getImproveCost() {
    return this->improveCost;
}

int Academic::getImproveCount() {
    return this->improveCount;
}

std::string Academic::getName() {
    return this->name;
}

std::string Academic::getBlock() {
    return this->block;
}

int Academic::getPosition() {
    return this->position;
}

std::string Academic::getOwner() {
    return this->owner;
}

void Academic::setOwner(std::string owner) {
    this->owner = owner;
}

std::string Academic::getType() {
    return "ACADEMIC";
}

bool Academic::addImprovement() {
    if (this->improveCount == 5) return false;
    ++this->improveCount;
    return true;
}

bool Academic::removeImprovement() {
    if (this->improveCount == 0) return false;
    --this->improveCount;
    return true;
}

bool Academic::getMortgaged() {
    return this->mortgaged;
}

void Academic::setMortgaged(bool mortgaged) {
    this->mortgaged = mortgaged;
}

Academic::~Academic() {
    delete[] tuitions;
}
