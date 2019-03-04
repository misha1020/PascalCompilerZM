#include "pch.h"

#include "Lexical.h"

textPosition errPositions[MAX_ERR_COUNT];
int currErrorsCount = 0;
bool isComment = false;


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
{"+'", plus        },
{"-'", minus       },
{"(*", lcomment    },
{"*)", rcomment    },
{":=", assign      },
{"..", twopoints   },
{"ident", ident    },
{"float", floatc   },
{"int", intc       },
{"char", charc     },
{"string", stringc },
{"eof", endoffile  },
{"eolint", eolint  },
{"false", FALSE    },
{"true", TRUE      }
};

bool IsName(string s)
{
	regex rx("[a-z][a-z0-9]*");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsInt(string s)
{
	regex rx("[+-]?[0-9]*");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsFloat(string s)
{
	regex rx("[+-]?[0-9]+\\.[0-9]*");
	return regex_match(s.begin(), s.end(), rx);
}

void AddErrorToTable(int lineNum, int charNum, int errNum)
{
	errPositions[currErrorsCount].lineNumber = lineNum;
	errPositions[currErrorsCount].charNumber = charNum;
	errPositions[currErrorsCount].errNumber = errNum;
	currErrorsCount++;
}

void IsLexemCorrenct(string lexem, int lineNum, int posNum, vector<int>& currentLexems)
{
	if (lexem != "" && !isComment)
	{
		posNum -= lexem.length();
		if (keyWords.find(lexem) != keyWords.end())
		{
			int definer = keyWords.find(lexem)->second;
			currentLexems.push_back(definer);
		}
		else
		{
			if (IsName(lexem))
				currentLexems.push_back(ident);
			else if (IsInt(lexem))
				currentLexems.push_back(intc);
			else if (IsFloat(lexem))
				currentLexems.push_back(floatc);
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
				IsLexemCorrenct(currentString, lineNum, i, currentLexems);
				IsLexemCorrenct(string(1, currentLiter), lineNum, i, currentLexems);			// ??
				currentString = "";
			}
			else if (currentLiter == '-' && !isComment)
			{
				if (i < currentLine.length() && IsInt(string(1, currentLine[i + 1])))
				{
					currentString += "-";
				}
				else
				{
					IsLexemCorrenct(currentString, lineNum, i, currentLexems);
					IsLexemCorrenct(string(1, currentLiter), lineNum, i, currentLexems);
					currentString = "";
				}
			}
			else if (currentLiter == '*')
			{
				IsLexemCorrenct(currentString, lineNum, i, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == ')')
				{
					isComment = false;
					i++;
					currentLexems.push_back(rcomment);
				}
				else if (!isComment)
					currentLexems.push_back(star);
				currentString = "";
			}
			else if (currentLiter == '(' && !isComment)
			{
				IsLexemCorrenct(currentString, lineNum, i, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '*')
				{
					isComment = true;
					i++;
					currentLexems.push_back(lcomment);
				}
				else
					currentLexems.push_back(leftpar);
				currentString = "";
			}
			else if (currentLiter == '.' && !isComment)
			{
				if (i > 0 && IsInt(string(1, currentLine[i - 1])))
				{
					currentString += ".";
				}
				else if (i < currentLine.length() && currentLine[i + 1] == '.')
				{
					IsLexemCorrenct(currentString, lineNum, i, currentLexems);
					i++;
					currentLexems.push_back(twopoints);
					currentString = "";
				}
				else
				{
					IsLexemCorrenct(currentString, lineNum, i, currentLexems);
					currentLexems.push_back(point);
					currentString = "";
				}
			}
			else if (currentLiter == '<' && !isComment)
			{
				IsLexemCorrenct(currentString, lineNum, i, currentLexems);
				if (i < currentLine.length())
				{
					currentLiter = currentLine[i + 1];
					if (currentLiter == '>')
					{
						i++;
						currentLexems.push_back(latergreater);
					}
					else if (currentLiter == '=')
					{
						i++;
						currentLexems.push_back(laterequal);
					}
					else
						currentLexems.push_back(later);
				}
				else
					currentLexems.push_back(later);
				currentString = "";
			}
			else if (currentLiter == '>' && !isComment)
			{
				IsLexemCorrenct(currentString, lineNum, i, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '=')
				{
					i++;
					currentLexems.push_back(greaterequal);
				}
				else
					currentLexems.push_back(greater);
				currentString = "";
			}
			else if (currentLiter == ':' && !isComment)
			{
				IsLexemCorrenct(currentString, lineNum, i, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '=')
				{
					i++;
					currentLexems.push_back(assign);
				}
				else
					currentLexems.push_back(colon);
				currentString = "";
			}
			else
				currentString += currentLiter;
		}
		else
		{
			IsLexemCorrenct(currentString, lineNum, i, currentLexems);
			currentString = "";
		}
	}
	IsLexemCorrenct(currentString, lineNum, currentLine.length(), currentLexems);
	return currentLexems;
}
