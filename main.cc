#include <iostream>
#include <string>
#include "watopoly.h"
using namespace std;

int main (int argc, char **argv){
    bool testing=false;
    bool load=false;
    string filename = "";
    
    // determine if loading/testing are enabled
    for(int a=1; a < argc; ++a){
        if(string(argv[a]) == "-load"){
            load=true;
            filename=argv[++a];
            cout << "LOADING FROM FILE" << filename << endl;
        }
        else if(string(argv[a]) == "-testing"){
            testing = true;
            cout << "TESTING ENABLED" << endl;
        }
    }
    Watopoly wp(load,testing,filename);
    wp.play();
}
