#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

#include "tokenizer.h"
#include "parser.h"

int main(int argc, char** argv)
{
	Tokenizer t;
	Token tk;

	Parser p;
	p.parse();

	p.showIntegerConstantList();
	p.showRealConstantList();
	p.showSymbolList();

	p.showQuaternionSequence();

	return 0;
}