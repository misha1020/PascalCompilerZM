#include "pch.h"

#include "Syntactic.h"
#include "starters_and_followers.h"


int lexNum = 0;

void Accept(int neededSym, int currentSym, int lineNum, int posNum)
{
	if (currentSym != neededSym)
	{
		if (lexNum >= lexemsCount)
		{
			lineNum = currLineNum - 1;
			posNum = 0;
		}
		AddErrorToTable(lineNum, posNum, neededSym);
		cout << "������ " << neededSym << " � ������ " << lineNum << " � ������� " << posNum << " ��������� � ������� ������!" << endl;
	}
	lexNum++;
}

bool Belong(int symbol, vector<int> symbols)
{
	if (find(symbols.begin(), symbols.end(), symbol) != symbols.end())
		return true;
	else
		return false;
}

void SkipTo(vector<int> symbols)
{
	while (!Belong(allLexems[lexNum].lexem, symbols))
		lexNum++;
}

void SkipToBoth(vector<int> starters, vector<int> followers)
{
	while (!(Belong(allLexems[lexNum].lexem, starters) || Belong(allLexems[lexNum].lexem, followers)))
		lexNum++;
}

vector<int> Union(vector<int> starters, vector<int> followers)
{
	vector<int> both;
	both.insert(both.end(), starters.begin(), starters.end());
	both.insert(both.end(), followers.begin(), followers.end());
	return both;
}

void CaseCycle()
{
	Const();
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Const();
	}
	Accept(colon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement();
}

void MultiplicativeOperation()
{
	switch (allLexems[lexNum].lexem)
	{
	case divsy:
		Accept(divsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case modsy:
		Accept(modsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case andsy:
		Accept(andsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case star:
		Accept(star, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case slash:
		Accept(slash, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void ConstWithoutSign()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case stringc:
		Accept(stringc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case charc:
		Accept(charc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case intc:
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case floatc:
		Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void Multiplier()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Variable();
		break;
	case charc:
	case stringc:
	case intc:
	case floatc:
		ConstWithoutSign();
		break;
	case leftpar:
		Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Expression();
		Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case notsy:
		Accept(notsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Multiplier();
		break;
	}
}

void Addend()
{
	Multiplier();
	while (allLexems[lexNum].lexem == divsy || allLexems[lexNum].lexem == modsy || allLexems[lexNum].lexem == andsy
		|| allLexems[lexNum].lexem == star || allLexems[lexNum].lexem == slash)
	{
		MultiplicativeOperation();
		Multiplier();
	}
}

void AdaptiveOperation()
{
	switch (allLexems[lexNum].lexem)
	{
	case plus:
		Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case minus:
		Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case orsy:
		Accept(orsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void ExpressionSimple()
{
	Sign();
	Addend();
	while (allLexems[lexNum].lexem == plus || allLexems[lexNum].lexem == minus || allLexems[lexNum].lexem == orsy)
	{
		AdaptiveOperation();
		Addend();
	}
}

void ComparisonOperation()
{
	switch (allLexems[lexNum].lexem)
	{
	case equal:
		Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case latergreater:
		Accept(latergreater, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case later:
		Accept(later, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case greater:
		Accept(greater, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case laterequal:
		Accept(laterequal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case greaterequal:
		Accept(greaterequal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void Expression()
{
	ExpressionSimple();
	if (allLexems[lexNum].lexem == equal || allLexems[lexNum].lexem == latergreater
		|| allLexems[lexNum].lexem == later || allLexems[lexNum].lexem == greater
		|| allLexems[lexNum].lexem == laterequal || allLexems[lexNum].lexem == greaterequal)
	{
		ComparisonOperation();
		ExpressionSimple();
	}
}

void Number()
{
	switch (allLexems[lexNum].lexem)
	{
	case intc:
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case floatc:
		Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	default:
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void Sign()
{
	switch (allLexems[lexNum].lexem)
	{
	case plus:
		Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case minus:
		Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void String()
{
	switch (allLexems[lexNum].lexem)
	{
	case stringc:
		Accept(stringc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case charc:
		Accept(charc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void Const()
{
	switch (allLexems[lexNum].lexem)
	{
	case stringc:
	case charc:
		String();
		break;
	case intc:
	case floatc:
		Number();
		break;
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case minus:
	case plus:
	{
		Sign();
		if (allLexems[lexNum].lexem == ident)
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		else
			Number();
	}
	break;
	}
}

void CycleFor()
{
	Accept(forsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
	Accept(tosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement();
}

void CycleWhile()
{
	Accept(whilesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement();
}

void CycleRepeat()
{
	Accept(repeatsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement();
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Statement();
	}
	Accept(untilsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
}

void StatementCycle()
{
	switch (allLexems[lexNum].lexem)
	{
	case repeatsy:
		CycleRepeat();
		break;
	case whilesy:
		CycleWhile();
		break;
	case forsy:
		CycleFor();
		break;
	}
}

void StatementConditional()
{
	Accept(ifsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
	Accept(thensy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement();
	if (allLexems[lexNum].lexem == elsesy)
	{
		Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Statement();
	}
}

void StatementComplex()
{
	switch (allLexems[lexNum].lexem)
	{
	case beginsy:
		StatementSection();
		break;
	case ifsy:
		StatementConditional();
		break;
	case repeatsy:
	case whilesy:
	case forsy:
		StatementCycle();
		break;
	}
}

void VariableIndexed()
{
	Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Expression();
	}
	Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void Variable()
{	
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	if (allLexems[lexNum].lexem == lbracket)
		VariableIndexed();
}

void StatementTransition()
{
	Accept(gotosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void StatementAssignment()
{
	Variable();
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression();
}

void StatementSimple()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		StatementAssignment();
		break;
	case gotosy:
		StatementTransition();
		break;
	}
}

void StatementUnlabeled()
{
	switch (allLexems[lexNum].lexem)
	{
	case beginsy:
	case ifsy:
	case whilesy:
	case repeatsy:
	case forsy:
		StatementComplex();
		break;
	case ident:
	case gotosy:
		StatementSimple();
		break;
	}
}

void Statement()
{
	switch (allLexems[lexNum].lexem)
	{
	case intc:
	{
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementUnlabeled();
	}
		break;
	case beginsy:
	case ifsy:
	case whilesy:
	case repeatsy:
	case forsy:
	case ident:
	case gotosy:
		StatementUnlabeled();
		break;
	}
}

//case casesy:
//{
//	Accept(casesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//	Expression();
//	Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//	CaseCycle();
//	while (allLexems[lexNum].lexem == semicolon)
//	{
//		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//		if (allLexems[lexNum].lexem != elsesy)
//			CaseCycle();
//		else
//		{
//			Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//			Statement();
//			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//		}
//	}
//	Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//}

void StatementSection()
{
	Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement();
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Statement();
	}
	Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void VarDefinition()
{
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
	Accept(colon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Type();

}

void VarSection()
{
	if (allLexems[lexNum].lexem == varsy)
	{
		Accept(varsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		VarDefinition();
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		while (allLexems[lexNum].lexem == ident)
		{
			VarDefinition();
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
	}
}

void TypeEnumerated()
{
	Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
	Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void TypeLimited()
{
	Const();
	Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Const();
}

void TypeSimple()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case leftpar:
		TypeEnumerated();
		break;
	case stringc:
	case charc:
	case minus:
	case plus:
	case floatc:
	case intc:
		TypeLimited();
		break;
	}
}

void TypeRegular()
{
	Accept(arraysy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	TypeSimple();
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		TypeSimple();
	}
	Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Type();
}

void Type()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
	case leftpar:
	case stringc:
	case charc:
	case minus:
	case plus:
	case intc:
	case floatc:
		TypeSimple();
		break;
	case arraysy:
		TypeRegular();
		break;
	default:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void TypeDefinition()
{
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Type();
}

void TypeSection()
{
	if (allLexems[lexNum].lexem == typesy)
	{
		Accept(typesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		TypeDefinition();
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		while (allLexems[lexNum].lexem == ident)
		{
			TypeDefinition();
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
	}
}

void Block()
{
	//vector<int> ptra;
	//if (!Belong(allLexems[lexNum].lexem, block_start))
	//{
	//	Accept(18, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//	SkipToBoth(block_start, followers);
	//}
	//if (Belong(allLexems[lexNum].lexem, block_start))
	//{

	//	ptra = Union(var_start, followers);
	TypeSection();
	
	//	ptra = Union(var_start, followers);
	VarSection();

	StatementSection();

	//ptra = Union(statement_start, followers);
	//if (!Belong(allLexems[lexNum].lexem, followers))
	//{
	//	Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//	SkipTo(followers);
	//}

	//}
}

void Program()
{	
	if (lexemsCount > 0)
	{
		if (allLexems[lexNum].lexem == programsy)
		{
			Accept(programsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		Block();
		Accept(point, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
}