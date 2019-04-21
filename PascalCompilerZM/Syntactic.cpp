#include "pch.h"

#include "Syntactic.h"
#include "starters_and_followers.h"
#include "Union.h"

int lexNum = 0, nameNum = 0, valueNum = 0;
vector<int> externalSymbols, currentExpression;
map<string, identElement> tableDummyIdents, tableIdents;
string currentLocation;
bool worksWithArray = false;
string isBoolType = "not";

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

identElement FindElemInTableIdents(map<string, identElement> tbIdents, string someValue)
{
	auto it = tbIdents.begin();
	while (it != tbIdents.end())
	{
		if (it->first == someValue)
			return it->second;
		it++;
	}
	identElement null{"null"};
	return null;
}

string exprType(vector<int> expr)
{
	if (expr.size() == 0)
		return "empty";
	if (expr.size() == 1)
	{
		if (expr[0] == ident)
		{
			nameNum++;
			identElement currentElem = FindElemInTableIdents(tableIdents, tableNames[nameNum]);
			if (currentElem.useClass == "var")
				return currentElem.values[0];
			else if (currentElem.useClass == "enumer")
				return "enumer";
			else if (currentElem.useClass == "type")
				return "type";
			else
				return "undefined";
		}
		if (expr[0] == intc)
			return "integer";
		if (expr[0] == floatc)
			return "real";
		if (expr[0] == charc || expr[0] == stringc)
			return "char";
		if (expr[0] == TRUE || expr[0] == FALSE)
			return "boolean";
	}
	else if (isBoolType == "boolean")
	{
		for (int i = 0; i < expr.size(); i++)
		{
			if (expr[i] == ident)
				nameNum++;
		}
	}
	else
	{
		vector<int> leftPart, rightPart;
		bool isLeftPart = true;
		for (int i = 0; i < expr.size(); i++)
		{
			if (isLeftPart)
			{
				if (expr[i] == plus || expr[i] == minus || expr[i] == slash || expr[i] == star)
					isLeftPart = false;
				else
					leftPart.push_back(expr[i]);
			}
			else
				rightPart.push_back(expr[i]);
		}
		string lPartResult = exprType(leftPart);
		string rPartResult = exprType(rightPart);

		if (lPartResult == "emty" || rPartResult == "empty")
			return "empty";
		if (lPartResult == rPartResult)
			return rPartResult;
		else
		{
			if (lPartResult == "integer"  && rPartResult == "real")
				return "real";
			if (rPartResult == "integer"  && lPartResult == "real")
				return "real";
		}
	}
	return "kek";
}

void MultiplicativeOperation(vector<int> followers)
{
	switch (allLexems[lexNum].lexem)
	{
	case divsy:
	{
		Accept(divsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(divsy);
	}
		break;
	case modsy:
	{
		Accept(modsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(modsy);
	}
		break;
	case andsy:
	{
		Accept(andsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(andsy);
	}
		break;
	case star:
	{
		Accept(star, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(star);
	}
		break;
	case slash:
	{
		Accept(slash, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(slash);
	}
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
	case stringc:
	{
		Accept(stringc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(stringc);
	}
		break;
	case charc:
	{
		Accept(charc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(charc);
	}
		break;
	case intc:
	{
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(intc);
	}
		break;
	case floatc:
	{
		Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(floatc);
	}
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
		{
			Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(leftpar);
			externalSymbols = Union(vector<int> {rightpar}, followers);
			Expression(externalSymbols);
			Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(rightpar);
		}
			break;
		case notsy:
		{
			Accept(notsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(notsy);
			Multiplier(followers);
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
	{
		Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(plus);
	}
		break;
	case minus:
	{
		Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(minus);
	}
		break;
	case orsy:
	{
		Accept(orsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(orsy);
	}
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
	{
		Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(equal);
	}
		break;
	case latergreater:
	{
		Accept(latergreater, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(latergreater);
	}
		break;
	case later:
	{
		Accept(later, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(later);
	}
		break;
	case greater:
	{
		Accept(greater, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(greater);
	}
		break;
	case laterequal:
	{
		Accept(laterequal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(laterequal);
	}
		break;
	case greaterequal:
	{
		Accept(greaterequal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(greaterequal);
	}
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
		currentExpression.clear();
		isBoolType = "not";
		ExpressionSimple(externalSymbols);
		if (allLexems[lexNum].lexem == equal || allLexems[lexNum].lexem == latergreater
			|| allLexems[lexNum].lexem == later || allLexems[lexNum].lexem == greater
			|| allLexems[lexNum].lexem == laterequal || allLexems[lexNum].lexem == greaterequal)
		{
			isBoolType = "boolean";
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
		{
			Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(intc);
		}
			break;
		case floatc:
		{
			Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(floatc);
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
		{
			Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(plus);
		}
			break;
		case minus:
		{
			Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(minus);
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
		{
			Accept(stringc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(stringc);
		}
			break;
		case charc:
		{
			Accept(charc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			currentExpression.push_back(charc);
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
			{
				Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				currentExpression.push_back(ident);
			}
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

void CycleFor(vector<int> followers)
{
	Accept(forsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(expression_start, statement_start);
	externalSymbols = Union(externalSymbols, vector<int> {tosy, dosy });
	externalSymbols = Union(externalSymbols, followers);
	Expression(externalSymbols);
	Accept(tosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(statement_start, vector<int> { dosy });
	externalSymbols = Union(externalSymbols, followers);
	Expression(externalSymbols);
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement(followers);
}

void CycleWhile(vector<int> followers)
{
	Accept(whilesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(vector<int> { dosy}, statement_start);
	externalSymbols = Union(externalSymbols, followers);
	Expression(externalSymbols);
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Statement(followers);
}

void CycleRepeat(vector<int> followers)
{
	Accept(repeatsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(vector<int> { semicolon, untilsy }, statement_start);
	externalSymbols = Union(expression_start, externalSymbols);
	externalSymbols = Union(externalSymbols, followers);
	Statement(externalSymbols);
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		externalSymbols = Union(vector<int> { semicolon, untilsy }, statement_start);
		externalSymbols = Union(expression_start, externalSymbols);
		externalSymbols = Union(externalSymbols, followers);
		Statement(externalSymbols);
	}
	Accept(untilsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Expression(followers);
}

void StatementCycle(vector<int> followers)
{
	switch (allLexems[lexNum].lexem)
	{
	case repeatsy:
		CycleRepeat(followers);
		break;
	case whilesy:
		CycleWhile(followers);
		break;
	case forsy:
		CycleFor(followers);
		break;
	}

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void StatementConditional(vector<int> followers)
{
	Accept(ifsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(vector<int> { thensy, elsesy }, statement_start);
	externalSymbols = Union(externalSymbols, followers);
	Expression(externalSymbols);
	Accept(thensy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	externalSymbols = Union(vector<int> { elsesy }, statement_start);
	externalSymbols = Union(externalSymbols, followers);
	Statement(externalSymbols);
	if (allLexems[lexNum].lexem == elsesy)
	{
		Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		externalSymbols = Union(externalSymbols, followers);
		Statement(externalSymbols);
	}

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
	}
}

void StatementComplex(vector<int> followers)
{
	switch (allLexems[lexNum].lexem)
	{
	case beginsy:
		StatementSection(followers);
		break;
	case ifsy:
		StatementConditional(followers);
		break;
	case repeatsy:
	case whilesy:
	case forsy:
		StatementCycle(followers);
		break;
	}

	if (!Belong(allLexems[lexNum].lexem, followers))
	{
		Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipTo(followers);
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
	if (!Belong(allLexems[lexNum].lexem, variable_start))
	{
		Accept(876, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(variable_start, followers);
	}
	if (allLexems[lexNum].lexem == ident)
	{
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		currentExpression.push_back(ident);
		if (allLexems[lexNum].lexem == lbracket)
			VariableIndexed(followers);

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void StatementTransition(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementTransition_start))
	{
		Accept(gotosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementTransition_start, followers);
	}
	if (allLexems[lexNum].lexem == gotosy)
	{
		Accept(gotosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void StatementAssignment(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementAssignment_start))
	{
		Accept(997, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementAssignment_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementAssignment_start))
	{
		externalSymbols = Union(vector<int> {assign}, followers);
		Variable(externalSymbols);
		Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Expression(followers);

		identElement currentElem = FindElemInTableIdents(tableIdents, tableNames[nameNum]);
		string currentType = exprType(currentExpression);
		if (currentElem.useClass == "type" || currentElem.useClass == "prog" || currentElem.useClass == "enumer")
			AddErrorToTable(allLexems[lexNum - 3].lineNumber, allLexems[lexNum - 3].charNumber, 100);
		else if (currentElem.useClass != "var" && currentElem.useClass != "undefined")
		{
			AddErrorToTable(allLexems[lexNum - 3].lineNumber, allLexems[lexNum - 3].charNumber, 104);
			if (nameNum < tableNames.size())
			{
				identElement tElement{ "undefined" };
				tableIdents.insert(pair<string, identElement>(tableNames[nameNum], tElement));
			}
		}
		else if (currentElem.values.size() != 0 && isBoolType != currentElem.values[0] &&
			currentElem.values[0] != currentType &&	(currentElem.typeCode != ENUMS || currentType != "enumer") )
		{
			AddErrorToTable(allLexems[lexNum - 1].lineNumber, allLexems[lexNum - 1].charNumber, 328);
		}
		nameNum++;

		if (!Belong(allLexems[lexNum].lexem, followers))
		{
			Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			SkipTo(followers);
		}
	}
}

void StatementSimple(vector<int> followers)
{
	if (!Belong(allLexems[lexNum].lexem, statementSimple_start))
	{
		Accept(322, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementSimple_start, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementSimple_start))
	{
		switch (allLexems[lexNum].lexem)
		{
		case ident:
		{
			externalSymbols = Union(expression_start, followers);
			StatementAssignment(externalSymbols);
		}
			break;
		case gotosy:
			StatementTransition(followers);
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
	if (!Belong(allLexems[lexNum].lexem, statementUnlabeled))
	{
		Accept(322, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementUnlabeled, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statementUnlabeled))
	{
		switch (allLexems[lexNum].lexem)
		{
		case beginsy:
		case ifsy:
		case whilesy:
		case repeatsy:
		case forsy:
			StatementComplex(followers);
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
	vector<int> statementStartFollow = Union(statement_start, statement_follow);
	if (!Belong(allLexems[lexNum].lexem, statementStartFollow))
	{
		Accept(322, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		SkipToBoth(statementStartFollow, followers);
	}
	if (Belong(allLexems[lexNum].lexem, statement_start))
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
		externalSymbols = Union(statement_follow, followers);
		Statement(externalSymbols);
		while (allLexems[lexNum].lexem == semicolon)
		{
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			externalSymbols = Union(statement_follow, followers);
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
		int i = 1;
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			i++;
		}

		if (!worksWithArray)
		{
			bool isOk = true;
			if (FindElemInTableIdents(tableIdents, tableNames[nameNum]).useClass != "null")
				AddErrorToTable(allLexems[lexNum - 4 - (i - 1) * 2].lineNumber, allLexems[lexNum - 4 - (i - 1) * 2].charNumber, 101);
			else
			{
				vector<string> enumElements;
				for (int j = 0; j < i; j++)
				{
					enumElements.push_back(tableNames[nameNum + 1 + j]);
					identElement currentElem = FindElemInTableIdents(tableIdents, tableNames[nameNum + 1 + j]);
					if (currentElem.useClass != "null")
					{
						AddErrorToTable(allLexems[lexNum - 1 - (i - j - 1) * 2].lineNumber, allLexems[lexNum - 1 - (i - j - 1) * 2].charNumber, 101);
						isOk = false;
					}
				}
				if (isOk)
				{
					for (int j = 0; j < i; j++)
					{
						identElement tElement{ "enumer" };
						tableIdents.insert(pair<string, identElement>(enumElements[j], tElement));
					}
					identElement tElement{ currentLocation, ENUMS, enumElements };
					tableIdents.insert(pair<string, identElement>(tableNames[nameNum], tElement));
				}
			}
		}
		nameNum += i + 1;

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
		bool isInt = false, isChar = false;;
		if (allLexems[lexNum].lexem == intc || allLexems[lexNum].lexem == plus || allLexems[lexNum].lexem == minus)	
			isInt = true;
		else 
			isChar = true;
		Const(externalSymbols);
		Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		if (allLexems[lexNum].lexem == intc || allLexems[lexNum].lexem == plus || allLexems[lexNum].lexem == minus)
			isChar = false;
		else
			isInt = false;
		Const(followers);

		if (!worksWithArray)
		{
			if (isInt || isChar)
			{
				if (FindElemInTableIdents(tableIdents, tableNames[nameNum]).useClass != "null")
					AddErrorToTable(allLexems[lexNum - 4].lineNumber, allLexems[lexNum - 3].charNumber, 101);
				else if (isInt && lexical_cast<int>(tableValues[valueNum]) < lexical_cast<int>(tableValues[valueNum + 1]))
				{
					identElement tElement{ currentLocation, LIMITEDS, vector<string> { tableValues[valueNum], tableValues[valueNum + 1]} };
					tableIdents.insert(pair<string, identElement>(tableNames[nameNum], tElement));
				}
				else if (tableValues[valueNum] < tableValues[valueNum + 1])
				{
					identElement tElement{ currentLocation, LIMITEDS, vector<string> { tableValues[valueNum], tableValues[valueNum + 1]} };
					tableIdents.insert(pair<string, identElement>(tableNames[nameNum], tElement));
				}
				else
					AddErrorToTable(allLexems[lexNum - 4].lineNumber, allLexems[lexNum - 3].charNumber, 102);
			}
			else
				AddErrorToTable(allLexems[lexNum - 4].lineNumber, allLexems[lexNum - 3].charNumber, 328);
			nameNum++;
		}
		valueNum += 2;
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
		{
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			if (!worksWithArray)
			{
				if (FindElemInTableIdents(tableIdents, tableNames[nameNum]).useClass != "null")
					AddErrorToTable(allLexems[lexNum - 3].lineNumber, allLexems[lexNum - 3].charNumber, 101);
				else
				{
					identElement currentElem = FindElemInTableIdents(tableIdents, tableNames[nameNum + 1]);
					if (currentElem.useClass != "type")
						currentElem = FindElemInTableIdents(tableDummyIdents, tableNames[nameNum + 1]);
					if (currentElem.useClass != "type")
						AddErrorToTable(allLexems[lexNum - 1].lineNumber, allLexems[lexNum - 1].charNumber, 331);
					else
					{
						identElement tElement{ currentLocation, currentElem.typeCode,  currentElem.values };  //vector<string> { tableNames[nameNum + 1]
						tableIdents.insert(pair<string, identElement>(tableNames[nameNum], tElement));
					}
				}
				nameNum++;
			}
			nameNum++;
		}
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
		worksWithArray = true;
		Accept(arraysy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		nameNum++;
		TypeSimple(followers);
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			TypeSimple(followers);
		}
		Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Type(followers);
		worksWithArray = false;
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
		currentLocation = "type";
		TypeSection(externalSymbols);
		externalSymbols = Union(varSection_follow, followers);
		currentLocation = "var";
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
		MakeTableDummyIdents();
		if (allLexems[lexNum].lexem == programsy)
		{
			Accept(programsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);

			if (allLexems[lexNum].lexem == ident)
			{
				identElement progElement{ "prog" };
				tableIdents.insert(pair<string, identElement>(tableNames[nameNum], progElement));
				nameNum++;
			}

			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		Block(block_follow);
		Accept(point, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
}

void MakeTableDummyIdents()
{
	identElement tElement{ "type", SCALARS, vector<string> {"integer"} };
	tableDummyIdents.insert(pair<string, identElement>("integer", tElement));
	
	tElement.values[0] = "boolean";
	tableDummyIdents.insert(pair<string, identElement>("boolean", tElement));
	tElement.values[0] = "real";
	tableDummyIdents.insert(pair<string, identElement>("real", tElement));
	tElement.values[0] = "char";
	tableDummyIdents.insert(pair<string, identElement>("char", tElement));
}