#pragma once

#include <fstream>
#include <climits>
#include <stack>

#include "Lexical.h"

using namespace std;
using namespace boost;

extern int lexNum;
extern stack<char> brackets;

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