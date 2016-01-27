#include "production_rules.h"

production_rules::production_rules()
{

}

production_rules::production_rules(const string& _lPart, const char& _rInputSymbol, const vector<string>& _rStates):
lPart(_lPart), rInputSymbol(_rInputSymbol), rStates(_rStates)
{

}