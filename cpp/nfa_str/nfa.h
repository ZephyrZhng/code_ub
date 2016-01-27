#ifndef __NFA_H
#define __NFA_H

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

class NFA
{
public:
	NFA() = default;
	NFA(const string& filePath) ;
	~NFA() = default;

	void display();

	vector<string> K;
	vector<string> Sigma;
	vector<vector<vector<string>>> Delta;
	string q0;
	vector<string> F;

private:
	
};

#endif /*__NFA_H*/