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
	CYKParser(const CFG& cfg);

	bool parse(const vector<string>& str);

	void displayCYKTable(ostream& os = cout);

	void testCYK();

private:
	CFG g; // in CNF
	int n;
	int r;
	vector<vector<vector<bool>>> m;
	vector<vector<vector<string>>> t;
};

#endif /*CYK_H*/