#include "pch.h"
#include <iostream>
#include <regex>
#include "NewString.h"
#include "randset.h"
#include "PlayingField.h"
#include "Ship.h"

void PlayingField::printField()
{
	printf("  ");
	for (int i = 1; i < 11; i++) printf("%d ", i);
	for (int i = 0; i < 10; i++) {
		if (i < 9) printf("\n%c ", char(224 + i));
		else printf("\nк ");
		for (int j = 0; j < 10; j++) {
			if (field[i][j] == part) {
				printf("%c ", char(164));
			}
			else if (field[i][j] != part) {
				printf("%c ", '~');
			}
			else {
				printf("  ");
			}
		}
	}
}

void PlayingField::printField2()
{
	printf(" ");
	for (int i = 1; i < 11; i++) printf("%d ", i);
	for (int i = 0; i < 10; i++) {
		if (i < 9) printf("\n%c", char(224 + i));
		else printf("\nк");
		for (int j = 0; j < 10; j++) {
			if (enemy[0][i][j] == part) {
				printf("%c ", char(164));
			}
			else if (enemy[0][i][j] == water) {
				printf("%c ", char(127));
			}
			else {
				printf("  ");
			}
		}
	}
}

void PlayingField::clearTheField()
{
	if (numberOfInitializedShips) {
		for (int i = 0; i < 4; i++) this->deck[i] = 4 - i;
		numberOfInitializedShips = 0;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				field[i][j] = 0;
			}
		}
	}
}

int PlayingField::getOverallHealth()
{
	return overallHealth;
}

int PlayingField::getOverallEnemyHealth()
{
	return overallEnemyHealth;
}

NewString PlayingField::coordinatesToLine(const int x, const int y)
{
	NewString cor;
	if (y < 9) cor += char(y + 'А');
	else cor += 'К';
	if (x == 9) cor += "10";
	else cor += char(x % 10 + '0' + 1);
	return cor;
}

void PlayingField::initializeTheRandomFleet()
{
	clearTheField();
	for (int i = 0; i < 10; i++) {
		int x1, x2, y1, y2;
		do {
			x1 = generateRandomInTheRange(0, 9);
			y1 = generateRandomInTheRange(0, 9);
			int gen = generatingRandomArrayNumber(deck, 4, 0);
			if (generateRandomInTheRange(0, 1)) {
				x2 = x1 < gen ? x1 + gen : x1 - gen;
				y2 = y1;
			}
			else {
				y2 = y1 < gen ? y1 + gen : y1 - gen;
				x2 = x1;
			}
		} while (dataValidation(x1, x2, y1, y2));
		initializeShip(x1, x2, y1, y2);
	}
}

bool PlayingField::addShip(int &x1, int &x2, int &y1, int &y2)
{
	if (dataValidation(x1, x2, y1, y2)) return 1;
	initializeShip(x1, x2, y1, y2);
	return 0;
}

bool PlayingField::getPos(int &x, int &y, FILE * filestream)
{
	NewString pos;
	pos.getns(' ');
	std::cmatch axis;
	std::regex regular("([а-и, к, А-И, К])([\\d]+)");
	if (std::regex_match(pos.getStr(), axis, regular)) {
		NewString interim = axis[1].first;
		if (interim[0] <= 'И' && interim[0] >= 'А') y = (interim[0] - 'А');
		else if (interim[0] <= 'и' && interim[0] >= 'а') y = (interim[0] - 'а');
		else if (interim[0] == 'к') y = (interim[0] - 'а' - 1);
		else if (interim[0] == 'К') y = (interim[0] - 'А' - 1);
		if (interim.getLength() == 2) x = (interim[1] - '0') - 1;
		else if (interim.getLength() == 3) x = (interim[2] - '0') + 10 * (interim[1] - '0') - 1;
	}
	else return 1;
	if (x < 0 || x > 9 || y < 0 || y > 9) return 1;
	return 0;
}

PlayingField::resultShot PlayingField::hitDetection(const int & x, const int & y, int & type)
{
	if (field[y][x] == part) {
		overallHealth--;
		for (int i = 0; i < 10; i++) {
			if (ships[i].verifyTheOwnership(x, y)) {
				if (ships[i].hurt(type) == hit) return hit;
				return killed;
			}
		}
	}
	return miss;
}

void PlayingField::hitResultDetection(const int & x, const int & y, resultShot result)
{
	switch (result)
	{

	case PlayingField::miss:
	{
		enemy[0][y][x] = water;
		if (lastShot == hit || lastShot == missOnWounded) lastShot = missOnWounded;
		else lastShot = miss;
	}
	break;

	case PlayingField::hit:
	{
		enemy[0][y][x] = part;

		if (lHitx2 == -1) {
			lHitx2 = x;
			lHity2 = y;
		}
		else if (lHitx1 == -1) {
			lHitx1 = x;
			lHity1 = y;
		}
		else {
			if (x > lHitx2 || y > lHity2) {
				lHitx2 = x;
				lHity2 = y;
			}
			else {
				lHitx1 = x;
				lHity1 = y;
			}
		}

		lastShot = hit;

		if (lHity1 > lHity2) swap(lHity1, lHity2);
		if (lHitx1 > lHitx2) swap(lHitx1, lHitx2);

		overallEnemyHealth--;
	}
	break;

	case PlayingField::killed:
	{
		enemy[0][y][x] = part;

		if (lHitx2 == -1) {
			lHitx2 = x;
			lHity2 = y;
		}
		else if (lHitx1 == -1) {
			lHitx1 = x;
			lHity1 = y;
		}
		else {
			if (x > lHitx2 || y > lHity2) {
				lHitx2 = x;
				lHity2 = y;
			}
			else {
				lHitx1 = x;
				lHity1 = y;
			}
		}
		lHitx1 = lHitx1 != -1 ? lHitx1 : lHitx2;
		lHity1 = lHity1 != -1 ? lHity1 : lHity2;

		if (lHity1 > lHity2) swap(lHity1, lHity2);
		if (lHitx1 > lHitx2) swap(lHitx1, lHitx2);

		int type = lHitx2 - lHitx1 + lHity2 - lHity1;
		type = type < 0 ? -type : type;
		enemySails[type]--;

		for (int i = (lHity1 > 0 ? lHity1 - 1 : lHity1); i <= (lHity2 < 9 ? lHity2 + 1 : lHity2); i++) {
			for (int j = (lHitx1 > 0 ? lHitx1 - 1 : lHitx1); j <= (lHitx2 < 9 ? lHitx2 + 1 : lHitx2); j++) {
				enemy[0][i][j] = PlayingField::water;
			}
		}
		for (int i = lHity1; i <= lHity2; i++) {
			for (int j = lHitx1; j <= lHitx2; j++) {
				enemy[0][i][j] = PlayingField::part;
			}
		}

		lastShot = killed;
		lHitx1 = lHity1 = -1;
		lHitx2 = lHity2 = -1;

		overallEnemyHealth--;
	}
	break;

	}
}

void PlayingField::shoot(int & x, int & y)
{
	x = y = -1;
	if (lastShot != hit && lastShot != missOnWounded) {
		int choice[3]{};
		if (searchPoints1) choice[0] = 1;
		if (!enemySails[3] && searchPoints2) choice[1] = 1;
		if (!enemySails[3] && !enemySails[2] && !enemySails[1] && searchPoints3) choice[2] = 1;
		switch (generatingRandomArrayNumber(choice, 3, 0))
		{
		case 0:
			if (searchPoints1) {
				do {
					int searchPoints[4]{ 3, 7, 11, 15 };
					y = generatingRandomArrayElement(searchPoints, 4);
					x = generateRandomInTheRange((y < 9 ? 0 : y - 9), (y > 9 ? 9 : y));
					y -= x;
					if (enemy[1][y][x] != search) {
						enemy[1][y][x] = search;
						searchPoints1--;
					}
					else continue;
					if (!searchPoints1 && scan(x, y)) {
						shoot(x, y);
						break;
					}
					if (enemy[0][y][x] != empty) continue;
				} while (scan(x, y));
			}
			break;
		case 1:
			if (searchPoints2) {
				do {
					int searchPoints[5]{ 1, 5, 9, 13, 17 };
					y = generatingRandomArrayElement(searchPoints, 5);
					x = generateRandomInTheRange((y < 9 ? 0 : y - 9), (y > 9 ? 9 : y));
					y -= x;
					if (enemy[1][y][x] != search) {
						enemy[1][y][x] = search;
						searchPoints2--;
					}
					else continue;
					if (!searchPoints2 && scan(x, y)) {
						shoot(x, y);
						break;
					}
					if (enemy[0][y][x] != empty) continue;
				} while (scan(x, y));
			}
			break;
		case 2:
			if (searchPoints3) {
				do {
					int searchPoints[10]{ 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
					y = generatingRandomArrayElement(searchPoints, 10);
					x = generateRandomInTheRange((y < 9 ? 0 : y - 9), (y > 9 ? 9 : y));
					y -= x;
					if (enemy[1][y][x] != search) {
						enemy[1][y][x] = search;
						searchPoints3--;
					}
					else continue;
					if (!searchPoints3 && scan(x, y)) {
						shoot(x, y);
						break;
					}
					if (enemy[0][y][x] != empty) continue;
				} while (scan(x, y));
			}
			break;
		}
	}
	else {
		if (lHitx1 == -1 && lHitx2 != -1) {
			int direct[4]{ 1, 2, 3, 4 };
			for (int i = 0; i < 4; i++) {
				switch (generatingRandomArrayElement(direct, 4, 0))
				{
				case 1:
					if (scan(lHitx2, lHity2 + 1, vertically, 1)) direct[0] = 0;
					else {
						x = lHitx2;
						y = lHity2 + 1;
					}
					break;
				case 2:
					if (scan(lHitx2 + 1, lHity2, horizon, 1)) direct[1] = 0;
					else {
						x = lHitx2 + 1;
						y = lHity2;
					}
					break;
				case 3:
					if (scan(lHitx2, lHity2 - 1, vertically, 1)) direct[2] = 0;
					else {
						x = lHitx2;
						y = lHity2 - 1;
					}
					break;
				case 4:
					if (scan(lHitx2 - 1, lHity2, horizon, 1)) direct[3] = 0;
					else {
						x = lHitx2 - 1;
						y = lHity2;
					}
					break;
				}
				if (x != -1) break;
			}
		}
		else {
			int type = lHitx2 - lHitx1 + lHity2 - lHity1;
			type = type < 0 ? -type : type;
			type++;
			if (lHitx2 - lHitx1) {
				int direct[2]{ 1, 2 };
				for (int i = 0; i < 2; i++) {
					switch (generatingRandomArrayElement(direct, 2, 0))
					{
					case 1:
						if (scan(lHitx1 - 1, lHity1, horizon, type)) direct[0] = 0;
						else {
							x = lHitx1 - 1;
							y = lHity1;
						}
						break;
					case 2:
						if (scan(lHitx2 + 1, lHity2, horizon, type)) direct[1] = 0;
						else {
							x = lHitx2 + 1;
							y = lHity2;
						}
						break;
					}
					if (x != -1) break;
				}
			} // if (lHity2 - lHity1)
			else if (lHity2 - lHity1) {
				int direct[2]{ 1, 2 };
				for (int i = 0; i < 2; i++) {
					switch (generatingRandomArrayElement(direct, 2, 0))
					{
					case 1:
						if (scan(lHitx1, lHity1 - 1, vertically, type)) direct[0] = 0;
						else {
							x = lHitx1;
							y = lHity1 - 1;
						}
						break;
					case 2:
						if (scan(lHitx2, lHity2 + 1, vertically, type)) direct[1] = 0;
						else {
							x = lHitx2;
							y = lHity2 + 1;
						}
						break;
					}
					if (x != -1) break;
				}
			} // else if (lHitx2 - lHitx1)
			if (type > 3) x = -1;
		}
	}
	if (x == -1) {
		//printf("\nx = -1, last = %d, hity1 = %d, hitx1 = %d, hity2 = %d, hitx2 = %d,\n", lastShot, lHity1, lHitx1, lHity2, lHitx2);
		printf("Предположительно вы - *******");
		exit(0);
	}
}

bool PlayingField::dataValidation(int x1, int x2, int y1, int y2)
{
	if (x1 != x2 && y1 != y2) return 1;
	if (y1 > y2) swap(y1, y2);
	if (x1 > x2) swap(x1, x2);
	int i;
	for (i = 3; i > -1; i--) if (deck[i]) break;
	if ((x2 - x1 + y2 - y1) > i) return 1;
	for (int i = (y1 > 0 ? y1 - 1 : y1); i <= (y2 < 9 ? y2 + 1 : y2); i++) {
		for (int j = (x1 > 0 ? x1 - 1 : x1); j <= (x2 < 9 ? x2 + 1 : x2); j++) {
			if (field[i][j] == PlayingField::part) return 1;
		}
	}
	for (int i = y1; i <= y2; i++) {
		for (int j = x1; j <= x2; j++) {
			if (field[i][j] != PlayingField::water) return 1;
		}
	}
	return 0;
}

void PlayingField::initializeShip(int x1, int x2, int y1, int y2)
{
	if (y1 > y2) swap(y1, y2);
	if (x1 > x2) swap(x1, x2);
	int type = x2 - x1 + y2 - y1;
	type = type < 0 ? -type : type;
	deck[type]--;
	for (int i = (y1 > 0 ? y1 - 1 : y1); i <= (y2 < 9 ? y2 + 1 : y2); i++) {
		for (int j = (x1 > 0 ? x1 - 1 : x1); j <= (x2 < 9 ? x2 + 1 : x2); j++) {
			field[i][j] = PlayingField::empty;
		}
	}
	for (int i = y1; i <= y2; i++) {
		for (int j = x1; j <= x2; j++) {
			field[i][j] = PlayingField::part;
		}
	}
	ships[numberOfInitializedShips++].setCoordinates(x1, x2, y1, y2);
}

int PlayingField::generatingRandomArrayElement(const int * arr, int size)
{
	return arr[generateRandomInTheRange(0, size - 1)];
}

int PlayingField::generatingRandomArrayElement(const int * arr, int size, int exception)
{
	int * choice = new int[size];
	int j = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] != exception) {
			choice[j] = arr[i];
			j++;
		}
	}
	int result;
	if (!j) result = exception;
	else result = choice[generateRandomInTheRange(0, --j)];
	delete[] choice;
	return result;
}

int PlayingField::generatingRandomArrayNumber(const int * arr, int size, int exception)
{
	int * choice = new int[size];
	int j = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] != exception) {
			choice[j] = i;
			j++;
		}
	}
	int result;
	if (!j) result = -1;
	else result = choice[generateRandomInTheRange(0, --j)];
	delete[] choice;
	return result;
}

bool PlayingField::scan(const int x, const int y, direction _direction, int _smallestShip)
{
	if (x < 0 || x > 9 || y < 0 || y > 9 || enemy[0][y][x] != empty) return 1;
	_smallestShip = smallestShip(_smallestShip);
	int freeCell = 0;
	int delta;
	switch (_direction)
	{
	case PlayingField::horizon:
		delta = 0;
		while (x + delta < 10 && enemy[0][y][x + delta++] != water) freeCell++;
		delta = 0;
		while (x - delta < 10 && enemy[0][y][x - delta++] != water) freeCell++;
		if (freeCell < _smallestShip) {
			filler(x, y, horizon);
			return 1;
		}
		break;
	case PlayingField::vertically:
		delta = 0;
		while (y + delta < 10 && enemy[0][y + delta++][x] != water) freeCell++;
		delta = 0;
		while (y - delta < 10 && enemy[0][y - delta++][x] != water) freeCell++;
		if (freeCell < _smallestShip) {
			filler(x, y, vertically);
			return 1;
		}
		break;
	case PlayingField::all:
		bool res = scan(x, y, horizon);
		res += scan(x, y, vertically);
		return res;
		break;
	}
	return 0;
}

void PlayingField::filler(const int & x, const int & y, direction _direction)
{
	int delta;
	switch (_direction)
	{
	case PlayingField::horizon:
		delta = 0;
		while (x + delta < 10 && enemy[0][y][x + delta++] != water) enemy[0][y][x + delta++] = water;
		delta = 0;
		while (x - delta < 10 && enemy[0][y][x - delta++] != water) enemy[0][y][x - delta++] = water;
		break;
	case PlayingField::vertically:
		delta = 0;
		while (y + delta < 10 && enemy[0][y + delta++][x] != water) enemy[0 + delta++][y][x] = water;
		delta = 0;
		while (y - delta < 10 && enemy[0][y - delta++][x] != water) enemy[0][y - delta++][x] = water;
		break;
	}
}

int PlayingField::smallestShip(int minimum)
{
	for (int i = 0; i < 4; i++) {
		if (enemySails[i] > 0 && i >= minimum) return i;
	}
	return -1;
}


PlayingField::PlayingField()
{
	for (int i = 0; i < 4; i++) deck[i] = enemySails[i] = 4 - i;
	numberOfInitializedShips = 0;
	overallHealth = 20;

	searchPoints1 = 24;
	searchPoints2 = 26;
	searchPoints3 = 50;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) enemy[0][i][j] = empty;
	overallEnemyHealth = 20;

	lHitx1 = lHity1 = -1;
	lHitx2 = lHity2 = -1;
}