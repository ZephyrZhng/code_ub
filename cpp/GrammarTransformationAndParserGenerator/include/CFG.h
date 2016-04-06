#ifndef CFG_H
#define CFG_H

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

using namespace std;

class Production
{
public:
	Production() = default;
	~Production() = default;
	Production(const string& l, const vector<string>& r);

	string left;
	vector<string> right;

private:

};

class CFG
{
public:
	CFG();
	~CFG() = default;

	void findGeneratingSymbols();
	void findReachableSymbols();
	void eliminateUselessSymbols();

	void findNullableSymbols();
	void eliminateEpsilonProductions();

	bool isUnitProduction(const Production& production);
	void findUnitPairs();
	void eliminateUnitProductions();

	void putInCNF();

	void displayGrammar();
	void displayGeneratingSymbols();
	void displayReachableSymbols();
	void displayNullableSymbols();
	void displayUnitPairs();

	int getVariableIndex(const string& a);

	vector<string> v;
	vector<string> t;
	vector<Production> p;
	string s;

	vector<string> generatingSymbols;
	vector<string> reachableSymbols;
	vector<string> nullableSymbols;
	vector<pair<string, string>> unitPairs;

private:

};

#endif /*CFG_H*/