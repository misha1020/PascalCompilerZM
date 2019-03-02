#include "pch.h"

#include "Lexical.h"

textPosition errPositions[MAX_ERR_COUNT];

vector<int> GetNextLexems(string currentLine)
{
	vector<int> currentLexems;
	char currLiter;
	for (int i = 0; i < currentLine.length(); i++)
	{
		currLiter = currentLine[i];
		
		// здесь что-то происходит с литерами
	}

	return currentLexems;
}