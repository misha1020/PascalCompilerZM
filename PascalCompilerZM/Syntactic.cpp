#include "pch.h"

#include "Syntactic.h"

int lexNum = 0;


void Accept(int neededSym, int currentSym, int lineNum, int posNum)
{
	if (currentSym != neededSym)
	{
		AddErrorToTable(lineNum, posNum, neededSym);
		cout << "Ошибка " << neededSym << " в строке " << lineNum << " в символе " << posNum << " добавлена в таблицу ошибок!" << endl;
	}
	if (lexNum < lexemsCount - 1)
		lexNum++;
}

void ExpressionOperations()
{
	while (allLexems[lexNum].lexem == plus || allLexems[lexNum].lexem == minus
		|| allLexems[lexNum].lexem == star || allLexems[lexNum].lexem == slash)
	{
		switch (allLexems[lexNum].lexem)
		{
		case plus:
			Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case minus:
			Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case star:
			Accept(star, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		case slash:
			Accept(slash, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			break;
		}
		ExpressionPlusMinus();
	}
}

void ExpressionPlusMinus()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case intc:
		Accept(intc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case floatc:
		Accept(floatc, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	default:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
}

void ExpressionInOutBrackets()
{
	if (allLexems[lexNum].lexem == minus)
	{
		Accept(minus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ExpressionPlusMinus();
		ExpressionOperations();
	}
	else if (allLexems[lexNum].lexem == plus)
	{
		Accept(plus, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ExpressionPlusMinus();
		ExpressionOperations();
	}
	else if (allLexems[lexNum].lexem == ident || allLexems[lexNum].lexem == intc || allLexems[lexNum].lexem == floatc)
	{
		ExpressionPlusMinus();
		ExpressionOperations();
	}
	else
	{
		switch (allLexems[lexNum].lexem)
		{
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
		default:
			break;
		}
	}
}

void BooleanCheck()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case TRUE:
		Accept(TRUE, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case FALSE:
		Accept(FALSE, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	default:
		Accept(TRUE, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);	
		break;
	}
}

void ExpressionForNot()
{
	Accept(notsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	BooleanCheck();
}

bool ExpressiionMake()
{
	if (allLexems[lexNum].lexem == leftpar)
	{
		Accept(leftpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		if (allLexems[lexNum].lexem == notsy)
		{
			ExpressionForNot();
			Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			return true;
		}
		ExpressionInOutBrackets();
		Accept(rightpar, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		// После право скобки продолжается выржание
		return false;
	}
	else if (allLexems[lexNum].lexem == notsy)
	{
		ExpressionForNot();
		return true;
	}
	else
	{
		ExpressionInOutBrackets();
		return false;
	}
}

void ExpressiongParsing()
{
	bool NotExpression = ExpressiionMake();
	if (!NotExpression)
		if (allLexems[lexNum].lexem == andsy)
		{
			Accept(andsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			BooleanCheck();
		}
		else if (allLexems[lexNum].lexem == orsy)
		{
			Accept(orsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
			BooleanCheck();
		}
		else
		{
			switch (allLexems[lexNum].lexem)
			{
			case equal:
				Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				ExpressiionMake();
				break;
			case latergreater:
				Accept(latergreater, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				ExpressiionMake();
				break;
			case later:
				Accept(later, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				ExpressiionMake();
				break;
			case greater:
				Accept(greater, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				ExpressiionMake();
				break;
			case laterequal:
				Accept(laterequal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				ExpressiionMake();
				break;
			case greaterequal:
				Accept(greaterequal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
				ExpressiionMake();
				break;
			default:
				break;
			}
		}
}

void StatementSwitch()
{
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		OperatorVariableParsing();
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
	default:
		break;
	}
}

void StatementParsing()
{
	StatementSwitch();
	while (allLexems[lexNum].lexem == semicolon)
	{
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementSwitch();
	}		
}

void OperatorVariableParsing()
{
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	if (allLexems[lexNum].lexem == lbracket)
	{
		Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		ArrayInBrackets();
		while (allLexems[lexNum].lexem == comma)
			ArrayInBrackets();
		Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
}

void OperatorWhileParsing()
{
	Accept(whilesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(beginsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementParsing();
	Accept(endsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void OperatorRepeatParsing()
{
	Accept(repeatsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementParsing();
	while (allLexems[lexNum].lexem == semicolon)
	{
		StatementParsing();
		Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	}
	Accept(untilsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
}

void OperatorForParsing()
{
	Accept(forsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(assign, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
	Accept(tosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
	Accept(dosy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementParsing();
}

void OperatorIfParsing()
{
	Accept(ifsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
	Accept(thensy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	StatementParsing();
	if (allLexems[lexNum].lexem == elsesy)
	{
		Accept(elsesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		StatementParsing();
	}
}

void Type()
{

}

void ArrayInBrackets()
{
	ExpressiongParsing();
	Accept(twopoints, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ExpressiongParsing();
}

void ArrayMake()
{
	Accept(arraysy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(lbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	ArrayInBrackets();
	while (allLexems[lexNum].lexem == comma)
		ArrayInBrackets();
	Accept(rbracket, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ofsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);	//Type();
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
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);	//Type();
		break;
	case arraysy:
		ArrayMake();
		break;
	default:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);	
		break;
	}
	Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void VarParsing()
{
	Accept(varsy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	while (allLexems[lexNum].lexem == ident)
		VarMake();
}


void TypeMake()
{
	Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	Accept(equal, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	switch (allLexems[lexNum].lexem)
	{
	case ident:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	case arraysy:
		ArrayMake();
		break;
	default:
		Accept(ident, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
		break;
	}
	Accept(semicolon, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
}

void TypeParsing()
{
	Accept(typesy, allLexems[lexNum].lexem, allLexems[lexNum].lineNumber, allLexems[lexNum].charNumber);
	while (allLexems[lexNum].lexem == ident)
		TypeMake();
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

void ProgramParsing()
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

void Parsing()
{
	//for (int i = 0; i < lexemsCount; i++)
	//	cout << allLexems[i].lexem << "  " << allLexems[i].lineNumber << "  " << allLexems[i].charNumber << endl;
	
	if (lexemsCount > 0)
	{
		ProgramParsing();
	}
}