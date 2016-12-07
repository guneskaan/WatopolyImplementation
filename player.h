#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <string>
#include <vector>
class Player{
	const std::string name;
	int money;
	int position;
	int resNum;
    int gymNum;
    int cups;
	int timsCard;
    int prison;
	bool alive;
	const char piece;	
	std::vector <int> buildings;
  public:
	Player(const std::string givename, const char piece);
	void changeCups(int change);
    const std::string getName();	
	const char getPiece();
	int getMoney();
    bool isAlive();
	void setMoney(const int newmoney);
	void bankrupt();
	int getPosition();
	void setPosition(const int newpos);
	int getResNum();
    int getGymNum();
	void addRes();
    void addGym();
    void remGym();
	void remRes();
	int getTims();
    void setGym(int gym);
    void setRes(int res);
	void addTims();
	void remTims();
    void setPrison(int prison);
    int getPrison();
	void setTims(const int newAmt); 	
	void attachBuilding(const int building);
	void detachBuilding(const int building);	
	std::vector <int> getAssets();
};
#endif
