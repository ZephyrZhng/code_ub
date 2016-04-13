#ifndef SLR_H
#define SLR_H

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

class SLRActionEntry
{
public:
	SLRActionEntry() = default;
	~SLRActionEntry() = default;
	SLRActionEntry(const ActionBehavior& b, int i);

	ActionBehavior behavior;
	int index;
	// behavior = shift, index = index of item set
	// behavior = reduce, inde = index of production

private:

};

class SLRParser
{
public:
	SLRParser() = default;
	~SLRParser() = default;
	SLRParser(const CFG& cfg);

	bool constructSLRTable();

	bool parse(const vector<string>& str);

	void displaySLRTable(ostream& os);

	void testConstructSLRTable();
	void testParse();

	vector<vector<SLRActionEntry>> action;
	// row: state(index of item set)
	// column: t union {$}

	vector<vector<int>> goTo;
	// row: state(index of item set)
	// column: v

private:
	CFG g;

};

#endif /*SLR_H*/