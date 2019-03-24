#include "pch.h"

#include "Lexical.h"

textPosition errPositions[MAX_ERR_COUNT];
int currErrorsCount = 0;
bool isComment = false;

int lexemsCurrSize = 256;
lexems*  allLexems = new lexems[lexemsCurrSize];
int lexemsCount = 0;

map<string, int> keyWords = {
{"do", dosy},
{"if", ifsy},
{"in", insy},
{"of", ofsy},
{"or", orsy},
{"to", tosy},
{"and", andsy},
{"div", divsy},
{"end", endsy},
{"for", forsy},
{"mod", modsy},
{"nil", nilsy},
{"not", notsy},
{"set", setsy},
{"var", varsy},
{"case", casesy},
{"else", elsesy},
{"file", filesy},
{"goto", gotosy},
{"only", onlysy},
{"then", thensy},
{"type", typesy},
{"unit", unitsy},
{"uses", usessy},
{"with", withsy},
{"array", arraysy},
{"begin", beginsy},
{"const", constsy},
{"label", labelsy},
{"until", untilsy},
{"while", whilesy},
{"downto", downtosy},
{"export", exportsy},
{"import", importsy},
{"module", modulesy},
{"packed", packedsy},
{"record", recordsy},
{"repeat", repeatsy},
{"vector", vectorsy},
{"string", stringsy},
{"forward", forwardsy},
{"process", processsy},
{"program", programsy},
{"segment", segmentsy},
{"function", functionsy},
{"separate", separatesy},
{"interface", interfacesy},
{"procedure", proceduresy},
{"qualified", qualifiedsy},
{"implementation", implementationsy},
{"*", star         },
{"/", slash        },
{"=", equal        },
{",", comma        },
{";", semicolon    },
{":", colon        },
{".", point        },
{"^", arrow        },
{"(", leftpar      },
{")", rightpar     },
{"[", lbracket     },
{"]", rbracket     },
{"{", flpar        },
{"}", frpar        },
{"<", later        },
{">", greater      },
{"<=", laterequal  },
{">=", greaterequal},
{"<>", latergreater},
{"+", plus        },
{"-", minus       },
{"(*", lcomment    },
{"*)", rcomment    },
{":=", assign      },
{"..", twopoints   },
{"eolint", eolint  },
{"false", FALSE    },
{"true", TRUE      }
};

bool IsName(string s)
{
	regex rx("[a-z][a-z | 0-9]*");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsInt(string s)
{
	regex rx("[0-9]+");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsReal(string s)
{
	regex rx("[0-9]+[.]{1}[0-9]+([eE][-]{0,1}[0-9]+)?");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsString(string s)
{
	regex rx("\'.*\'");
	return regex_match(s.begin(), s.end(), rx);
}

void AddErrorToTable(int lineNum, int charNum, int errNum)
{
	if (currErrorsCount < 20)
	{
		errPositions[currErrorsCount].lineNumber = lineNum;
		errPositions[currErrorsCount].charNumber = charNum;
		errPositions[currErrorsCount].errNumber = errNum;
		currErrorsCount++;
	}
}

void ArrayResize()
{
	int sizeNew = lexemsCurrSize * 2;
	lexems*  arrayNew = new lexems[sizeNew];
	memmove(arrayNew, allLexems, sizeof(lexems) * lexemsCurrSize);
	delete[] allLexems;
	allLexems = arrayNew;
	lexemsCurrSize = sizeNew;
}

void AddLexem(int lineNum, int charNum, int lexem)
{
	if (lexemsCount == lexemsCurrSize)
		ArrayResize();
	allLexems[lexemsCount].lineNumber = lineNum;
	allLexems[lexemsCount].charNumber = charNum;
	allLexems[lexemsCount].lexem = lexem;
	lexemsCount++;
}

void IsLexemCorrenct(string lexem, int lineNum, int posNum, vector<int>& currentLexems)
{
	if (lexem != "" && !isComment)
	{
		if (keyWords.find(lexem) != keyWords.end())
		{
			int definer = keyWords.find(lexem)->second;
			AddLexem(lineNum, posNum, definer);
			currentLexems.push_back(definer);
		}
		else
		{
			if (IsName(lexem))
			{
				AddLexem(lineNum, posNum, ident);
				currentLexems.push_back(ident);
			}
			else if (IsInt(lexem))
			{
				int currentInt = lexical_cast<int>(lexem);
				if (currentInt > 32767 || currentInt < -32766)
					AddErrorToTable(lineNum, posNum, 203);
				else
				{
					AddLexem(lineNum, posNum, intc);
					currentLexems.push_back(intc);
				}
			}
			else if (IsReal(lexem))
			{
				vector<string> floatValueParts;
				boost::split(floatValueParts, lexem, boost::is_any_of("e"));
				if (floatValueParts.size() == 2)
				{
					if (floatValueParts[0].length() > 12)
					{
						AddErrorToTable(lineNum, posNum, 207);
						return;
					}

					if (lexical_cast<int>(floatValueParts[1]) > 38)
					{
						AddErrorToTable(lineNum, posNum, 207);
						return;
					}
					else if (lexical_cast<int>(floatValueParts[1]) < -39)
					{
						AddErrorToTable(lineNum, posNum, 206);
						return;
					}
				}
				else
				{
					if (lexem.length() > 41)
					{
						AddErrorToTable(lineNum, posNum, 207);
						return;
					}
				}
				AddLexem(lineNum, posNum, floatc);
				currentLexems.push_back(floatc);
			}
			else if (IsString(lexem))
			{
				string currentChar = lexical_cast<string>(lexem);
				if (currentChar.length() > 257)
				{
					AddErrorToTable(lineNum, posNum, 76);
				}
				else if (currentChar.length() == 3)
				{
					AddLexem(lineNum, posNum, charc);
					currentLexems.push_back(charc);
				}
				else 
				{
					AddLexem(lineNum, posNum, stringc);
					currentLexems.push_back(stringc);
				}
			}
			else
				AddErrorToTable(lineNum, posNum, 6);
		}
	}
}

vector<int> GetNextLexems(string currentLine, int lineNum)
{
	vector<int> currentLexems;
	char currentLiter;
	string currentString = "";

	for (int i = 0; i < currentLine.length(); i++)
	{
		currentLiter = currentLine[i];
		if (currentLiter != ' ')
		{
			if ((currentLiter == '/' || currentLiter == '=' || currentLiter == ',' ||
				currentLiter == ';' || currentLiter == '^' || currentLiter == ')' ||
				currentLiter == '[' || currentLiter == ']' || currentLiter == '{' ||
				currentLiter == '}' || currentLiter == '+') && !isComment)
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				IsLexemCorrenct(string(1, currentLiter), lineNum, i, currentLexems);
				currentString = "";
			}
			else if (currentLiter == '-')
			{
				if (i > 0 && currentString.length() > 0 && currentString[currentString.length() - 1] == 'e')
				{
					currentString += "-";
				}
				else
				{
					IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
					IsLexemCorrenct("-", lineNum, i, currentLexems);
					currentString = "";
				}
			}
			else if (currentLiter == '*')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == ')')
				{
					if (!isComment)
						IsLexemCorrenct("*)", lineNum, i, currentLexems);
					isComment = false;
					i++;

				}
				else if (!isComment)
					IsLexemCorrenct("*", lineNum, i, currentLexems);
				currentString = "";
			}
			else if (currentLiter == '(')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '*')
				{
					isComment = true;
					IsLexemCorrenct("(*", lineNum, i, currentLexems);
					i++;
				}
				else
					IsLexemCorrenct("(", lineNum, i, currentLexems);
				currentString = "";
			}
			else if (currentLiter == '.')
			{
				if (i > 0 && IsInt(string(1, currentLine[i - 1])))
				{
					if (i < currentLine.length() && currentLine[i + 1] == '.')
					{
						IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
						IsLexemCorrenct("..", lineNum, i, currentLexems);
						i++;
						currentString = "";
					}
					else
						currentString += ".";
				}
				else
				{
					IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
					IsLexemCorrenct(".", lineNum, i, currentLexems);
					currentString = "";
				}
			}
			else if (currentLiter == '<')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length())
				{
					currentLiter = currentLine[i + 1];
					if (currentLiter == '>')
					{
						i++;
						IsLexemCorrenct("<>", lineNum, i, currentLexems);
					}
					else if (currentLiter == '=')
					{
						i++;
						IsLexemCorrenct("<=", lineNum, i, currentLexems);
					}
					else
						IsLexemCorrenct("<", lineNum, i, currentLexems);
				}
				else
					IsLexemCorrenct("<", lineNum, i, currentLexems);
				currentString = "";
			}
			else if (currentLiter == '>')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '=')
				{
					i++;
					IsLexemCorrenct(">=", lineNum, i, currentLexems);
				}
				else
					IsLexemCorrenct(">", lineNum, i, currentLexems);
				currentString = "";
			}
			else if (currentLiter == ':')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '=')
				{
					i++;
					IsLexemCorrenct(":=", lineNum, i, currentLexems);
				}
				else
					IsLexemCorrenct(":", lineNum, i, currentLexems);
				currentString = "";
			}
			else
				currentString += currentLiter;
		}
		else
		{
			IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
			currentString = "";
		}
	}
	IsLexemCorrenct(currentString, lineNum, currentLine.length() - 1, currentLexems);
	return currentLexems;
}
