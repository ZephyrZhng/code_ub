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

class PPart
{
public:
	string LPart;
	vector<string> RPart;

	PPart();	
	PPart(const string& l, const vector<string>& r);

private:
	
};

class Grammar
{
public:
	Grammar();
	~Grammar();

	void display();

	bool terminal(char x);
	bool nonterminal(char x);

	void first(set<char>& firstx, char x);
	// iteration of first() function
	void follow(set<char>& followB, char B);
	// Segmentation fault (core dumped)

	vector<char> getVN();
	vector<char> getVT();

private:
	vector<char> VN;
	vector<char> VT;
	vector<string> RawP;
	vector<PPart> P;
	char S;
};

#endif /*__GRAMMAR_H*/