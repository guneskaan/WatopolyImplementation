#ifndef __WATOPOLY_H__
#define __WATOPOLY_H__

#include <string>
#include "board.h"

class Watopoly {
    bool test;
    Board *b;
  public:
    Watopoly(bool load, bool test, std::string filename);
    void play();
    
};
#endif
