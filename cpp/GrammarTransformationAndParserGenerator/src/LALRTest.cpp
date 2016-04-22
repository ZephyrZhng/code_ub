#include "LALR.h"
#include "utility.h"

void LALRParser::testConstructLALRTable()
{
	g.computeFirst();
	g.computeFollow();
	constructLALRTable();
	fstream f("../src/LogLALRTable");
	f.clear();
	displayLALRTable(f);
	f.close();
}
