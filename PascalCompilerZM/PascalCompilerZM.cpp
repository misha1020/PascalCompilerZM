#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct textPosition
{
	int lineNumber;
	int charNumber;
	int errNumber;
};

void NextCh(textPosition *errPos, ifstream& inFile, ofstream& outFile, string *errArray);

int main()
{
	setlocale(LC_ALL, "Russian");
	string *errorsArray = new string[500];									// INT_MAX
	int allErrorsCount = 0;
	ifstream fErrMsgs("Err.msg");
	while (getline(fErrMsgs, errorsArray[allErrorsCount]))
	{
		allErrorsCount++;
	}
	fErrMsgs.close();

	textPosition errPositions[500];
	ifstream fErrors("ErrTable.txt");
	int currErrorsCount = 0;
	while (!fErrors.eof())
	{
		fErrors >> errPositions[currErrorsCount].lineNumber;
		fErrors >> errPositions[currErrorsCount].charNumber;
		fErrors >> errPositions[currErrorsCount].errNumber;
		currErrorsCount++;
	}

	ofstream fResult;
	fResult.open("Result.txt");
	ifstream fPascalCode;
	fPascalCode.open("PascalCode.txt");
	NextCh(errPositions, fPascalCode, fResult, errorsArray);

	fResult.close();
	fPascalCode.close();
	//for (int i = 0; i < currErrorsCount; i++)
	//	cout << "Считанная строка: " << errPositions[i].lineNumber << errPositions[i].charNumber << errPositions[i].errNumber << endl;
	//for (int i = 0; i < allErrorsCount; i++)
	//	cout << "Считанная строка: " << errorsArray[i] << endl;

	system("pause");
	return 0;
}

void NextCh(textPosition *errPos, ifstream& inFile, ofstream& outFile, string *errArray)
{
	int currLineNum = 0;
	int lastError = 0;
	string currentLine = "", errorLine = "";
	char currLiter;
	while (!inFile.eof())
	{
		inFile >> currLiter;
		currentLine += currLiter;
		if (currLiter == ' ')
			cout << "LOL!" << endl;
		if (inFile.peek() == '\n' || inFile.peek() == EOF)
		{
			outFile << currentLine << endl;
			currentLine = "";
			while (errPos[lastError].lineNumber - 1 == currLineNum)
			{
				for (int i = 0; i < errPos[lastError].charNumber - 1; i++)
					errorLine += " ";
				errorLine += "^ ошибка код ";
				outFile << errorLine << errPos[lastError].errNumber << endl;
				outFile << errArray[errPos[lastError].errNumber] << endl;
				lastError++;
				errorLine = "";
			}
			currLineNum++;
		}
	}
}