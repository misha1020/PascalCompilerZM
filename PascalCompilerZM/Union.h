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
//extern
//extern
//extern
//extern
//extern


extern vector<int> block_follow;
extern vector<int> typeSection_follow;
extern vector<int> varSection_follow;