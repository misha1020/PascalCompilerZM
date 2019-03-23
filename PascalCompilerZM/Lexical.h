#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "DECKEY.h"
#include "DECW.h"

using namespace std;
using namespace boost;

struct textPosition
{
	int lineNumber;
	int charNumber;
	int errNumber;
};

struct lexems
{
	int lineNumber;
	int charNumber;
	int lexem;
};

const int MAX_ERR_COUNT = 20;
extern int currErrorsCount;
extern bool isComment;
extern textPosition errPositions[MAX_ERR_COUNT];

extern int lexemsCount;
extern 	int currLineNum;
extern lexems*  allLexems;

vector<int> GetNextLexems(string currentLine, int lineNum);
void AddErrorToTable(int lineNum, int charNum, int errNum);