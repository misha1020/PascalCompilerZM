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
{"+", plus        },
{"-", minus       },
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
	regex rx("[0-9]+[.]{1}[0-9]+([eE][-][0-9]+)?");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsString(string s)
{
	regex rx("\".*\"");
	return regex_match(s.begin(), s.end(), rx);
}

bool IsChar(string s)
{
	regex rx("\'.+\'");
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
		posNum = posNum - lexem.length() + 1;
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
			{
				int currentInt = lexical_cast<int>(lexem);
				if (currentInt > 32767 || currentInt < -32766)
					AddErrorToTable(lineNum, posNum, 203);
				else
					currentLexems.push_back(intc);
			}
			else if (IsReal(lexem))
				currentLexems.push_back(floatc);
			//206:слишком маленькая вещественная константа
			//207 : слишком большая вещественная константа
			else if (IsString(lexem))
				currentLexems.push_back(stringc);
			else if (IsChar(lexem))
			{
				string currentChar = lexical_cast<string>(lexem);
				if (currentChar.length() > 3)
					AddErrorToTable(lineNum, posNum, 75);
				else
					currentLexems.push_back(charc);
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
				if (i > 0 && currentString[currentString.length()] == 'e')
				{
					currentString += "-";
				}
				else
				{
					IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
					IsLexemCorrenct("-", lineNum, i, currentLexems);
					//currentLexems.push_back(point);
					currentString = "";
				}
			}
			else if (currentLiter == '*')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == ')')
				{
					isComment = false;
					i++;
					IsLexemCorrenct("*)", lineNum, i, currentLexems);
					//currentLexems.push_back(rcomment);
				}
				else if (!isComment)
					IsLexemCorrenct("*", lineNum, i, currentLexems);
				//currentLexems.push_back(star);
				currentString = "";
			}
			else if (currentLiter == '(')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '*')
				{
					isComment = true;
					i++;
					IsLexemCorrenct("(*", lineNum, i, currentLexems);
					//currentLexems.push_back(lcomment);
				}
				else
					IsLexemCorrenct("(", lineNum, i, currentLexems);
				//currentLexems.push_back(leftpar);
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
						//currentLexems.push_back(twopoints);
						currentString = "";
					}
					else
						currentString += ".";
				}
				else
				{
					IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
					IsLexemCorrenct(".", lineNum, i, currentLexems);
					//currentLexems.push_back(point);
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
						//currentLexems.push_back(latergreater);
					}
					else if (currentLiter == '=')
					{
						i++;
						IsLexemCorrenct("<=", lineNum, i, currentLexems);
						//currentLexems.push_back(laterequal);
					}
					else
						IsLexemCorrenct("<", lineNum, i, currentLexems);
						//currentLexems.push_back(later);
				}
				else
					IsLexemCorrenct("<", lineNum, i, currentLexems);
					//currentLexems.push_back(later);
				currentString = "";
			}
			else if (currentLiter == '>')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '=')
				{
					i++;
					IsLexemCorrenct(">=", lineNum, i, currentLexems);
					//currentLexems.push_back(greaterequal);
				}
				else
					IsLexemCorrenct(">", lineNum, i, currentLexems);
					//currentLexems.push_back(greater);
				currentString = "";
			}
			else if (currentLiter == ':')
			{
				IsLexemCorrenct(currentString, lineNum, i - 1, currentLexems);
				if (i < currentLine.length() && currentLine[i + 1] == '=')
				{
					i++;
					IsLexemCorrenct(":=", lineNum, i, currentLexems);
					//currentLexems.push_back(assign);
				}
				else
					IsLexemCorrenct(":", lineNum, i, currentLexems);
					//currentLexems.push_back(colon);
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
