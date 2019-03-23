#include "pch.h"

#include "Syntactic.h"

int lexNum = 0;
stack<char> brackets;

void Accept(int neededSym, int currentSym, int lineNum, int posNum)
{
	if (currentSym != neededSym && lexNum < lexemsCount)
	{
		AddErrorToTable(lineNum, posNum, neededSym);
		cout << "������ " << neededSym << " � ������ " << lineNum << " � ������� " << posNum << " ��������� � ������� ������!" << endl;
	}
	lexNum++;
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
			Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
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

void ExpressionParsing() // ?????????/
{
	ExpressionSimple();
	while (allLexems[lexNum].lexem == equal || allLexems[lexNum].lexem == latergreater
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

void Type()
{

}

void ArrayInBrackets()
{
	ExpressionParsing();
	Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressionParsing();
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
	case intc:
		ConstMake();
		Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ConstMake();
		break;
	}
}


void ArrayMake()
{
	ArrayInBrackets();
		//Type();
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
		TypeSimple();
		break;
	case arraysy:
	{
		Accept(arraysy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		TypeSimple();
		while (allLexems[lexNum].lexem == comma)
			TypeSimple();
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
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		TypeMake();
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
			ExpressionParsing();
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
	while (allLexems[lexNum].lexem == ident || allLexems[lexNum].lexem == ifsy || allLexems[lexNum].lexem == whilesy
		|| allLexems[lexNum].lexem == repeatsy || allLexems[lexNum].lexem == forsy)
	{
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
		case ifsy:
		{
			OperatorIfParsing();
		}
			break;
		case casesy:
		{
			ExpressionParsing();
			Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			CaseCycle();
			while (allLexems[lexNum].lexem == semicolon)
			{
				Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				CaseCycle();
			}
			Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		}
			break;
		case whilesy:
		{
			OperatorWhileParsing();
		}
			break;
		case repeatsy:
		{
			OperatorRepeatParsing();
		}
			break;
		case forsy:
			OperatorForParsing();
			break;
		}
	}
}

void StatementParsing()
{
	//if (lexNum < lexemsCount - 1)
	//{
	StatementMake();
	while (allLexems[lexNum].lexem == semicolon) // && lexNum < lexemsCount)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementMake();
	}
	//if (lexNum == lexemsCount - 1)
	//	return;
}


void BlockParsing()
{
	if (allLexems[lexNum].lexem == typesy)
		TypeParsing();
	if (allLexems[lexNum].lexem == varsy)
		VarParsing();
	Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementParsing();
	Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
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