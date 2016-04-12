#include "CFG.h"
#include "utility.h"

void CFG::testLR0Closure1(int i, const vector<LR0Item>& Ii, ostream& os)
{
	os << "I" << i << ":" << endl;
	displayLR0ItemSet(Ii, os);
	vector<LR0Item> closureIi = closure(Ii);
	os << "closureI" << i << ":" << endl;
	displayLR0ItemSet(closureIi, os);
}

void CFG::testLR0Closure()
{
	// use grammar for test of closure, goto, canonical lr0 collection
	// E -> E + T | T
	// T -> T * F | F
	// F -> ( E ) | 0

	augmentGrammar();

	fstream f("../src/SLRLog");

	testLR0Closure1(0, vector<LR0Item>({
		LR0Item(6, 0)
	}), f);

	testLR0Closure1(1, vector<LR0Item>({
		LR0Item(6, 1), LR0Item(0, 1)
	}), f);

	testLR0Closure1(2, vector<LR0Item>({
		LR0Item(1, 1), LR0Item(2, 1)
	}), f);

	testLR0Closure1(3, vector<LR0Item>({
		LR0Item(3, 1)
	}), f);

	testLR0Closure1(4, vector<LR0Item>({
		LR0Item(4, 1)
	}), f);

	testLR0Closure1(5, vector<LR0Item>({
		LR0Item(5, 1)
	}), f);

	testLR0Closure1(6, vector<LR0Item>({
		LR0Item(0, 2)
	}), f);

	testLR0Closure1(7, vector<LR0Item>({
		LR0Item(2, 2)
	}), f);

	testLR0Closure1(8, vector<LR0Item>({
		LR0Item(0, 1), LR0Item(4, 2)
	}), f);

	testLR0Closure1(9, vector<LR0Item>({
		LR0Item(0, 3), LR0Item(2, 1)
	}), f);

	testLR0Closure1(10, vector<LR0Item>({
		LR0Item(2, 3)
	}), f);

	testLR0Closure1(11, vector<LR0Item>({
		LR0Item(4, 3)
	}), f);

	f.close();
}
