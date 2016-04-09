#ifndef LL1PARSER_H
#define LL1PARSER_H

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

class LL1Parser
{
public:
	LL1Parser() = default;
	~LL1Parser() = default;
	LL1Parser(const CFG& cfg);

	bool setM(int row, int column, int index);
	bool constructLL1Table();

	void displayLL1Table();

	bool parse(const vector<string>& str);

private:
	CFG g;
	vector<vector<int>> m;

};

#endif /*LL1PARSER_H*/