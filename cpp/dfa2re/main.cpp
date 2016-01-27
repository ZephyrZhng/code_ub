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
#include "dfa2re.h"

using namespace std;

int main(int argc, char** argv)
{
	DFA dfa;
	dfa.display();

	DFA2REConverter converter(dfa);
	cout << "re: " << 
	converter.to_re() 
	<< endl
	;

	return 0;
}