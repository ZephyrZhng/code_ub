#include "Earley.h"
#include "utility.h"

void EarleyParser::testEarley()
{
	// <P> ::= <S>      # the start rule
	// <S> ::= <S> "+" <M> | <M>
	// <M> ::= <M> "*" <T> | <T>
	// <T> ::= "1" | "2" | "3" | "4"

	g.v = {
		"S",
		"M",
		"T",
	};

	g.t = {
		"+",
		"*",
		"1",
		"2",
		"3",
		"4",
	};

	g.p = {
		Production("S", {"S", "+", "M"}),
		Production("S", {"M"}),
		Production("M", {"M", "*", "T"}),
		Production("M", {"T"}),
		Production("T", {"1"}),
		Production("T", {"2"}),
		Production("T", {"3"}),
		Production("T", {"4"}),
	};

	g.s = "S";

	g.augmentGrammar();
	g.displayGrammar();

	parse({"2", "+", "3", "*", "4"});
	fstream f("../src/LogEarley");
	displayS(f);
	f.close();
}