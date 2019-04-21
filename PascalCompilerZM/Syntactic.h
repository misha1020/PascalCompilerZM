#pragma once

#include <fstream>
#include <climits>
#include "Lexical.h"

using namespace std;
using namespace boost;

#define   SCALARS	401
#define   LIMITEDS	402
#define   ENUMS		403
#define   ARRAYS	404

//union variablePart
//{
//	struct typeScalar
//	{
//		string scalarValue;
//	};
//
//	struct typeLimited
//	{
//		union range
//		{
//			struct charLimited
//			{
//				string min;
//				string max;
//			};
//
//			struct intLimited
//			{
//				int min;
//				int max;
//			};
//		};
//	};
//
//	struct typeEnumerated
//	{
//		vector<string> enumElems;
//	};
//
//	struct typeRegular
//	{
//		vector<typeLimited> limiteds;
//		typeScalar scalar;
//	};
//};

//struct typeElement
//{
//	int typeCode;
//	union variablePart varpart;
//};

struct identElement
{
	string useClass;
	int typeCode;
	vector<string> values;
};

extern map<string, identElement> tableDummyIdents;
extern map<string, identElement> tableIdents;
extern int lexNum;
extern int nameNum;
extern vector<int> externalSymbols;
extern vector<int> currentExpression;
extern string currentLocation;
extern bool worksWithArray;

void MakeTableDummyIdents();
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
void StatementSection(vector<int> followers);
void Statement(vector<int> followers);
void StatementUnlabeled(vector<int> followers);
void StatementSimple(vector<int> followers);
void StatementAssignment(vector<int> followers);
void StatementTransition(vector<int> followers);
void Variable(vector<int> followers);
void VariableIndexed(vector<int> followers);
void StatementComplex(vector<int> followers);
void StatementConditional(vector<int> followers);
void StatementCycle(vector<int> followers);
void CycleRepeat(vector<int> followers);
void CycleWhile(vector<int> followers);
void CycleFor(vector<int> followers);
void Const(vector<int> followers);
void String(vector<int> followers);
void Sign(vector<int> followers);
void Number(vector<int> followers);
void Expression(vector<int> followers);
void ComparisonOperation(vector<int> followers);
void ExpressionSimple(vector<int> followers);
void AdaptiveOperation(vector<int> followers);
void Addend(vector<int> followers);
void Multiplier(vector<int> followers);
void ConstWithoutSign(vector<int> followers);
void MultiplicativeOperation(vector<int> followers);


void Accept(int neededSym, int currentSym, int lineNum, int posNum);
vector<int> Union(vector<int> starters, vector<int> followers);
void SkipToBoth(vector<int> starters, vector<int> followers);
void SkipTo(vector<int> symbols);
bool Belong(int symbol, vector<int> symbols);
identElement FindElemInTableIdents(map<string, identElement> tbIdents, string someValue);