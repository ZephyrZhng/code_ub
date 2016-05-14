#include "CYK.h"
#include "utility"

void CYKParser::testCYK()
{
	// test CYK
	g.v = {
		"S",
		"A",
		"B",
		"C",
	};

	g.t = {
		"a",
		"b",
	};

	g.p = {
		Production("S", {"A", "B"}),
		Production("S", {"B", "C"}),
		Production("A", {"B", "A"}),
		Production("A", {"a"}),
		Production("B", {"C", "C"}),
		Production("B", {"b"}),
		Production("C", {"A", "B"}),
		Production("C", {"a"}),
	};

	g.s = "S";

	g.displayGrammar();
	parse({"b", "a", "a", "b", "a"});
	fstream f("../src/LogCYK");
	f.clear();
	displayCYKTable(f);

	// test CYK
	g.v = {
		"S",
		"NP",
		"VP",
		"PP",
		"V",
		"P",
		"Det",
		"N",
	};

	g.t = {
		"eats",
		"she",
		"with",
		"fish",
		"fork",
		"a",
	};

	g.p = {
		Production("S", {"NP", "VP"}),
		Production("VP", {"VP", "PP"}),
		Production("VP", {"V", "NP"}),
		Production("VP", {"eats"}),
		Production("PP", {"P", "NP"}),
		Production("NP", {"Det", "N"}),
		Production("NP", {"she"}),
		Production("V", {"eats"}),
		Production("P", {"with"}),
		Production("N", {"fish"}),
		Production("N", {"fork"}),
		Production("Det", {"a"}),
	};

	g.s = "S";

	g.displayGrammar();
	parse({"she", "eats", "a", "fish", "with", "a", "fork"});
	displayCYKTable(f);
	f.close();
}