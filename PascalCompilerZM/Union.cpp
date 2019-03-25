#include "pch.h"

#include "Union.h"

vector<int> block_start{ typesy, varsy, beginsy };
vector<int> type_start{ ident, varsy, beginsy };
vector<int> var_start{ ident, beginsy };
vector<int> statement_start{ ident, beginsy, casesy, ifsy, whilesy, repeatsy, forsy };
vector<int> begin_start = statement_start;
vector<int> case_start{ ident, plus, minus, elsesy, endsy};
vector<int> varop_start{ ident };

vector<int> type_follow{ equal, semicolon, ident, leftpar, stringc, charc, intc, floatc, minus, plus, arraysy };
vector<int> var_follow{ comma, colon, semicolon, ident, leftpar, stringc, charc, intc, floatc, minus, plus, arraysy };
vector<int> statement_follow{ semicolon };
vector<int> begin_follow = statement_follow;
vector<int> case_follow{ ofsy, semicolon};
vector<int> varop_follow{ lbracket, comma, rbracket, assign };