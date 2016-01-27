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

#include "epsilon_nfa.h"

using namespace std;

int main(int argc, char** argv)
{
	EpsilonNFA enfa("./epsilon_nfa.txt");
	enfa.display();

	return 0;
}