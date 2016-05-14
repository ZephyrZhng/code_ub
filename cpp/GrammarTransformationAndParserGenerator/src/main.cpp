#include "CFG.h"
#include "CYK.h"
#include "LL1.h"
#include "SLR.h"
#include "CLR.h"
#include "LALR.h"
#include "Earley.h"
#include "utility.h"

using namespace std;

int main(int argc, char** argv)
{
	CFG g = CFG();

	g.testEliminateUselessSymbols();
	g.testEliminateUnitProductions();
	g.testPutInCNF();


	CYKParser cyk = CYKParser(g);

	cyk.testCYK();

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

	// g.testConstructCanonicalLR1Collection();

	// CLRParser clr = CLRParser(g);

	// clr.testConstructCLRTable();

	// LALRParser lalr = LALRParser(g);

	// lalr.testConstructLALRTable();

	// EarleyParser earley = EarleyParser(g);
	// earley.testEarley();

	// grammar transformation
	// testEliminateUselessSymbols
	// testEliminateUnitProductions
	// testPutInCNF
	// testComputeFirst
	// testComputeFollow
	// testLR0Closure1
	// testLR0Closure
	// testLR0GoTo
	// testConstructCanonicalLR0Collection
	// testConstructCanonicalLR1Collection

	// parser generator
	// testConstructCLRTable
	// testCYK
	// testEarley
	// testConstructLALRTable
	// testConstructLL1Table
	// testParse
	// testConstructSLRTable
	// testParse

	

	return 0;
}
