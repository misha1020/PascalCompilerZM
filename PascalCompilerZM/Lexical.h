#pragma once

#include <string>
#include <vector>

#include "DECKEY.h"
#include "DECW.h"

using namespace std;

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
extern textPosition errPositions[MAX_ERR_COUNT];

vector<int> GetNextLexems(string inc);