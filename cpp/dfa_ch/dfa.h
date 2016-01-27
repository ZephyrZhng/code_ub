#ifndef __DFA_H
#define __DFA_H

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

class DFA
{
public:
	DFA() = default;
	DFA(const string& filePath);
	~DFA() = default;

	void display();

	vector<char> K;
	vector<char> Sigma;
	vector<vector<char>> Delta;
	char q0;
	vector<char> F;

private:
	
};

#endif /*__DFA_H*/