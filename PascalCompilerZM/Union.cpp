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
vector<int> expression_start{ minus, plus, ident, charc, stringc, intc, floatc, leftpar, notsy};
vector<int> comparisonOperation_start{ equal, latergreater, later, greater, laterequal, greaterequal};
vector<int> addend_start{ ident, charc, stringc, intc, floatc, leftpar, notsy };
vector<int> multiplicativeOperation_start{ star, slash, divsy, modsy, andsy};
vector<int> adaptiveOperation_start{ plus, minus, orsy };
vector<int> statementAssignment_start{ assign, ident, charc, stringc, intc, floatc, leftpar, notsy };



vector<int> block_follow{ point };
vector<int> typeSection_follow{ varsy, beginsy };
vector<int> varSection_follow{ beginsy };



//vector<int> type_follow{ equal, semicolon, ident, leftpar, stringc, charc, intc, floatc, minus, plus, arraysy };
//vector<int> var_follow{ comma, colon, semicolon, ident, leftpar, stringc, charc, intc, floatc, minus, plus, arraysy };
//vector<int> statement_follow{ semicolon };
//vector<int> begin_follow = statement_follow;
//vector<int> case_follow{ ofsy, semicolon};
//vector<int> varop_follow{ lbracket, comma, rbracket, assign };