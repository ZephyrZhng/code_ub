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
#include "epsilon_nfa_to_nfa_converter.h"
#include "nfa.h"

// epsilon_nfa
// delta	0			1		2		epsilon
// 0		0			phi		phi		1
// 1		phi			1		phi		2
// 2		phi			phi		2		phi

// nfa
// delta	0			1		2
// 0		0 1 2		1 2		2
// 1		phi			1 2		2
// 2		phi			phi		2

int main()
{
	EpsilonNFA enfa;
	enfa.display();
	NFA nfa;



	return 0;
}