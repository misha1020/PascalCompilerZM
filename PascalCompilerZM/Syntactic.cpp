#include "pch.h"

#include "Syntactic.h"
#include "starters_and_followers.h"
#include "Union.h"

int lexNum = 0;
vector<int> externalSymbols;

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
		cout << "Ошибка " << neededSym << " в строке " << lineNum << " в символе " << posNum << " добавлена в таблицу ошибок!" << endl;
	}
	else
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

//void CaseCycle()
//{
//	Const();
//	while (allLexems[lexNum].lexem == comma)
//	{
//		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//		Const();
//	}
//	Accept(colon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
//	Statement();
//}

void MultiplicativeOperation(vector<int> followers)
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

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void ConstWithoutSign(vector<int> followers)
{
	switch (allLexems[lexNum].lexem)
	{
	//case ident:
	//	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//	break;
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

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void Multiplier(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, addend_start))
	{
		Accept(511, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(addend_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, addend_start))
	{
		switch (allLexems[lexNum].lexem)
		{
		case ident:
			Variable(followers);
			break;
		case charc:
		case stringc:
		case intc:
		case floatc:
			ConstWithoutSign(followers);
			break;
		case leftpar:
			Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			externalSymbols = Union(vector<int> {rightpar}, followers);
			Expression(externalSymbols);
			Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case notsy:
			Accept(notsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Multiplier(followers);
			break;
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Addend(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, addend_start))
	{
		Accept(511, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(addend_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, addend_start))
	{
		externalSymbols = Union(multiplicativeOperation_start, followers);
 		Multiplier(externalSymbols);
		while (allLexems[lexNum].lexem == divsy || allLexems[lexNum].lexem == modsy || allLexems[lexNum].lexem == andsy
			|| allLexems[lexNum].lexem == star || allLexems[lexNum].lexem == slash)
		{
			MultiplicativeOperation(Union(addend_start, followers));
			externalSymbols = Union(multiplicativeOperation_start, followers);
			Multiplier(externalSymbols);
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void AdaptiveOperation(vector<int> followers)
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

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void ExpressionSimple(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, expression_start))
	{
		Accept(511, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(expression_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, expression_start))
	{
		externalSymbols = Union(addend_start, followers);
		if (allLexems[lexNum].lexem == minus || allLexems[lexNum].lexem == plus)
			Sign(externalSymbols);
		externalSymbols = Union(multiplicativeOperation_start, followers);
		externalSymbols = Union(externalSymbols, adaptiveOperation_start);
		Addend(externalSymbols);
		while (allLexems[lexNum].lexem == plus || allLexems[lexNum].lexem == minus || allLexems[lexNum].lexem == orsy)
		{			
			AdaptiveOperation(Union(addend_start, followers));
			externalSymbols = Union(multiplicativeOperation_start, followers);
			externalSymbols = Union(externalSymbols, adaptiveOperation_start);
			Addend(externalSymbols);
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void ComparisonOperation(vector<int> followers)
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

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void Expression(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, expression_start))
	{
		Accept(511, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(expression_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, expression_start))
	{
		externalSymbols = Union(comparisonOperation_start, followers);
		ExpressionSimple(followers);
		if (allLexems[lexNum].lexem == equal || allLexems[lexNum].lexem == latergreater
			|| allLexems[lexNum].lexem == later || allLexems[lexNum].lexem == greater
			|| allLexems[lexNum].lexem == laterequal || allLexems[lexNum].lexem == greaterequal)
		{
			externalSymbols = Union(expression_start, followers);
			ComparisonOperation(externalSymbols);
			ExpressionSimple(followers);
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Number(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, number_start))
	{
		Accept(502, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(number_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, number_start))
	{
		switch (allLexems[lexNum].lexem)
		{
		case intc:
			Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case floatc:
			Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Sign(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, sign_start))
	{
		Accept(503, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(sign_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, sign_start))
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

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void String(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, string_start))
	{
		Accept(501, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(string_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, string_start))
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

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Const(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, const_start))
	{
		Accept(50, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(const_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, const_start))
	{
		switch (allLexems[lexNum].lexem)
		{
		case stringc:
		case charc:
			String(followers);
				break;
		case intc:
		case floatc:
			Number(followers);
				break;
		case minus:
		case plus:
		{
			externalSymbols = Union(vector<int> { ident, intc, floatc}, followers);
			Sign(externalSymbols);
			if (allLexems[lexNum].lexem == ident)
				Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			else
				Number(followers);
		}
			break;
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void CycleFor()
{
	Accept(forsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Expression();
	Accept(tosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Expression();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Statement();
}

void CycleWhile()
{
	Accept(whilesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Expression();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Statement();
}

void CycleRepeat()
{
	Accept(repeatsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Statement();
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		//Statement();
	}
	Accept(untilsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Expression();
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
	//Expression();
	Accept(thensy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//Statement();
	if (allLexems[lexNum].lexem == elsesy)
	{
		Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		//Statement();
	}
}

void StatementComplex()
{
	switch (allLexems[lexNum].lexem)
	{
	case beginsy:
		//StatementSection();
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

void VariableIndexed(vector<int> followers)
{
	Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(vector<int> {comma, rbracket}, followers);
	Expression(externalSymbols);
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		externalSymbols = Union(vector<int> {comma, rbracket}, followers);
		Expression(externalSymbols);
	}
	Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void Variable(vector<int> followers)
{	
	if (!Belong(allLexems[lexNum].lexem, vector<int> {ident})) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		Accept(876, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(vector<int> {ident}, followers);
	}
	if (allLexems[lexNum].lexem == ident) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		if (allLexems[lexNum].lexem == lbracket)
			VariableIndexed(followers);

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void StatementTransition()
{
	Accept(gotosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void StatementAssignment(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		Accept(876, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementAssignment_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		externalSymbols = Union(vector<int> {assign}, followers);
		Variable(externalSymbols);
		Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Expression(followers);

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void StatementSimple(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		Accept(876, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementAssignment_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		switch (allLexems[lexNum].lexem)
		{
		case ident:
			externalSymbols = Union(expression_start, followers);
			StatementAssignment(externalSymbols);
			break;
		case gotosy:
			StatementTransition();
			break;
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void StatementUnlabeled(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		Accept(876, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementAssignment_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
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
			StatementSimple(followers);
			break;
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Statement(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		Accept(876, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementAssignment_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementAssignment_start)) // statementAssignment_startstatementAssignment_startstatementAssignment_start
	{
		switch (allLexems[lexNum].lexem)
		{
		case intc:
		{
			Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			StatementUnlabeled(followers);  // ?????
		}
		break;
		case beginsy:
		case ifsy:
		case whilesy:
		case repeatsy:
		case forsy:
		case ident:
		case gotosy:
			StatementUnlabeled(followers);  // ????/
			break;
		}

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
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

void StatementSection(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementSection_start))
	{
		Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementSection_start, followers);
	}
	if (allLexems[lexNum].lexem == beginsy)
	{
		Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		externalSymbols = Union(vector<int> { endsy, semicolon}, followers);
		Statement(externalSymbols);
		while (allLexems[lexNum].lexem == semicolon)
		{
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			externalSymbols = Union(vector<int> { endsy, semicolon}, followers);
			Statement(externalSymbols);
		}
		Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void VarDefinition(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, varDef_start))
	{
		Accept(2, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(varDef_start, followers);
	}
	if (allLexems[lexNum].lexem == ident)
	{
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		Accept(colon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Type(followers);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void VarSection(vector<int> followers)
{
	vector<int> varStartFollow = Union(varSection_start, varSection_follow);
	if (!Belong(allLexems[lexNum].lexem, varStartFollow))
	{
		Accept(18, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(varStartFollow, followers);
	}
	if (allLexems[lexNum].lexem == varsy)
	{
		Accept(varsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		do {
			externalSymbols = Union(vector<int> { semicolon }, followers);
			VarDefinition(externalSymbols);
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		} while (allLexems[lexNum].lexem == ident);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void TypeEnumerated(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, typeEnumerated_start))
	{
		Accept(10, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(typeEnumerated_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, typeEnumerated_start))
	{
		Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void TypeLimited(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, typeLimited_start))
	{
		Accept(10, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(typeLimited_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, typeLimited_start))
	{
		externalSymbols = Union(vector<int> { twopoints}, followers);
		Const(externalSymbols);
		Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Const(followers);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void TypeSimple(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, typeSimple_start))
	{
		Accept(10, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(typeSimple_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, typeSimple_start))
	{
		switch (allLexems[lexNum].lexem)
		{
		case ident:
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case leftpar:
			externalSymbols = Union(vector<int> { rightpar }, followers);
			TypeEnumerated(externalSymbols);
			break;
		case stringc:
		case charc:
		case minus:
		case plus:
		case floatc:
		case intc:
			externalSymbols = Union(vector<int> { twopoints }, followers);
			TypeLimited(externalSymbols);
			break;
		}
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void TypeRegular(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, typeRegular_start))
	{
		Accept(10, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(typeRegular_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, typeRegular_start))
	{
		Accept(arraysy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		TypeSimple(followers);
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			TypeSimple(followers);
		}
		Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Type(followers);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Type(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, type_start))
	{
		Accept(10, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(type_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, type_start))
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
			TypeSimple(followers);
			break;
		case arraysy:
		{
			externalSymbols = Union(vector<int> { comma, lbracket, rbracket, ofsy }, followers);
			TypeRegular(externalSymbols);
		}
			break;
		}
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void TypeDefinition(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, typeDef_start))
	{
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(typeDef_start, followers);
	}
	if (allLexems[lexNum].lexem == ident)
	{
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Type(followers);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void TypeSection(vector<int> followers)
{
	vector<int> typeStartFollow = Union(typeSection_start, typeSection_follow);
	if (!Belong(allLexems[lexNum].lexem, typeStartFollow))
	{
		Accept(10, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(typeStartFollow, followers);
	}
	if (allLexems[lexNum].lexem == typesy)
	{
		Accept(typesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		do {
			externalSymbols = Union(vector<int> { semicolon }, followers);
			TypeDefinition(externalSymbols);
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		} while (allLexems[lexNum].lexem == ident);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void Block(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, block_start))
	{
		Accept(18, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(block_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, block_start))
	{
		externalSymbols = Union(typeSection_follow, followers);
		TypeSection(externalSymbols);
		externalSymbols = Union(varSection_follow, followers);
		VarSection(externalSymbols);
		StatementSection(followers);
		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
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
		Block(block_follow);
		Accept(point, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
}