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
		cout << "Ошибка " << neededSym << " в строке " << lineNum << " в символе " << posNum << " добавлена в таблицу ошибок!" << endl;
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

void MultiplierParsing()
{
	while (allLexems[lexNum].lexem == ident || allLexems[lexNum].lexem == leftpar 
		|| allLexems[lexNum].lexem == notsy || allLexems[lexNum].lexem == intc
		|| allLexems[lexNum].lexem == floatc || allLexems[lexNum].lexem == stringc
		|| allLexems[lexNum].lexem == charc || allLexems[lexNum].lexem == TRUE
		|| allLexems[lexNum].lexem == FALSE)
	{
		switch (allLexems[lexNum].lexem)
		{
		case intc:
			Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case floatc:
			Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case stringc:
			Accept(stringc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case charc:
			Accept(charc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case TRUE:
			Accept(TRUE, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case FALSE:
			Accept(FALSE, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case ident:
			OperatorVariableParsing();
			break;
		case leftpar:
		{
			Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			ExpressionParsing();
			Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		break;
		case notsy:
		{
			Accept(notsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			MultiplierParsing();
		}
			break;
		default:
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		}
	}
}

void CaseCycle()
{
	ConstMake();
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ConstMake();
	}
	Accept(colon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementMake();
}

void AddendParsing()
{
	MultiplierParsing();
	while (allLexems[lexNum].lexem == divsy || allLexems[lexNum].lexem == modsy || allLexems[lexNum].lexem == andsy
		|| allLexems[lexNum].lexem == star || allLexems[lexNum].lexem == slash)
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
		MultiplierParsing();
	}
}

void ExpressionSimple()
{
	if (allLexems[lexNum].lexem == minus)
		Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	else if (allLexems[lexNum].lexem == plus)
		Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	AddendParsing();
	while (allLexems[lexNum].lexem == plus || allLexems[lexNum].lexem == minus || allLexems[lexNum].lexem == orsy)
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
		default:
			break;
		}
		AddendParsing();
	}
}

void ExpressionParsing()
{
	ExpressionSimple();
	if (allLexems[lexNum].lexem == equal || allLexems[lexNum].lexem == latergreater
		|| allLexems[lexNum].lexem == later || allLexems[lexNum].lexem == greater
		|| allLexems[lexNum].lexem == laterequal || allLexems[lexNum].lexem == greaterequal)
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
		ExpressionSimple();
	}

}

void ConstMake()
{
	switch (allLexems[lexNum].lexem)
	{
	case stringc:
		Accept(stringc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case charc:
		Accept(charc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case minus:
		Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		if (allLexems[lexNum].lexem == intc)
			Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		else
			Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case plus:
		Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		if (allLexems[lexNum].lexem == intc)
			Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		else
			Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case intc:
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case floatc:
		Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void TypeSimple()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case leftpar:
		Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case stringc:
	case charc:
	case minus:
	case plus:
	case floatc:
	case intc:
		ConstMake();
		Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ConstMake();
		break;
	}
}

void VarMake()
{
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	while (allLexems[lexNum].lexem == comma)
	{
		Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
	Accept(colon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	TypeMake();
	Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void VarParsing()
{
	//vector<int> ptra;
	//if (!Belong(allLexems[lexNum].lexem, codes_block))
	//{
	//	Accept(18, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//	SkipToBoth(codes_block, followers);
	//}
	Accept(varsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	VarMake();
	while (allLexems[lexNum].lexem == ident)
		VarMake();
}
 
void TypeMake()
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
		TypeMake();
	}
		break;
	default:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void TypeParsing()
{
	Accept(typesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	TypeMake();
	Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	while (allLexems[lexNum].lexem == ident)
	{
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		TypeMake();
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
}

void OperatorVariableParsing()
{
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	if (allLexems[lexNum].lexem == lbracket)
	{
		Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ExpressionParsing();
		while (allLexems[lexNum].lexem == comma)
		{
			Accept(comma, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			ExpressionParsing();
		}
		Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
}

void OperatorWhileParsing()
{
	Accept(whilesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressionParsing();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementMake();
}

void OperatorRepeatParsing()
{
	Accept(repeatsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementMake();
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementMake();
	}
	Accept(untilsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressionParsing();
}

void OperatorForParsing()
{
	Accept(forsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressionParsing();
	Accept(tosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressionParsing();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementMake();
}

void OperatorIfParsing()
{
	Accept(ifsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressionParsing();
	Accept(thensy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementMake();
	if (allLexems[lexNum].lexem == elsesy)
	{
		Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementMake();
	}
}

void StatementMake()
{
	//vector<int> ptra;
	//if (!Belong(allLexems[lexNum].lexem, statement_start))
	//{
	//	Accept(18, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//	SkipToBoth(statement_start, followers);
	//}
	//if (Belong(allLexems[lexNum].lexem, statement_start))
	//{
	while (allLexems[lexNum].lexem == ident || allLexems[lexNum].lexem == beginsy || allLexems[lexNum].lexem == casesy 
		|| allLexems[lexNum].lexem == ifsy || allLexems[lexNum].lexem == whilesy
		|| allLexems[lexNum].lexem == repeatsy || allLexems[lexNum].lexem == forsy)
		switch (allLexems[lexNum].lexem)
		{
		case ident:
		{
			OperatorVariableParsing();
			Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			ExpressionParsing();
		}
			break;
		case beginsy:
		{
			Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			StatementMake();
			while (allLexems[lexNum].lexem == semicolon)
			{
				Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				StatementMake();
			}
			Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
			break;
		case casesy:
		{
			Accept(casesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			ExpressionParsing();
			Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			CaseCycle();
			while (allLexems[lexNum].lexem == semicolon)
			{
				Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				if (allLexems[lexNum].lexem != elsesy)
					CaseCycle();
				else
				{
					Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
					StatementMake();
					Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				}
			}
			Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
			break;
		case ifsy:
			OperatorIfParsing();
			break;
		case whilesy:
			OperatorWhileParsing();
			break;
		case repeatsy:
			OperatorRepeatParsing();
			break;
		case forsy:
			OperatorForParsing();
			break;
		}
}

void StatementParsing()
{
	StatementMake();
	while (allLexems[lexNum].lexem == semicolon) // && lexNum < lexemsCount)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementMake();
	}
}

void BlockParsing()
{
	//vector<int> ptra;
	//if (!Belong(allLexems[lexNum].lexem, block_start))
	//{
	//	Accept(18, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//	SkipToBoth(block_start, followers);
	//}
	//if (Belong(allLexems[lexNum].lexem, block_start))
	//{
		if (allLexems[lexNum].lexem == typesy)
		{
		//	ptra = Union(var_start, followers);
			TypeParsing();
		}

		if (allLexems[lexNum].lexem == varsy)
		{
		//	ptra = Union(var_start, followers);
			VarParsing();
		}
		Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		//ptra = Union(statement_start, followers);
		StatementParsing();
		//if (!Belong(allLexems[lexNum].lexem, followers))
		//{
		//	Accept(6, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		//	SkipTo(followers);
		//}
		Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	//}
}

void Parsing()
{	
	if (lexemsCount > 0)
	{
		if (allLexems[lexNum].lexem == programsy)
		{
			Accept(programsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
		BlockParsing();
		Accept(point, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
}