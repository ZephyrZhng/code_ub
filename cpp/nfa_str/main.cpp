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

#include "nfa.h"

using namespace std;

int main(int argc, char** argv)
{
	NFA nfa("./nfa.txt");
	nfa.display();

	return 0;
}