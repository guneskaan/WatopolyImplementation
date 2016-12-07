#include "watopoly.h"
#include "building.h"
#include "player.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;


Watopoly::Watopoly(bool load, bool test, string filename) {
    this->test=test;
    b = new Board(load, test, filename);
    //play();
}

int convertToInt(string &str){
    int res = 0;
    for(int i=0;i<str.size();i++){
        if(str[i]<'0' || str[i]>'9')
            return -1;
        res = res*10+(str[i]-'0');
    }
    return res;
}

void Watopoly::play(){
    string str,input;
    while(true){
        if (b->numPlayers() == 1) {
            delete b;
            cout << "Game over.\n";
            break;
        }
        cout << "Please enter a command\n";
        getline(cin,input);
        if(input.size()==0)
            getline(cin,input);
   //     cout<<input<<"***"<<endl;
        istringstream is(input);
        is>>str;
        if(str == "roll"){
            if(test){ // normal if no parameters
                int roll1, roll2;
                is >> roll1;
                if(is.eof()){
                    b->roll();
                }
                else{
                    is>>roll2;
                    b->roll(roll1,roll2);
                }
            }
            else{
                b->roll();
            }
        }
        else if(str == "trade"){
            
            string arr[3];
            int cint[3];
            bool invalid = false;
            
            for(int k=0;k<3;k++){
                is>>arr[k];
                if(arr[k].size()==0){
                    invalid = true;
                    break;
                }
                cint[k] = convertToInt(arr[k]);
            }
            
            if(invalid==true || is.eof()==false){
                cout<<"Invalid input"<<endl;
                continue;
            }
            
            if(cint[1]!=-1){ // means arr[1] is int
                if(cint[2]!=-1){ // means arr[2] is int
                    cout<<"Invalid input"<<endl;
                    continue;
                }
                b->trade(arr[0],cint[1],arr[2]);
            }
            else{   // means arr[1] is string
                if(cint[2]==-1) {
                    b->trade(arr[0],arr[1],arr[2]);
                } else {
                    b->trade(arr[0],arr[1],cint[2]);
                }
            }
            
        }
        else if(str == "next") b->next();
        else if(str == "all")b->all();
        else if(str == "assets")b->assetsHelper(); //modify assets
        else if(str == "save"){
            string filename;
            is >> filename;
            b->save(filename);
        }
        else if(str == "improve"){
            string buildingName,buyOrSell;
            is >> buildingName >> buyOrSell;
            b->improve(buildingName,buyOrSell);
        }
        else if(str == "mortgage"){
            string buildingName;
            is >> buildingName;
            b->mortgage(buildingName);
        }
        else if(str == "unmortgage") {
            string buildingName;
            is >> buildingName;
            b->unmortgage(buildingName);
        }
        else {
            cout << "Invalid input." << endl;
        }
    }
}
