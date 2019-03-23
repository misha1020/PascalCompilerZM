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
void Type();
void ConstMake();
void BlockParsing();
void TypeParsing();
void TypeMake();
void VarParsing();
void VarMake();
void ArrayMake();
void ArrayInBrackets();
void OperatorIfParsing();
void OperatorForParsing();
void OperatorRepeatParsing();
void OperatorWhileParsing();
void OperatorVariableParsing();
void StatementMake();
void StatementParsing();
void ExpressionParsing();
void ExpressionSimple();
void AddendParsing();
void MultiplierParsing();
void Accept(int neededSym, int currentSym, int lineNum, int posNum);