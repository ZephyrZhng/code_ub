#ifndef __EPSILON_NFA_TO_NFA_CONVERTER_H
#define __EPSILON_NFA_TO_NFA_CONVERTER_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

#include "epsilon_nfa.h"
#include "nfa.h"

using namespace std;

class EpsilonNFA2NFAConverter
{
public:
	EpsilonNFA2NFAConverter(const NFA& _nfa): nfa(_nfa){}
	~EpsilonNFA2NFAConverter() = default;
	void find_epsilon_edges();
	void step2();
	void step3();
	void step4();

private:
	NFA nfa;
};

#endif