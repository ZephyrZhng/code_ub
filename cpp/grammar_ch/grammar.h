#ifndef __GRAMMAR_H
#define __GRAMMAR_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

class ProductionRule
{
public:
	ProductionRule();
	ProductionRule(const char& l, const vector<string>& r);

	void display();

	char lPart;
	vector<string> rPartSet;

private:

};

class Grammar
{
public:
	Grammar(const string& filePath);

	void calNullableFirstFollow();

	void display();
	bool isNonTerminal(char ch);
	int getProductionRuleByLeft(ProductionRule& pr, char lPart); // return index of production rule which has lPart as left part
	set<char> getFirst(char ch);
	set<char> getFirst(string str);
	set<char> getFollow(char ch);

	vector<char> VN;
	vector<char> VT;
	vector<ProductionRule> P;
	char S;

	vector<bool> nullable; // for symbols in VN and VT, elements in VT are followed by elements in VN
	vector<set<char>> first; // for symbols in VN and VT, elements in VT are followed by elements in VN
	vector<set<char>> follow; // only for symbols in VN

private:
	
};

#endif /*__GRAMMAR_H*/