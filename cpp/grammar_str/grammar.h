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

typedef vector<string> rPart;

class ProductionRule
{
public:
	ProductionRule();
	ProductionRule(const string& l, const vector<rPart>& r);

	string lPart;
	vector<rPart> rPartSet;

private:

};

class Grammar
{
public:
	Grammar(const string& filePath);

	void display();

	vector<string> VN;
	vector<string> VT;
	vector<ProductionRule> P;
	string S;

private:
	
};

#endif /*__GRAMMAR_H*/