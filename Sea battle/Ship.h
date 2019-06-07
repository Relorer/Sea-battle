#pragma once

class Ship
{
public:
	void setCoordinates(const int newx1, const int newx2, const int newy1, const int newy2);
	
	bool verifyTheOwnership(const int & x, const int & y);

	int hurt(int & type);

	int getx(bool n);

	int gety(bool n);

	Ship();
private:
	int x1, y1, x2, y2;
	int health;
};