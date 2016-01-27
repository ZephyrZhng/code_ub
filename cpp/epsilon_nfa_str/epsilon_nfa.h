#ifndef __EPSILON_FNA_H
#define __EPSILON_FNA_H

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

class EpsilonNFA
{
public:
	EpsilonNFA() = default;
	EpsilonNFA(const string& filePath) ;
	~EpsilonNFA() = default;

	void display();

	vector<string> K;
	vector<string> Sigma;
	vector<vector<vector<string>>> Delta;
	string q0;
	vector<string> F;

private:
	
};

#endif /*__EPSILON_FNA_H*/