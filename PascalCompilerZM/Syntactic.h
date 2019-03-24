#pragma once

#include <fstream>
#include <climits>

#include "Lexical.h"

using namespace std;
using namespace boost;

extern int lexNum;

void Parsing();
void BlockParsing();
void TypeParsing(); 
void TypeMake();
void TypeSimple();
void ConstMake();
void VarParsing(); 
void VarMake();
void StatementParsing();
void StatementMake();
void OperatorVariableParsing();
void ExpressionParsing();
void ExpressionSimple();
void AddendParsing();
void MultiplierParsing();
void CaseCycle();
void OperatorIfParsing();
void OperatorWhileParsing();
void OperatorRepeatParsing();
void OperatorForParsing();
void Accept(int neededSym, int currentSym, int lineNum, int posNum);
vector<int> Union(vector<int> starters, vector<int> followers);
void SkipToBoth(vector<int> starters, vector<int> followers);
void SkipTo(vector<int> symbols);
bool Belong(int symbol, vector<int> symbols);