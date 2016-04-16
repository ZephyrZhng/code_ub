#ifndef LALR_H
#define LALR_H

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

class LALRActionEntry
{
public:
	LALRActionEntry() = default;
	~LALRActionEntry() = default;
	LALRActionEntry(const ActionBehavior& b, int i);

	ActionBehavior behavior;
	int index;
	// behavior = shift, index = index of item set
	// behavior = reduce, inde = index of production

private:

};

class LALRParser
{
public:
	LALRParser() = default;
	~LALRParser() = default;
	LALRParser(const CFG& cfg);

	bool constructLALRTable();

	bool parse(const vector<string>& str);

	// void displayLALRTable(ostream& os);

	// void testConstructLALRTable();
	// void testParse();

	vector<vector<LALRActionEntry>> action;
	// row: state(index of item set)
	// column: t union {$}

	vector<vector<int>> goTo;
	// row: state(index of item set)
	// column: v

private:
	CFG g;

};

#endif /*LALR_H*/