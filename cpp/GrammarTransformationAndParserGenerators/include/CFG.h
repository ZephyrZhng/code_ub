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

	void display(ostream& os = cout);

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

class LR1Item
{
public:
	LR1Item() = default;
	~LR1Item() = default;
	LR1Item(unsigned int pi, unsigned int dp, const string& la);

	friend bool operator ==(const LR1Item& l, const LR1Item& r);

	unsigned int productionIndex;
	unsigned int dotPosition;
	string lookahead;

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

	void computeFirst();
	vector<string> computeFirst(const vector<string>& str);

	void computeFollow();

	void augmentGrammar();

	vector<LR0Item> closure(const vector<LR0Item>& is);
	vector<LR0Item> goTo(const vector<LR0Item>& is, const string& x);
	// x in (v union t)
	void constructCanonicalLR0Collection();

	vector<LR1Item> closure(const vector<LR1Item>& is);
	vector<LR1Item> goTo(const vector<LR1Item>& is, const string& x);
	// x in (v union t)
	void constructCanonicalLR1Collection();

	bool hasSameCore(const vector<LR1Item>& is1, const vector<LR1Item>& is2);

	void displayGrammar(ostream& os = cout);
	void displayGeneratingSymbols(ostream& os = cout);
	void displayReachableSymbols(ostream& os = cout);
	void displayNullableSymbols(ostream& os = cout);
	void displayUnitPairs(ostream& os = cout);
	void displayFirst(ostream& os = cout);
	void displayFollow(ostream& os = cout);
	void displayLR0ItemSet(const vector<LR0Item>& is, ostream& os = cout);
	void displayCanonicalLR0Collection(ostream& os = cout);
	void displayLR1ItemSet(const vector<LR1Item>& is, ostream& os = cout);
	void displayCanonicalLR1Collection(ostream& os = cout);

	int getVariableIndex(const string& a);
	int getTerminalIndex(const string& a);
	int getProductionIndex(const Production& production);
	int getFirstIndex(const string& symbol);
	int getFollowIndex(const string& symbol);
	int getLR0ItemSetIndex(const vector<LR0Item>& is);
	int getLR1ItemSetIndex(const vector<LR1Item>& is);

	void testEliminateUselessSymbols();
	void testEliminateUnitProductions();
	void testPutInCNF();
	void testComputeFirst();
	void testComputeFollow();
	void testLR0Closure1(int i, const vector<LR0Item>& Ii, ostream& os = cout);
	void testLR0Closure();
	void testLR0GoTo();
	void testConstructCanonicalLR0Collection();
	void testConstructCanonicalLR1Collection();

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
	// index of start state(constructed by item set containing S' -> S): 0

	vector<vector<LR1Item>> canonicalLR1Collection;
	// index of start state(constructed by item set containing S' -> S): 0

private:

};

#endif /*CFG_H*/