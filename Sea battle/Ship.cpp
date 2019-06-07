#include "pch.h"
#include "Ship.h"

void Ship::setCoordinates(const int newx1, const int newx2, const int newy1, const int newy2)
{
	x1 = newx1;
	x2 = newx2;
	y1 = newy1;
	y2 = newy2;
	int type = x2 - x1 + y2 - y1;
	health = 1 + (type < 0 ? -type : type);
}

bool Ship::verifyTheOwnership(const int & x, const int & y)
{
	if (x1 <= x && y1 <= y && x2 >= x && y2 >= y) return 1;
	return 0;
}

int Ship::hurt(int & type)
{
	health--;
	type = x2 - x1 + y2 - y1;
	if (!health) {
		return 2;
	}
	return 1;
}

int Ship::getx(bool n)
{
	if (n) return x2;
	return x1;
}

int Ship::gety(bool n)
{
	if (n) return y2;
	return y1;
}

Ship::Ship() {}