#pragma once

#include <fstream>
#include <climits>

#include "Lexical.h"

using namespace std;
using namespace boost;

extern int lexNum;
extern vector<int> externalSymbols;

void Program();
void Block(vector<int> followers);
void TypeSection(vector<int> followers);
void TypeDefinition(vector<int> followers);
void Type(vector<int> followers);
void TypeRegular(vector<int> followers);
void TypeSimple(vector<int> followers);
void TypeLimited(vector<int> followers);
void TypeEnumerated(vector<int> followers);
void VarSection(vector<int> followers);
void VarDefinition(vector<int> followers);
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
void Const(vector<int> followers);
void String(vector<int> followers);
void Sign(vector<int> followers);
void Number(vector<int> followers);
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