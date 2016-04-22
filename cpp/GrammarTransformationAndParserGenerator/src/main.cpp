#include "CFG.h"
#include "CYK.h"
#include "LL1.h"
#include "SLR.h"
#include "CLR.h"
#include "LALR.h"
#include "utility.h"

using namespace std;

int main(int argc, char** argv)
{
	CFG g = CFG();

	// g.testEliminateUselessSymbols();
	// g.testEliminateUnitProductions();
	// g.testPutInCNF();


	// CYKParser cyk = CYKParser(g);

	// cyk.testCYK();

	// g.testComputeFirst();
	// g.testComputeFollow();

	// LL1Parser ll1 = LL1Parser(g);

	// ll1.testConstructLL1Table();
	// ll1.testParse();

	// g.testLR0Closure();

	// g.testLR0GoTo();
	
	// g.testConstructCanonicalLR0Collection();

	// SLRParser slr = SLRParser(g);

	// slr.testConstructSLRTable();
	// slr.testParse();

	g.testConstructCanonicalLR1Collection();

	// CLRParser clr = CLRParser(g);

	// clr.testConstructCLRTable();

	LALRParser lalr = LALRParser(g);

	lalr.testConstructLALRTable();

	return 0;
}
