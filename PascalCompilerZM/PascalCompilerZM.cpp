#include "pch.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "Lexical.h"

using namespace std;
using namespace boost;


void GetNextLine(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap, ofstream& fSym);

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
	ofstream fResult;
	fResult.open("Result.lst");
	ifstream fPascalCode;
	fPascalCode.open("PascalCode.txt");
	ofstream fSym;
	fSym.open("Sym.txt");
	GetNextLine(fPascalCode, fResult, errorsMap, fSym);
	fPascalCode.close();
	if (currErrorsCount < 20)
		fResult << endl << "Кoмпиляция окончена, ошибок: " << currErrorsCount << "!";
	else
		fResult << endl << "Компиляция окончена, ошибок больше 20!";
	fResult.close();

	system("pause");
	return 0;
}



void GetNextLine(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap, ofstream& fSym)
{
	outFile << "				Работает ZM-компилятор" << endl;
	outFile << "				Листинг программы:" << endl;
	vector<int> nextLexemsVec;
	int currLineNum = 0;
	int lastError = 0;
	string currentLine = "", errorLine = "", isLineNumLessTen = "";

	while (!inFile.eof())
	{
		getline(inFile, currentLine);
		to_lower(currentLine);

		nextLexemsVec = GetNextLexems(currentLine, currLineNum);

		for (int j = 0; j < nextLexemsVec.size(); j++)
			fSym << nextLexemsVec[j] << "   ";
		fSym << endl;

		if (currLineNum < 9)
			isLineNumLessTen = "   ";
		else
			isLineNumLessTen = "  ";
		outFile << isLineNumLessTen << currLineNum + 1 << "   " << currentLine << endl;
		while (errPositions[lastError].lineNumber == currLineNum && errPositions[lastError].errNumber != 0)
		{
			for (int i = 0; i < errPositions[lastError].charNumber - 1; i++)
				errorLine += " ";
			errorLine += "^ ошибка код ";
			if (lastError < 10)
				isLineNumLessTen = "**0";
			else
				isLineNumLessTen = "**";
			outFile << isLineNumLessTen << lastError + 1 << "** " << errorLine << errPositions[lastError].errNumber << endl;
			outFile << "****** " << errorsMap[errPositions[lastError].errNumber] << endl;
			lastError++;

			errorLine = "";
		}
		currLineNum++;
	}
}