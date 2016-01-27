#ifndef __PDA_H
#define __PDA_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

#include "grammar.h"

using namespace std;

class delta_r_tuple
{
public:
	delta_r_tuple();
	delta_r_tuple(const char& s, const string& str);
	char state;
	string strToPush;

private:

};

typedef vector<delta_r_tuple> delta_r_set;

class PDA
{
public:
	PDA();
	void display();
	Grammar toCFG();

private:
	vector<char> K;
	vector<char> Sigma;
	vector<char> Tau;
	char q0;
	char Z0;
	vector<char> F;
	vector<vector<vector<delta_r_set>>> delta;
};

#endif /*__PDA_H*/