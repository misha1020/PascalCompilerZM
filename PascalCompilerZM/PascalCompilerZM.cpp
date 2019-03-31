#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "Lexical.h"
#include "Syntactic.h"
#include "starters_and_followers.h"

using namespace std;
using namespace boost;

int currLineNum = 0;

void GetNextLine(ifstream& inFile, ofstream& fSym);
void Print(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap);
void Sort(textPosition *arr, int size);

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
		split(errorsVec, errorString, [](char c) {return c == '?'; });
		int errorNum = lexical_cast<int>(errorsVec[0]);
		errorsMap[errorNum] = errorsVec[1];
	}
	fErrMsgs.close();

	for (int i = 0; i < MAX_ERR_COUNT; i++)
		errPositions[i].lineNumber = 999;

	ifstream fPascalCode;
	fPascalCode.open("PascalCode.txt");

	ofstream fSymWrite;
	fSymWrite.open("Sym.txt");
	GetNextLine(fPascalCode,  fSymWrite);
	fSymWrite.close();
	fPascalCode.close();

	Program();
	Sort(errPositions, MAX_ERR_COUNT);

	ofstream fResult;
	fResult.open("Result.lst");
	fPascalCode.open("PascalCode.txt");
	Print(fPascalCode, fResult, errorsMap);
	fPascalCode.close();
	fResult.close();

	system("pause");
	return 0;
}



void GetNextLine(ifstream& inFile, ofstream& fSym)
{
	vector<int> nextLexemsVec;
	currLineNum = 0;
	string currentLine = "";

	while (!inFile.eof())
	{
		getline(inFile, currentLine);
		to_lower(currentLine);
		nextLexemsVec = GetNextLexems(currentLine, currLineNum);
		for (int j = 0; j < nextLexemsVec.size(); j++)
			fSym << nextLexemsVec[j] << "   ";
		fSym << endl;
		currLineNum++;
	}
}

void Print(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap)
{
	outFile << "								Работает ZM-компилятор"				 << endl;
	outFile << "								Листинг программы:"					 << endl;
	string isLineNumLessTen = "", errorLine = "", currentLine = "";
	int lastError = 0;
	currLineNum = 0;

	while (!inFile.eof())
	{
		getline(inFile, currentLine);
		if (currLineNum < 9)
			isLineNumLessTen = "   ";
		else
			isLineNumLessTen = "  ";
		outFile << isLineNumLessTen << currLineNum + 1 << "   " << currentLine << endl;

		while (errPositions[lastError].lineNumber == currLineNum && errPositions[lastError].errNumber != 0)
		{
			for (int i = 0; i < errPositions[lastError].charNumber; i++)
				errorLine += " ";
			errorLine += "^ ошибка код ";
			if (lastError < 9)
				isLineNumLessTen = "**0";
			else
				isLineNumLessTen = "**";
			outFile << isLineNumLessTen << lastError + 1 << "** " << errorLine << errPositions[lastError].errNumber << endl;
			outFile << "****** " << errorsMap[errPositions[lastError].errNumber] << endl;
			lastError++;
			errorLine = "";
		}
		currLineNum++;
		if (currLineNum > 9)
			isLineNumLessTen = "  ";

	}

	if (currErrorsCount < MAX_ERR_COUNT)
		outFile << endl << "Кoмпиляция окончена, ошибок: " << currErrorsCount << "!";
	else
		outFile << endl << "Компиляция окончена, ошибок больше " << MAX_ERR_COUNT << "!";
}

void Sort(textPosition *arr, int size)
{
	for (int i = 0; i < size - 1; ++i)
		for (int j = i + 1; j < size; ++j)
			if (arr[i].lineNumber > arr[j].lineNumber) 
				swap(arr[i], arr[j]);
}