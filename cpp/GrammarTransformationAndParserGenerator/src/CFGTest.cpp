#include "CFG.h"
#include "utility.h"

void CFG::testEliminateUselessSymbols()
{
	// test eliminateUselessSymbols
	v = {
		"S",
		"A",
		"B",
		"C",
	};
	// variables begin with "_" are built-in variables

	t = {
		"a",
		"b",
	};

	p = {
		Production("S", {"A"}),
		Production("S", {"B"}),
		Production("A", {"a", "B"}),
		Production("A", {"b", "S"}),
		Production("A", {"b"}),
		Production("B", {"A", "B"}),
		Production("B", {"B", "a"}),
		Production("C", {"A", "S"}),
		Production("C", {"b"}),
	};

	s = "S";

	displayGrammar();
	eliminateUselessSymbols();
}

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

	fstream f("../src/LogLR0Closure");
	f.clear();

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

void CFG::testLR0GoTo()
{
	fstream f("../src/LogLR0GoTo");
	f.clear();

	displayLR0ItemSet(goTo(closure(vector<LR0Item>({
		LR0Item(6, 0)
	})), "E"), f); // I0 ->(E) I1
	displayLR0ItemSet(goTo(closure(vector<LR0Item>({
		LR0Item(6, 0)
	})), "T"), f); // I0 ->(T) I2
	displayLR0ItemSet(goTo(closure(vector<LR0Item>({
		LR0Item(6, 0)
	})), "0"), f); // I0 ->(0) I5
	displayLR0ItemSet(goTo(closure(vector<LR0Item>({
		LR0Item(6, 0)
	})), "("), f); // I0 ->(() I4
	displayLR0ItemSet(goTo(closure(vector<LR0Item>({
		LR0Item(6, 0)
	})), "F"), f); // I0 ->(F) I3

	f.close();
}

void CFG::testConstructCanonicalLR0Collection()
{
	constructCanonicalLR0Collection();
	fstream f("../src/LogLR0Collection");
	f.clear();
	displayCanonicalLR0Collection(f);
	f.close();
}
