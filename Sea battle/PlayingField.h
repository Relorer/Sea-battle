#pragma once
#include "Ship.h"
#include "NewString.h"

class PlayingField
{
public:

	void printField();

	void printField2();

	void clearTheField();

	int getOverallHealth();

	int getOverallEnemyHealth();

	NewString coordinatesToLine(const int x, const int y);

	void initializeTheRandomFleet();

	bool addShip(int &x1, int &x2, int &y1, int &y2);

	bool getPos(int &x, int &y, FILE * filestream = stdin);

	enum resultShot { miss = 0, hit, killed, missOnWounded};
	resultShot hitDetection(const int & x, const int & y, int & type);

	void hitResultDetection(const int & x, const int & y, resultShot result);

	void shoot(int & x, int & y);

	PlayingField();
	
private:

	bool dataValidation(int x1, int x2, int y1, int y2);

	void initializeShip(const int x1, const int x2, const int y1, const int y2);

	int generatingRandomArrayElement(const int * arr, int size);

	int generatingRandomArrayElement(const int * arr, int size, int exception);

	int generatingRandomArrayNumber(const int * arr, int size, int exception);
	
	enum direction { all = 0, horizon, vertically };
	bool scan(const int x, const int y, direction _direction = all, int _smallestShip = 0);

	void filler(const int & x, const int & y, direction _direction);

	int smallestShip(int minimum = 0);

	enum status { water = 0, part, empty, search };

	int field[10][10]{};
	int deck[4];
	Ship ships[10];
	int numberOfInitializedShips;
	int overallHealth;
	
	
	int enemySails[4];
	int overallEnemyHealth;
	int enemy[2][10][10]{};

	resultShot lastShot = miss;
	int lHitx2, lHity2;
	int lHitx1, lHity1;
	int searchPoints1, searchPoints2, searchPoints3;
};