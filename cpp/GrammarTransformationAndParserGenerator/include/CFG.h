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

	void display();
	void display(fstream& f);

	string left;
	vector<string> right;

private:

};

class LR0Item
{
public:
	LR0Item() = default;
	~LR0Item() = default;
	LR0Item(int pi, int dp);

	friend bool operator ==(const LR0Item& l, const LR0Item& r);

	unsigned int productionIndex;
	unsigned int dotPosition;
	// dotPosition == i <==> dot lies before the production.right[i]
	// dotPosition == production.right[i].size() <==> dot lies at the end of production.right

	// A -> epsilon: A -> .

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
	int getTerminalIndex(const string& a);

	int getFirstIndex(const string& symbol);
	int getFollowIndex(const string& symbol);
	void computeFirst();
	vector<string> computeFirst(const vector<string>& str);
	void computeFollow();

	void displayFirst();
	void displayFollow();

	void augmentGrammar();

	vector<LR0Item> closure(const vector<LR0Item>& is);
	vector<LR0Item> goTo(const vector<LR0Item>& is, const string& x);
	// x in (v union t)

	void constructCanonicalLR0Collection();

	vector<string> v;
	vector<string> t;
	vector<Production> p;
	string s;

	vector<string> generatingSymbols;
	vector<string> reachableSymbols;
	vector<string> nullableSymbols;
	vector<pair<string, string>> unitPairs;

	vector<pair<string, vector<string>>> first;
	vector<pair<string, vector<string>>> follow;

	bool augmented;

	vector<vector<LR0Item>> canonicalLR0Collection;

private:

};

#endif /*CFG_H*/