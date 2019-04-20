#pragma once

#include <vector>

#include "DECKEY.h"
#include "DECW.h"

using namespace std;

extern vector<int> block_start;
extern vector<int> typeSection_start;
extern vector<int> varSection_start;
extern vector<int> varDef_start;
extern vector<int> typeDef_start;
extern vector<int> statementSection_start;
extern vector<int> type_start;
extern vector<int> typeSimple_start;
extern vector<int> typeRegular_start;
extern vector<int> const_start;
extern vector<int> typeLimited_start;
extern vector<int> typeEnumerated_start;
extern vector<int> number_start;
extern vector<int> string_start;
extern vector<int> sign_start;
extern vector<int> expression_start;
extern vector<int> comparisonOperation_start;
extern vector<int> addend_start;
extern vector<int> multiplicativeOperation_start;
extern vector<int> adaptiveOperation_start;
extern vector<int> statementAssignment_start;
extern vector<int> statement_start;
extern vector<int> statementUnlabeled;
extern vector<int> statementSimple_start;
extern vector<int> statementTransition_start;
extern vector<int> variable_start;

extern vector<int> block_follow;
extern vector<int> typeSection_follow;
extern vector<int> varSection_follow;
extern vector<int> statement_follow;