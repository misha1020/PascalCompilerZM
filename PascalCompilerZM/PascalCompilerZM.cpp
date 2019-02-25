#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

const int MAX_ERR_COUNT = 20;

struct textPosition
{
	int lineNumber;
	int charNumber;
	int errNumber;

	textPosition(int line, int chr, int err)
	{
		lineNumber = line;
		charNumber = chr;
		errNumber = err;
	}

	textPosition()
	{

	}
};

void NextCh(textPosition *errPos, ifstream& inFile, ofstream& outFile, map<int, string> errorsMap);

int main()
{
	setlocale(LC_ALL, "Russian");
	vector<string> errorsVec;
	map<int, string> errorsMap;
	int allErrorsCount = 0;
	ifstream fErrMsgs("Err.msg");
	string errorString = "";
	while (getline(fErrMsgs, errorString))
	{

		split(errorsVec, errorString, [](char c) {return c == ':'; });
		int errorNum = lexical_cast<int>(errorsVec[0]);
		errorsMap[errorNum] = errorsVec[1];
	}
	fErrMsgs.close();

	// Эту часть кода вынести в функцию, которая будет при вызове добавлять новую ошибку в список ошибок
	textPosition errPositions[20];
	ifstream fErrors("ErrTable.txt");
	int currErrorsCount = 0;
	while (!fErrors.eof() && currErrorsCount < MAX_ERR_COUNT)
	{
		fErrors >> errPositions[currErrorsCount].lineNumber;
		fErrors >> errPositions[currErrorsCount].charNumber;
		fErrors >> errPositions[currErrorsCount].errNumber;
		currErrorsCount++;
	}

	ofstream fResult;
	fResult.open("Result.lst");
	ifstream fPascalCode;
	fPascalCode.open("PascalCode.txt");
	NextCh(errPositions, fPascalCode, fResult, errorsMap);
	fPascalCode.close();
	if (currErrorsCount < 20)
		fResult << endl << "Кoмпиляция окончена, ошибок: " << currErrorsCount << "!";
	else
		fResult << endl << "Компиляция окончена, ошибок больше 20!";
	fResult.close();

	system("pause");
	return 0;
}

void NextCh(textPosition *errPos, ifstream& inFile, ofstream& outFile, map<int, string> errorsMap)
{
	outFile << "				Работает ZM-компилятор" << endl;
	outFile << "				Листинг программы:" << endl;
	int currLineNum = 0;
	int lastError = 0;
	string currentLine = "", errorLine = "", isLineNumLesTen = "";
	char currLiter;
	while (!inFile.eof())
	{
		getline(inFile, currentLine);
		for (int i = 0; i < currentLine.length(); i++)
		{
			currLiter = currentLine[i];
			// здесь что-то происходит с литерами
		}
		if (currLineNum < 10)
			isLineNumLesTen = "   ";
		else
			isLineNumLesTen = "   ";
		outFile << isLineNumLesTen << currLineNum + 1 << "   " << currentLine << endl;
		while (errPos[lastError].lineNumber - 1 == currLineNum)
		{
			for (int i = 0; i < errPos[lastError].charNumber - 1; i++)
				errorLine += " ";
			errorLine += "^ ошибка код ";
			if (lastError < 10)
				isLineNumLesTen = "**0";
			else
				isLineNumLesTen = "**";
			outFile << isLineNumLesTen << lastError + 1 << "** " << errorLine << errPos[lastError].errNumber << endl;
			outFile << "****** " << errorsMap[errPos[lastError].errNumber] << endl;
			lastError++;

			errorLine = "";
		}
		currLineNum++;
	}
}

//void NextCh(textPosition *errPos, ifstream& inFile, ofstream& outFile, string *errArray)
//{
//	int currLineNum = 0;
//	int lastError = 0;
//	string currentLine = "", errorLine = "";
//	char currLiter;
//	while (!inFile.eof())
//	{
//		inFile >> currLiter;
//		currentLine += currLiter;
//		if (currLiter == ' ')
//			cout << "LOL!" << endl;
//		if (inFile.peek() == '\n' || inFile.peek() == EOF)
//		{
//			outFile << currentLine << endl;
//			currentLine = "";
//			while (errPos[lastError].lineNumber - 1 == currLineNum)
//			{
//				for (int i = 0; i < errPos[lastError].charNumber - 1; i++)
//					errorLine += " ";
//				errorLine += "^ ошибка код ";
//				outFile << errorLine << errPos[lastError].errNumber << endl;
//				outFile << errArray[errPos[lastError].errNumber] << endl;
//				lastError++;
//				errorLine = "";
//			}
//			currLineNum++;
//		}
//	}
//}