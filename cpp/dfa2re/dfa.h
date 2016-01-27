#ifndef __DFA_H
#define __DFA_H

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

class DFA
{
public:
	DFA();
	DFA(const DFA& dfa);
	~DFA() = default;

	string delta_x_y(int x, int y);

	void display();

	vector<string> get_K();
	vector<string> get_Sigma();
	string get_S();
	vector<string> get_F();
	int get_KSize();
	int get_SigmaSize();
	int get_FSize();

private:
	vector<vector<string>> Delta; // row count from 1
	vector<string> K; // count from 1
	vector<string> Sigma;
	string S;
	vector<string> F;
	int KSize;
	int SigmaSize;
	int FSize;
};

#endif /*__DFA_H*/