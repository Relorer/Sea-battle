#include "pch.h"
#include "NewString.h"

int NewString::assignID()
{
	static int currentID;
	return currentID++;
}

void NewString::print(FILE * filestream)
{
	if (str != nullptr) {
		char *strCopy = str;
		while (*strCopy) putc(*strCopy++, filestream);
	}
}

void NewString::print(const char escapeSequence, FILE * filestream)
{
	if (str != nullptr) {
		char *strCopy = str;
		while (*strCopy) putc(*strCopy++, filestream);
	}
	if (escapeSequence) putc(escapeSequence, filestream);
}

void NewString::print(const int rightLim, int leftLim, const char escapeSequence, FILE * filestream)
{
	if (leftLim == -1) leftLim = this->length - 1;

#ifdef DEBUG // validation of arguments
	if (rightLim < 0 || rightLim >= length) {
		printf("NewString.print(): rightLim выходит за границы массива");
		exit(0);
	}
	if (leftLim < 0 || leftLim >= length) {
		printf("NewString.print(): leftLim выходит за границы массива");
		exit(0);
	}
#endif // DEBUG 

	for (int i = rightLim; i <= leftLim; i++) {
		putc(str[i], filestream);
	}
	if (escapeSequence) putc(escapeSequence, filestream);
}

int NewString::printToBinaryFileAsNum(FILE * filestream)
{
	fpos_t position;
	fgetpos(filestream, &position);
	int count = 0;
	int currentNum = 0;
	int degree = 1;
	bool isitnum = 0;
	for (int i = length - 1; i > 0; i--) {
		if (this->str[i] <= '9' && this->str[i] >= '0') {
			isitnum = 1;
			currentNum += (int(this->str[i]) - 48) * degree;
			degree *= 10;
		}
		if (isitnum && !(this->str[i - 1] <= '9' && this->str[i - 1] >= '0')) {
			if (this->str[i - 1] == '-') currentNum = -currentNum;
			fwrite(&currentNum, sizeof(int), 1, filestream);
			currentNum = 0;
			degree = 0;
			isitnum = 0;
			count++;
		}
		if (!(i - 1)) {
			currentNum += (int(this->str[i - 1]) - 48) * degree;
			fwrite(&currentNum, sizeof(int), 1, filestream);
			i = 0;
			count++;
		}
	}

	for (int i = 0; i < count / 2; i++) {
		int left, right;
		fsetpos(filestream, &position);
		fread(&left, sizeof(int), 1, filestream);
		int offset = -1 * ((i + 1) * sizeof(int));
		fseek(filestream, offset, SEEK_END);
		fread(&right, sizeof(int), 1, filestream);
		left += right;
		right = left - right;
		left = left - right;
		fsetpos(filestream, &position);
		fwrite(&left, sizeof(int), 1, filestream);
		fgetpos(filestream, &position);
		fseek(filestream, offset, SEEK_END);
		fwrite(&right, sizeof(int), 1, filestream);
	}
	fseek(filestream, 0, SEEK_END);
	return count;
}

int NewString::getLength()
{
	return this->length;
}

char * NewString::getStr()
{
	return str;
}

void NewString::getns(const char lineEnd, FILE * filestream)
{
	char buffer[BUFSIZ];
	int i;
	do {
		i = 0;
		if ((buffer[i] = getc(filestream)) != '\n') i++;
		do {
			buffer[i] = getc(filestream);
		} while (buffer[i] != '\n' && buffer[i] != lineEnd && i++ < BUFSIZ - 2);
		buffer[i] = '\0';
		*this += buffer;
	} while (i == BUFSIZ - 1);
}

void NewString::getns(int maxsize, const char lineEnd, FILE * filestream)
{
	if (maxsize > 0) {
		char buffer[BUFSIZ];
		int i;
		do {
			i = 0;
			buffer[i] = getc(filestream);
			if (buffer[i] != '\n' && buffer[i] != lineEnd) {
				i++;
				maxsize--;
			}
			do {
				buffer[i] = getc(filestream);
			} while (buffer[i] != '\n' && buffer[i] != lineEnd && i++ < BUFSIZ - 2 && --maxsize);
			buffer[i] = '\0';
			*this += buffer;
		} while (i == BUFSIZ - 1);
	}
}

void NewString::convertLowercase()
{
	if (str != nullptr) {
		for (int i = 0; i < length; i++) {
			if (this->str[i] <= 'Z' && this->str[i] >= 'A') this->str[i] = (this->str[i] + 32);
			else if (this->str[i] <= 'Я' && this->str[i] >= 'А') this->str[i] = (this->str[i] + 32);
			else if (this->str[i] == 'Ё') this->str[i] = 'ё';
		}
	}
}

int NewString::countWords(const char separator)
{
	int n = 0;
	int i = 0;
	if (this->str[i] != separator) {
		i++;
		n++;
	}
	for (; i < length - 1; i++) {
		if (this->str[i] == separator && this->str[i + 1] != separator) {
			n++;
			i++;
		}
	}
	return n;
}


NewString::NewString()
{
	str = nullptr;
	length = 0;
	ID = assignID();
}

NewString::NewString(const char * str)
{
	length = strlen(str);
	this->str = new char[length + 1];
	for (int i = 0; i < length; i++) {
		this->str[i] = str[i];
	}
	this->str[length] = '\0';
	ID = assignID();
}

NewString::NewString(const NewString & anotherStr)
{
	this->length = strlen(anotherStr.str);
	this->str = new char[length + 1];
	for (int i = 0; i < length; i++) {
		this->str[i] = anotherStr.str[i];
	}
	this->str[length] = '\0';
}

NewString::NewString(NewString && anotherStr)
{
	this->length = strlen(anotherStr.str);
	this->str = anotherStr.str;
	anotherStr.str = nullptr;
}



NewString & NewString::operator=(const NewString & anotherStr)
{
	if (&anotherStr == this) return *this;
	if (this->str != nullptr) delete[] this->str;
	if (anotherStr.str == nullptr) {
		this->length = 0;
		this->str = nullptr;
		return *this;
	}
	this->length = anotherStr.length;
	this->str = new char[length + 1];
	for (int i = 0; i < length; i++) {
		this->str[i] = anotherStr.str[i];
	}
	this->str[length] = '\0';
	return *this;
}

bool NewString::operator==(const NewString & anotherStr)
{
	if (this->length != anotherStr.length) return 0;
	for (int i = 0; i < length; i++)
		if (this->str[i] != anotherStr.str[i]) return 0;
	return 1;
}

bool NewString::operator!=(const NewString & anotherStr)
{
	if (this->length != anotherStr.length) return 1;
	for (int i = 0; i < length; i++)
		if (this->str[i] != anotherStr.str[i]) return 1;
	return 0;
}

NewString NewString::operator+(const NewString & anotherStr)
{
	NewString resultStr;
	resultStr.length = this->length + anotherStr.length;
	resultStr.str = new char[resultStr.length + 1];
	int i = 0;
	for (; i < this->length; i++) {
		resultStr.str[i] = this->str[i];
	}
	for (int j = 0; j < anotherStr.length; j++, i++) {
		resultStr.str[i] = anotherStr.str[j];
	}
	resultStr.str[resultStr.length] = '\0';
	return resultStr;
}

NewString NewString::operator+(const char anotherStr)
{
	NewString resultStr;
	resultStr.length = this->length + 1;
	resultStr.str = new char[resultStr.length + 1];
	int i = 0;
	for (; i < this->length; i++) {
		resultStr.str[i] = this->str[i];
	}
		resultStr.str[i] = anotherStr;
	resultStr.str[resultStr.length] = '\0';
	return resultStr;
}

NewString NewString::operator+=(const NewString & anotherStr)
{
	*this = *this + anotherStr;
	return *this;
}

NewString NewString::operator+=(const char anotherStr)
{
	*this = *this + anotherStr;
	return *this;
}

char & NewString::operator[](const int index)
{

#ifdef DEBUG // validation of arguments
	if (index < 0 || index >= length) {
		printf("NewString[]: Index выходит за границы массива");
		exit(0);
	}
#endif // DEBUG

	return this->str[index];
}

NewString::~NewString()
{
	delete[] this->str;
}

int NewString::findSubstring(const NewString & substring, int startPoint)
{
	for (int i = startPoint, count = 0; i < this->length; i++) {
		if (this->str[i] == substring.str[count]) count++;
		else count = 0;
		if (count == substring.length)
			return i + 1 - substring.length;
	}
	return -1;
}

int NewString::findChar(const char & ch, int startPoint)
{
	for (int i = startPoint; i < this->length; i++) {
		if (this->str[i] == ch) return i;
	}
	return -1;
}

bool NewString::checkForCorrectness(const NewString & substring)
{
	for (int i = 0; i < substring.length; i++) {
		if (this->findChar(substring.str[i]) != -1) return 1;
	}
	return 0;
}

bool askQuestion()
{
	while (true) {
		printf("да/нет: ");
		NewString answer;
		answer.getns();
		answer.convertLowercase();
		if (answer == "да" || answer == "д" || answer == "yes" || answer == "y" || answer == "+") return 1;
		else if (answer == "нет" || answer == "н" || answer == "no" || answer == "n" || answer == "-") return 0;
		else printf("Введен некорректный ответ, попробуйте  ещё\n");
	}
}

int askSelect(int min, int max)
{
	int selected;
	while (true) {
		printf("Введите соответствующий номер: ");
		scanf_s("%d", &selected);
		if (selected <= max && selected >= min) return selected;
		printf("Вариант %d не существует, попробуйте ещё\n", selected);
	}
}
