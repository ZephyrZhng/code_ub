#ifndef __DFA2RE_H
#define __DFA2RE_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

#include "dfa.h"

using namespace std;

class DFA2REConverter
{
public:
	DFA2REConverter(const DFA& _dfa);
	~DFA2REConverter() = default;

	string to_re();

private:
	vector<string> get_edge_from_qi_to_qj(int i, int j);
	bool check_concatenate(string re);
	string simplify_closure(string re);
	string simplify_concatenate(string re1, string re2);
	string simplify_union(string re1, string re2);
	string r_k_i_j(int k, int i, int j);

	DFA dfa;
	vector<vector<vector<string>>> r;

};

#endif /*__DFA2RE_H*/