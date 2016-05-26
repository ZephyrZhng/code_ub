#ifndef EARLEY_H
#define EARLEY_H

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

#include "Earley.h"
#include "CFG.h"

using namespace std;

class EarleyState
{
public:
	EarleyState() = default;
	~EarleyState() = default;
	EarleyState(unsigned int pi, unsigned int dp, unsigned int op);

	friend bool operator ==(const EarleyState& l, const EarleyState& r);

	unsigned int productionIndex;
	unsigned int dotPosition;
	unsigned int originPosition;

private:

};

class EarleyParser
{
public:
	EarleyParser() = default;
	~EarleyParser() = default;
	EarleyParser(const CFG& cfg);

	void predict(const EarleyState& s, int k);
	void scan(const EarleyState& s, const vector<string>& w, int k);
	void complete(const EarleyState& s, int k);
	void parse(const vector<string>& w);

	void displayS(ostream& os = cout);

	void testEarley();

private:
	CFG g;
	vector<vector<EarleyState>> S;

};


#endif /*EARLEY_H*/