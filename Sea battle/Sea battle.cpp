#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <time.h>
#include "PlayingField.h"
#include "NewString.h"
#include "randset.h"

void loadingAnimation(NewString str = "") {
	str.print();
	Sleep(generateRandomInTheRange(100, 200));
	printf(".");
	Sleep(generateRandomInTheRange(100, 200));
	printf(".");
	Sleep(generateRandomInTheRange(100, 200));
	printf(".");
	Sleep(generateRandomInTheRange(100, 200));
	printf("\r");
	for (int i = 0; i < 100; i++) printf(" ");
	printf("\r");
	str.print();
}

void randomSpeech(NewString * phrases, int size, const char escapeSequence = '\0') {
	phrases[generateRandomInTheRange(0, size - 1)].print(escapeSequence);
}

void help() {
	printf("\nСправка\n");
	printf("1. Размеры игрового поля 10 на 10, 10 букв по вертикали (А, Б, В, Г, Д, Е, Ж, З, И, К) и 10 чисел по горизонтали (1-10)\n");
	printf("2. На игровом поле размещаются 10 кораблей: 1 четырехпалубный, 2 трехпалубных, 3 двухпалубных, 4 однопалубных\n");
	printf("3. В начале игры вы можете создать цифровую модель своего поля, что позволит AI самостоятельно обрабатывать свои выстрелы.\n");
	printf("4. Если вы не создали модель из предыдущего пункта, то вам придется передавать результат выстрела AI: Мимо, Попал, Убил (каждую из команд можно заменить первой буквой слова: м, п, у)\n");
	printf("5. Координаты конкретной клетки поля принимаются в формате: буква + число (А1, Б5, К10)\n");
	printf("6. Координаты корабля задаются координатами его крайних клеток, разделенных пробелом (Б1 Б3). Для катеров необходимо два раза написать одну координату (А1 А1)\n\n");
	Sleep(1000);
}

bool flipСoin() {
	printf("Выберете сторону: \n");
	printf("1. Орёл\n");
	printf("2. Решка\n");
	if (askSelect(1, 2) - 1) {
		loadingAnimation();
		if (generateRandomInTheRange(1, 2) == 1) {
			printf("Решка\n");
			printf("Первый ход за вами\n");
			return 1;
		}
		else {
			printf("Орёл\n");
			printf("Первым ходит AI\n");
			return 0;
		}
	}
	else {
		loadingAnimation();
		if (generateRandomInTheRange(1, 2) == 1) {
			printf("Орёл\n");
			printf("Первый ход за вами\n");
			return 1;
		}
		else {
			printf("Решка\n");
			printf("Первым ходит AI\n");
			return 0;
		}
	}
}

void shotPlayer(PlayingField & fieldAI) {
	int x, y;
	int type;
	NewString phrases[4]{"получил непоправимый урон", "потоплен", "отправлен на дно", "уничтожен"};
	while (true) {
		printf("Ваш выстрел: ");
		if (fieldAI.getPos(x, y)) {
			printf("Введены не корректные координаты, попробуйте еще\n");
			continue;
		}
		break;
	}
	int result = fieldAI.hitDetection(x, y, type);
	if (result == PlayingField::miss) {
		printf("Мимо\n");
	}
	else if (result == PlayingField::hit) {
		printf("Попал\n");
	}
	else if (result == PlayingField::killed) {
		switch (type)
		{
		case 0:
			printf("Вражеский катер ");
			randomSpeech(phrases, 4, '\n');
			break;
		case 1:
			printf("Вражеский Эсминец ");
			randomSpeech(phrases, 4, '\n');
			break;
		case 2:
			printf("Вражеский Крейсер ");
			randomSpeech(phrases, 4, '\n');
			break;
		case 3:
			printf("Вражеский Линкор ");
			randomSpeech(phrases, 4, '\n');
			break;
		}
	}
	if (result != PlayingField::miss && fieldAI.getOverallHealth() && fieldAI.getOverallEnemyHealth()) {
		shotPlayer(fieldAI);
	}
}

void shotAI(PlayingField & fieldPlayer, PlayingField & fieldAI, bool mode = 1) 
{
	int x, y;
	int type;
	fieldAI.shoot(x, y);
	loadingAnimation("Выстрел AI: ");
	fieldAI.coordinatesToLine(x, y).print('\n');
	PlayingField::resultShot result = PlayingField::miss;
	if (mode) {
		result = fieldPlayer.hitDetection(x, y, type);
		if (result == PlayingField::hit) {
			NewString phrasesDestroyer[2]{ "Наш Эсминец ранен", "На нашем Эсминце начался пожар" };
			switch (type)
			{
			case 1:
				randomSpeech(phrasesDestroyer, 2, '\n');
				break;
			case 2:
				printf("Наш Крейсер ранен\n");
				break;
			case 3:
				printf("Наш Линкор ранен\n");
				break;
			}
		}
		else if (result == PlayingField::killed) {
			NewString phrasesBoat[2]{ "Наш катер уничтожен", "Связь с катером потеряна" };
			NewString phrasesDestroyer[2]{ "Наш Эсминец уничтожен", "Связь с Эсминцем потеряна" };
			NewString phrasesCruiser[3]{ "Наш Крейсер уничтожен", "Связь с Крейсером потеряна", "История нашего Крейсера подошла к концу" };
			NewString phrasesBattleship[3]{ "Наш Линкор уничтожен", "Связь с Линкором потеряна", "Наш Линкор начал погружение" };
			switch (type)
			{
			case 0:
				randomSpeech(phrasesBoat, 2, '\n');
				break;
			case 1:
				randomSpeech(phrasesDestroyer, 2, '\n');
				break;
			case 2:
				randomSpeech(phrasesCruiser, 3, '\n');
				break;
			case 3:
				randomSpeech(phrasesBattleship, 3, '\n');
				break;
			}
		}
	}
	else {
		NewString res;
		do {
			printf("Результат выстрела: ");
			res = "";
			res.getns();
			res.convertLowercase();
			if (res == "м" || res == "мимо") result = PlayingField::miss;
			else if (res == "п" || res == "попал") result = PlayingField::hit;
			else if (res == "у" || res == "убил") result = PlayingField::killed;
			else {
				printf("Введен не корректный ответ, попробуйте еще\n");
				continue;
			}
			break;
		} while (true);
	}
	fieldAI.hitResultDetection(x, y, result);
	if (result != PlayingField::miss && fieldAI.getOverallHealth() && fieldAI.getOverallEnemyHealth()) {
		shotAI(fieldPlayer, fieldAI, mode);
	}
}

enum queue { Player = 0, AI };

int main()
{
	system("mode con cols=102 lines=25");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(0));
	bool mode;

	PlayingField fieldAI;
	fieldAI.initializeTheRandomFleet();
	PlayingField fieldPlayer;
	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cd;
	cd.X = 44;
	cd.Y = 3;
	SetConsoleCursorPosition(hd, cd);
	printf("Морской бой\n");
	printf("Желаете открыть справку по игре?\n");
	if (askQuestion()) help();
	printf("Нарисуйте своё поле и расположите на нем корабли\n");
	printf("Для более комфортной игры рекомендуется заполнить цифровую модель вашего поля. Заполнить?\n");
	if (mode = askQuestion()) {
		do {
			fieldPlayer.clearTheField();
			printf("По очереди вводите координаты крайних клеток кораблей\n");
			int x1, x2, y1, y2;
			for (int i = 0; i < 10; i++) {
				while (true)
				{
					printf("Введите координаты %d-го корабля: ", i + 1);
					if (fieldPlayer.getPos(x1, y1) || fieldPlayer.getPos(x2, y2)) {
						printf("Введены не корректные координаты, попробуйте еще\n");
						continue;
					}
					if (fieldPlayer.addShip(x1, x2, y1, y2)) {
						printf("%d %d %d %d", x1, x2, y1, y2);
						printf("По данным координатам не может быть размещен корабль, попробуйте еще\n");
						continue;
					}
					break;
				}

			}
			printf("Ваше поле после заполнения\n\n");
			fieldPlayer.printField();
			printf("\n\nЖелаете продолжить игру с таким полем?\n");
			printf("1. Да\n");
			printf("2. Нет. Повторить заполнение поля\n");
		} while (askSelect(1, 2) - 1);
	} // if (askQuestion())

	printf("Кто ходит первым:\n");
	printf("1. Я\n");
	printf("2. AI\n");
	printf("3. Подбросить монетку\n");
	int selected = askSelect(1, 3);
	if (selected == 3) selected = flipСoin() ? 1 : 2;

	queue _queue;

	if (selected == 1) _queue = Player;
	else _queue = AI;

	do {
		if (_queue == Player) {
			shotPlayer(fieldAI);
			_queue = AI;
		}
		else {
			shotAI(fieldPlayer, fieldAI, mode);
			_queue = Player;
		}
	} while (fieldAI.getOverallHealth() && fieldAI.getOverallEnemyHealth());
	if (fieldAI.getOverallHealth()) printf("Вы проиграли(\n");
	else printf("Вы выиграли\n");
	system("pause");
}