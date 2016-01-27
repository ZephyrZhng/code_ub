#ifndef __SYNTAX_ANALYZER_H
#define __SYNTAX_ANALYZER_H

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

#include "../grammar_ch/grammar.h"

using namespace std;

class LL1Item
{
public:
	LL1Item() = default;
	LL1Item(unsigned int myI, unsigned int myJ);

	unsigned int i;
	unsigned int j;

private:

};

class LR0Item
{
public:
	LR0Item() = default;
	LR0Item(unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myDotPos);

	unsigned int lProductionRuleIdx;
	unsigned int rProductionRuleIdx;
	unsigned int dotPos;

	friend bool operator < (const LR0Item& i1, const LR0Item& i2);
	friend bool operator ==(const LR0Item& i1, const LR0Item& i2);
	friend bool operator > (const LR0Item& i1, const LR0Item& i2);
	friend bool operator !=(const LR0Item& i1, const LR0Item& i2);

private:

};

class LR1Item
{
public:
	LR1Item() = default;
	LR1Item(const LR0Item& myLR0Item, char myLookaheadSymbol);
	LR1Item(unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myDotPos, char myLookaheadSymbol);

	unsigned int lProductionRuleIdx;
	unsigned int rProductionRuleIdx;
	unsigned int dotPos;
	char lookaheadSymbol;

	friend bool operator < (const LR1Item& i1, const LR1Item& i2);
	friend bool operator ==(const LR1Item& i1, const LR1Item& i2);
	friend bool operator > (const LR1Item& i1, const LR1Item& i2);
	friend bool operator !=(const LR1Item& i1, const LR1Item& i2);

private:

};

typedef enum {SHIFT, REDUCE, ACCEPT, ERR} Action;

class LR0ActionTabItem
{
public:
	LR0ActionTabItem() = default;
	LR0ActionTabItem(const Action& myAction, unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myItemSetIdx);

	Action action;
	unsigned int lProductionRuleIdx;
	unsigned int rProductionRuleIdx;
	unsigned int itemSetIdx;
};

class LR1ActionTabItem
{
public:
	LR1ActionTabItem() = default;
	LR1ActionTabItem(const Action& myAction, unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myItemSetIdx);

	Action action;
	unsigned int lProductionRuleIdx;
	unsigned int rProductionRuleIdx;
	unsigned int itemSetIdx;

private:

};

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer() = default;
	SyntaxAnalyzer(const string& filePath);
	~SyntaxAnalyzer();

	Grammar G;

	void display(const LR0Item& item);
	void display(const LR1Item& item);
	void display(const set<LR0Item>& I);
	void display(const set<LR1Item>& I);

	bool expanded;

	void expandGrammar();

	// LR(0)
	vector<set<LR0Item>> lr0ItemSetFamily;
	vector<vector<LR0ActionTabItem>> lr0ActionTab;
	vector<vector<unsigned int>> lr0GoToTab;
	vector<unsigned int> lr0Stack;
	string lr0Input;
	
	set<LR0Item> LR0Closure(const set<LR0Item>& I);
	set<LR0Item> LR0GoTo(const set<LR0Item>& I, char X);
	void calLR0ItemSetFamily();
	void buildLR0ParseTab();
	bool lr0SyntaxAnalyze(const string& myLR0Input);

	// LR(1)
	vector<set<LR1Item>> lr1ItemSetFamily;
	vector<vector<LR1ActionTabItem>> lr1ActionTab;
	vector<vector<unsigned int>> lr1GoToTab;
	vector<unsigned int> lr1Stack;
	string lr1Input;

	set<LR1Item> LR1Closure(const set<LR1Item>& I);
	set<LR1Item> LR1GoTo(const set<LR1Item>& I, char X);
	void calLR1ItemSetFamily();
	void buildLR1ParseTab();
	bool lr1SyntaxAnalyze(const string& myLR1Input);

	// LL(1)
	vector<vector<LL1Item>> ll1ParseTab;
	vector<char> ll1Stack;
	string ll1Input;

	void buildLL1ParseTab();
	bool ll1SyntaxAnalyze(const string& myLL1Input);

private:

};

#endif /*__SYNTAX_ANALYZER_H*/