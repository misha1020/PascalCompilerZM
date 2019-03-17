#pragma once

#include <fstream>
#include <climits>

#include "Lexical.h"

using namespace std;
using namespace boost;

extern int lexNum;

void Parsing();
void Type();
void ProgramParsing();
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
void StatementParsing();
void StatementSwitch();
void ExpressiongParsing();
void ExpressionInOutBrackets();
void ExpressionPlusMinus();
void ExpressionOperations();
void Accept(int neededSym, int currentSym, int lineNum, int posNum);

