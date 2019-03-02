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

int currErrorsCount = 0;

void GetNextLine(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap);
void AddErrorToTable(int lineNum, int charNum, int errNum);

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
	GetNextLine(fPascalCode, fResult, errorsMap);
	fPascalCode.close();
	if (currErrorsCount < 20)
		fResult << endl << "Кoмпиляция окончена, ошибок: " << currErrorsCount << "!";
	else
		fResult << endl << "Компиляция окончена, ошибок больше 20!";
	fResult.close();

	system("pause");
	return 0;
}

void AddErrorToTable(int lineNum, int charNum, int errNum)
{
	errPositions[currErrorsCount].lineNumber = lineNum;
	errPositions[currErrorsCount].charNumber = charNum;
	errPositions[currErrorsCount].errNumber = errNum;
	currErrorsCount++;
}

void GetNextLine(ifstream& inFile, ofstream& outFile, map<int, string> errorsMap)
{
	outFile << "				Работает ZM-компилятор" << endl;
	outFile << "				Листинг программы:" << endl;
	vector<int> nextLexemsVec;
	int currLineNum = 0;
	int lastError = 0;
	string currentLine = "", errorLine = "", isLineNumLesTen = "";
	while (!inFile.eof())
	{
		getline(inFile, currentLine);

		nextLexemsVec = GetNextLexems(currentLine);

		if (currLineNum < 10)
			isLineNumLesTen = "   ";
		else
			isLineNumLesTen = "  ";
		outFile << isLineNumLesTen << currLineNum + 1 << "   " << currentLine << endl;
		while (errPositions[lastError].lineNumber - 1 == currLineNum)
		{
			for (int i = 0; i < errPositions[lastError].charNumber - 1; i++)
				errorLine += " ";
			errorLine += "^ ошибка код ";
			if (lastError < 10)
				isLineNumLesTen = "**0";
			else
				isLineNumLesTen = "**";
			outFile << isLineNumLesTen << lastError + 1 << "** " << errorLine << errPositions[lastError].errNumber << endl;
			outFile << "****** " << errorsMap[errPositions[lastError].errNumber] << endl;
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