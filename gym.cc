
#include <string>
#include <iostream>
#include <cstdlib>
#include "gym.h"
using namespace std;

int Gym::getPrice() {
    return this->price;
}

int Gym::getRent() {
    // roll dice * 10;
    srand (time(NULL));
    int roll = rand() % 6 + 1;
    int roll2 = rand () % 6 + 1;
    return (roll+roll2) * 4;
}

std::string Gym::getName() {
    return this->name;
}

int Gym::getPosition() {
    return this->position;
}

std::string Gym::getOwner() {
    return this->owner;
}

void Gym::setOwner(std::string owner) {
    this->owner = owner;
}

std::string Gym::getType() {
    return "GYM";
}

bool Gym::getMortgaged() {
    return mortgaged;
}

void Gym::setMortgaged(bool mortgaged) {
    this->mortgaged = mortgaged;
}

Gym::~Gym() {}
