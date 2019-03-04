#pragma once

#include <string>
#include <vector>
#include <map>
#include <regex>
#include <fstream>
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

const int MAX_ERR_COUNT = 20;
extern int currErrorsCount;
extern bool isComment;
extern textPosition errPositions[MAX_ERR_COUNT];

vector<int> GetNextLexems(string currentLine, int lineNum);