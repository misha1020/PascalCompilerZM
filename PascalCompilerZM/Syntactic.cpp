#include "pch.h"

#include "Syntactic.h"

int lexemNumber = 0;


void Accept(int currentSym, int neededSym, int lineNum, int posNum)
{
	if (currentSym != neededSym)
	{
		AddErrorToTable(lineNum, posNum, neededSym);
		cout << "Ошибка " << neededSym << " в строке " << lineNum << " в символе " << posNum << " добавлена в таблицу ошибок!" << endl;
	}
}

void ProgramParsing()
{
	//if (allLexems.size > 0)
	{
	//	Accept()
	}
}

void Parsing(ifstream& fSym)
{
	//int currLineNum = 0;
	//while (!fSym.eof())
	//{
	//	string readLine;
	//	getline(fSym, readLine);
	//	string currentNum = "";
	//	for (int i = 0; i < readLine.length(); i++)
	//		if (readLine[i] != ' ')
	//			currentNum += readLine[i];
	//		else
	//			if (currentNum != "")
	//			{
	//				allLexems[lexemsCount].lexem = lexical_cast<int>(currentNum);
	//				allLexems[lexemsCount].lineNumber = currLineNum;
	//				allLexems[lexemsCount].charNumber = i;
	//				lexemsCount++;
	//				currentNum = "";
	//			}
	//	currLineNum++;
	//}

	for (int i = 0; i < lexemsCount; i++)
		cout << allLexems[i].lexem << "  " << allLexems[i].lineNumber << "  " << allLexems[i].charNumber << endl;

	//ProgramParsing();
}