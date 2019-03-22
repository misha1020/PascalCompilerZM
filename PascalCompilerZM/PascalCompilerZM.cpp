﻿#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "Lexical.h"
#include "Syntactic.h"

using namespace std;
using namespace boost;

void GetNextLine(ifstream& inFile, ofstream& fSym);
void Print(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap);

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

	ifstream fPascalCode;
	fPascalCode.open("PascalCode.txt");
	//fPascalCode.open("Code.txt");

	ofstream fSymWrite;
	fSymWrite.open("Sym.txt");
	GetNextLine(fPascalCode,  fSymWrite);
	fSymWrite.close();
	//fSymWrite.open("Sym.txt");

	//fSymWrite.close();

	fPascalCode.close();

	Parsing();

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
	int currLineNum = 0;
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
	fSym  << 999;
	allLexems[lexemsCount].lexem = 999;
	allLexems[lexemsCount].lineNumber = currLineNum;
	allLexems[lexemsCount].charNumber = 0;
	lexemsCount++;
}

void Print(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap)
{
	outFile << "				Работает ZM-компилятор" << endl;
	outFile << "				Листинг программы:" << endl;
	string isLineNumLessTen = "", errorLine = "", currentLine = "";
	int currLineNum = 0, lastError = 0;

	while (!inFile.eof())
	{
		getline(inFile, currentLine);
		if (currLineNum < 9)
			isLineNumLessTen = "   ";
		//else
		//	isLineNumLessTen = "  ";
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
	AddErrorToTable(currLineNum, 3, 61);

	//outFile << isLineNumLessTen << currLineNum + 1 << "   " << 999 << endl;
	//errorLine += "^ ошибка код ";
	//if (lastError < 9)
	//	isLineNumLessTen = "**0";
	//outFile << isLineNumLessTen << lastError + 1 << "** " << errorLine << errPositions[lastError].errNumber << endl;
	//outFile << "****** " << errorsMap[errPositions[lastError].errNumber] << endl;

	if (currErrorsCount < MAX_ERR_COUNT)
		outFile << endl << "Кoмпиляция окончена, ошибок: " << currErrorsCount << "!";
	else
		outFile << endl << "Компиляция окончена, ошибок больше " << MAX_ERR_COUNT << "!";
}