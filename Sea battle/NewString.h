#pragma once
#include <iostream>

class NewString
{
private:
	char *str;
	int length;
	int ID;
	int assignID();
public:
	void print(FILE * filestream = stdout);
	void print(const char escapeSequence, FILE * filestream = stdout);
	void print(const int rightLim, int leftLim = -1, const char escapeSequence = '\0', FILE * filestream = stdout);
	int printToBinaryFileAsNum(FILE * filestream);
	int getLength();
	char* getStr();
	void getns(const char lineEnd = '\n', FILE * filestream = stdin);
	void getns(int maxsize, const char lineEnd = '\n', FILE * filestream = stdin);
	void convertLowercase();
	int countWords(const char separator = ' ');
	int findSubstring(const NewString &substring, int startPoint = 0);
	int findChar(const char &ch, int startPoint = 0);
	bool checkForCorrectness(const NewString &substring);

	NewString();
	NewString(const char *str);
	NewString(const NewString &anotherStr);
	NewString(NewString &&anotherStr);
	NewString& operator=(const NewString &anotherStr);
	bool operator==(const NewString &anotherStr);
	bool operator!=(const NewString &anotherStr);
	NewString operator+(const NewString &anotherStr);
	NewString operator+(const char anotherStr);
	NewString operator+=(const NewString &anotherStr);
	NewString operator+=(const char anotherStr);
	char& operator[](const int index);
	~NewString();


	//Splitting strings into substrings

	//—делать шаблон метода замены дл€ разных типов данных 

	bool replaceSubstring(const char *substringBefore, const char *substringAfter, const int startPoint = 0) {
		int foundString = this->findSubstring(substringBefore, startPoint);
		if (foundString != -1) {
			this->insertSubstring(foundString, substringAfter, strlen(substringBefore));
			return true;
		}
		return false;
	}

	bool replaceSubstring(NewString substringBefore, NewString substringAfter, const int startPoint = 0) {
		return replaceSubstring(substringBefore.getStr(), substringAfter.getStr(), startPoint);
	}

	void insertSubstring(const int startPoint, const char *substring, int replaceableCharacters = 0) {
		int substringLength = strlen(substring);
		NewString resultStr;
		resultStr.length = this->length + substringLength - replaceableCharacters;
		resultStr.str = new char[resultStr.length + 1];
		int i = 0;
		if (this->length < startPoint) {
			for (; i < this->length; i++) resultStr.str[i] = this->str[i];
			for (; i < startPoint; i++) resultStr.str[i] = ' ';
		}
		else {
			for (; i < startPoint; i++) resultStr.str[i] = this->str[i];
		}
		for (int j = 0; j < substringLength; j++, i++) {
			resultStr.str[i] = substring[j];
		}
		for (int j = i - substringLength + replaceableCharacters; j < this->length; j++, i++) {
			resultStr.str[i] = this->str[j];
		}
		resultStr.str[resultStr.length] = '\0';
		*this = resultStr;
	}

	

	

	//ћетод, удал€ющий пробелы в начале и в конце строки


};

bool askQuestion();
int askSelect(int min = 0, int max = 1);