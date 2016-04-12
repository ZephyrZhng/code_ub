#include "CFG.h"
#include "CYK.h"
#include "LL1.h"
#include "SLR.h"
#include "CLR.h"
#include "utility.h"

using namespace std;

int main(int argc, char** argv)
{
	// CYKParser p = CYKParser(CFG());
	// p.parse({"b", "a", "a", "b", "a"});
	// p.displayCYKTable();

	// CYKParser p = CYKParser(CFG());
	// p.parse({"she", "eats", "a", "fish", "with", "a", "fork"});
	// p.displayCYKTable();

	// LL1Parser ll1 = LL1Parser(CFG());
	// ll1.constructLL1Table();
	// ll1.displayLL1Table();
	// ll1.parse(vector<string>{"0", "+", "0", "*", "0"});

	CFG g = CFG();
	g.testLR0Closure();
	// notice the revised version!!!

	// g.constructCanonicalLR0Collection();
	// g.displayCanonicalLR0Collection();

	return 0;
}
