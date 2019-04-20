#include "pch.h"

#include "Union.h"

vector<int> block_start{ typesy, varsy, beginsy };
vector<int> typeSection_start = { typesy };
vector<int> varSection_start{ varsy };
vector<int> statementSection_start{ beginsy };
vector<int> varDef_start{ ident };
vector<int> typeDef_start = varDef_start;
vector<int> type_start{ ident, leftpar, stringc, charc, minus, plus, intc, floatc, arraysy };
vector<int> typeSimple_start{ ident, leftpar, stringc, charc, minus, plus, intc, floatc };
vector<int> typeRegular_start{ arraysy };
vector<int> const_start{ charc, stringc, intc, floatc, minus, plus };
vector<int> number_start{ intc, floatc };
vector<int> string_start{ charc, stringc };
vector<int> sign_start{ minus, plus };
vector<int> typeLimited_start = const_start;
vector<int> typeEnumerated_start{ leftpar };
vector<int> expression_start{ minus, plus, ident, charc, stringc, intc, floatc, leftpar, notsy };
vector<int> comparisonOperation_start{ equal, latergreater, later, greater, laterequal, greaterequal };
vector<int> addend_start{ ident, charc, stringc, intc, floatc, leftpar, notsy };
vector<int> multiplicativeOperation_start{ star, slash, divsy, modsy, andsy };
vector<int> adaptiveOperation_start{ plus, minus, orsy };
vector<int> statement_start{ intc, beginsy, ident, ifsy, whilesy, repeatsy, forsy, gotosy };
vector<int> statementUnlabeled{ beginsy, ident, ifsy, whilesy, repeatsy, forsy, gotosy };
vector<int> statementSimple_start{ assign, ident, charc, stringc, intc, floatc, leftpar, notsy, gotosy };
vector<int> statementAssignment_start{ assign, ident, charc, stringc, intc, floatc, leftpar, notsy };
vector<int> statementTransition_start{ gotosy };
vector<int> variable_start = varDef_start;


vector<int> block_follow{ point };
vector<int> typeSection_follow{ varsy, beginsy };
vector<int> varSection_follow{ beginsy };
vector<int> statement_follow{ semicolon, endsy };

