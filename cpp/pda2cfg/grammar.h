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

class Grammar
{
public:
	Grammar();

	vector<char> VN;
	vector<char> VT;
	vector<string> P;
	char S;

private:
	
};

#endif /*__GRAMMAR_H*/