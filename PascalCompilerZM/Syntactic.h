#pragma once

#include <fstream>
#include <climits>

#include "Lexical.h"

using namespace std;
using namespace boost;

extern int lexNum;

void Program();
void Block();
void TypeSection();
void TypeDefinition();
void Type();
void TypeRegular();
void TypeSimple();
void TypeLimited();
void TypeEnumerated();
void VarSection();
void VarDefinition();
void StatementSection();
void Statement();
void StatementUnlabeled();
void StatementSimple();
void StatementAssignment();
void StatementTransition();
void Variable();
void VariableIndexed();
void StatementComplex();
void StatementConditional();
void StatementCycle();
void CycleRepeat();
void CycleWhile();
void CycleFor();
void Const();
void String();
void Sign();
void Number();
void Expression();
void ComparisonOperation();
void ExpressionSimple();
void AdaptiveOperation();
void Addend();
void Multiplier();
void ConstWithoutSign();
void MultiplicativeOperation();
void CaseCycle();




void Accept(int neededSym, int currentSym, int lineNum, int posNum);
vector<int> Union(vector<int> starters, vector<int> followers);
void SkipToBoth(vector<int> starters, vector<int> followers);
void SkipTo(vector<int> symbols);
bool Belong(int symbol, vector<int> symbols);