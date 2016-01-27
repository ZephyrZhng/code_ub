#ifndef __PRODUCTION_RULES_H
#define __PRODUCTION_RULES_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

#include "grammar.h"

using namespace std;

class production_rules
{
public:
	production_rules();
	production_rules(const string& _lPart, const char& _rInputSymbol, const vector<string>& _rStates);

	string lPart;
	char rInputSymbol;
	vector<string> rStates;

private:

};

#endif /*__PRODUCTION_RULES_H*/