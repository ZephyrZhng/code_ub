#ifndef CLR_H
#define CLR_H

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
#include "LR.h"

using namespace std;

class CLRActionEntry
{
public:
	CLRActionEntry() = default;
	~CLRActionEntry() = default;
	CLRActionEntry(const ActionBehavior& b, int i);	

	ActionBehavior behavior;
	int index;
	// behavior = shift, index = index of item set
	// behavior = reduce, inde = index of production

private:

};

class CLRParser
{
public:
	CLRParser() = default;
	~CLRParser() = default;
	CLRParser(const CFG& cfg);

	bool constructCLRTable();

	bool parse(const vector<string>& str);

	void displayCLRTable(ostream& os = cout);

	void testConstructCLRTable();

	vector<vector<CLRActionEntry>> action;
	// row: state(index of item set)
	// column: t union {$}

	vector<vector<int>> goTo;
	// row: state(index of item set)
	// column: v

private:
	CFG g;

};

#endif /*CLR_H*/