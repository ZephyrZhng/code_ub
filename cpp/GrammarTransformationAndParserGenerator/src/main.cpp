#include "CFG.h"
#include "CLR.h"
#include "CYK.h"
#include "Earley.h"
#include "LALR.h"
#include "LL1.h"
#include "SLR.h"
#include "utility.h"

using namespace std;

int main(int argc, char** argv)
{
	cout
	<< "Test grammar transformation:\n"
	<< "\t<0>Test eliminate useless symbols.\n"
	<< "\t<1>Test eliminate unit productions.\n"
	<< "\t<2>Test put in CNF.\n"
	<< "\t<3>Test compute First and Follow.\n"
	<< "\t<4>Test construct canonical LR(0) collection.\n"
	<< "\t<5>Test construct canonical LR(1) collection.\n"
	<< "Test parser generator:\n"
	<< "\t<6>Test CYK parser.\n"
	<< "\t<7>Test Earley parser.\n"
	<< "\t<8>Test LL(1) parser(construct LL(1) table, parse).\n"
	<< "\t<9>Test SLR parser(construct SLR table, parse).\n"
	<< "\t<10>Test CLR parser(construct CLR table).\n"
	<< "\t<11>Test LALR parser(construct LALR table).\n"
	;

	CFG g = CFG();

	int option = 0;
	cin >> option;
	switch(option)
	{
		case 0: 
		{
			g.testEliminateUselessSymbols();
		}
		break; 

		case 1: 
		{
			g.testEliminateUnitProductions();
		}
		break; 

		case 2: 
		{
			g.testPutInCNF();
		}
		break; 

		case 3: 
		{
			g.testComputeFirst();
			g.testComputeFollow();
		}
		break; 

		case 4: 
		{
			g.testLR0Closure();
			g.testLR0GoTo();
			g.testConstructCanonicalLR0Collection();
		}
		break; 

		case 5: 
		{
			g.testConstructCanonicalLR1Collection();
		}
		break; 

		case 6:
		{
			CYKParser cyk = CYKParser(g);
			cyk.testCYK();
		}
		break;

		case 7: 
		{
			EarleyParser earley = EarleyParser(g);
			earley.testEarley();
		}
		break;

		case 8: 
		{
			g.testComputeFirst();
			g.testComputeFollow();

			LL1Parser ll1 = LL1Parser(g);
			ll1.testConstructLL1Table();
			ll1.testParse();
		}
		break;

		case 9: 
		{
			g.testLR0Closure();
			g.testLR0GoTo();
			g.testConstructCanonicalLR0Collection();

			SLRParser slr = SLRParser(g);
			slr.testConstructSLRTable();
			slr.testParse();
		}
		break;

		case 10: 
		{
			g.testConstructCanonicalLR1Collection();

			CLRParser clr = CLRParser(g);
			clr.testConstructCLRTable();
		}
		break;

		case 11: 
		{
			g.testConstructCanonicalLR1Collection();
			
			LALRParser lalr = LALRParser(g);
			lalr.testConstructLALRTable();
		}
		break;
	}

	return 0;
}
