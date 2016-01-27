#ifndef __EPSILON_NFA_H
#define __EPSILON_NFA_H

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

class EpsilonNFA
{
public:
	EpsilonNFA();
	EpsilonNFA(const EpsilonNFA& _epsilon_nfa): delta(_epsilon_nfa.delta), s(_epsilon_nfa.s), sigma(_epsilon_nfa.sigma), s_num(_epsilon_nfa.s_num), sigma_num(_epsilon_nfa.sigma_num){}
	~EpsilonNFA() = default;
	void display();
	int get_s_num();
	int get_sigma_num();
	vector<string> delta_x_y(int x, int y);
	void set_delta_x_y(int x, int y, const vector<string>& _delta_x_y);

private:
	vector<vector<vector<string>>> delta;
	vector<string> s;
	vector<string> sigma;
	int s_num;
	int sigma_num;
};

#endif