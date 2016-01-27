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

#include "dfa.h"

using namespace std;

int main(int argc, char** argv)
{
	DFA dfa("./dfa.txt");
	dfa.display();

	return 0;
}