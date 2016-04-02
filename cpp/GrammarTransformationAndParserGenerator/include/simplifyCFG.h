#ifndef SIMPLIFYCFG_H
#define SIMPLIFYCFG_H

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
#include <vector>

using namespace std;

class ContextFreeGrammar
{
public:
	ContextFreeGrammar() = default;
	~ContextFreeGrammar() = default;
	ContextFreeGrammar(const string& cfgFilePath);

	template<typename T>
	void displaySet(const set<T>& Set);

	void displayCFG();

	bool isNonTerminal(char ch);

	bool isTerminal(char ch);

	bool canDeriveNonTerminalString(char ch);


	template<typename T>
	void setReplace(set<T>& destSet, const set<T>& srcSet);

	template<typename T>
	bool setEqual(set<T>& s1, set<T>& s2);

	void _eliminateUselessSymbolics();

	void eliminateUselessSymbolics1();

	void eliminateUselessSymbolics2();

	void getV0(set<char>& v0);

	void _eliminateEpsilonProductionRules();

	void eliminateEpsilonProductionRules();

	void _eliminateSingleProductionRules(char ch, set<string>& Set);

	void eliminateSingleProductionRules();

	void toCNF();

	int order(char a, char b);

	int checkSmall2Big(string str);

	void checkLeftRecursion(string str, vector<int>& left_recursion_pos_v);

	bool checkGNFForm(string str);

	void toGNF();

	void simplifyCFG();

private:
	set<char>		vn;
	set<char>		vt;
	set<string>		p;
	char			s;
	char 			c;
};

#endif /*SIMPLIFYCFG_H*/