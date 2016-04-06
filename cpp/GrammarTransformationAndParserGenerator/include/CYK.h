#ifndef CYK_H
#define CYK_H

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "CFG.h"

using namespace std;

class CYKParser
{
public:
	CYKParser() = default;
	~CYKParser() = default;

	bool parse(const vector<string>& s);

private:
	CFG g; // in CNF

};

#endif /*CYK_H*/