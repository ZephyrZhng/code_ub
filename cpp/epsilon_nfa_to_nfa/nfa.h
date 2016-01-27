#ifndef __NFA_H
#define __NFA_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

using namespace std;

class NFA
{
public:
	NFA() = default;
	NFA(const NFA& _nfa): delta(_nfa.delta), s(_nfa.s), sigma(_nfa.sigma), s_num(_nfa.s_num), sigma_num(_nfa.sigma_num){}
	~NFA() = default;
	void display();
	int get_s_num();
	int get_sigma_num();

private:
	vector<vector<vector<string>>> delta;
	vector<string> s;
	vector<string> sigma;
	int s_num;
	int sigma_num;
};

#endif