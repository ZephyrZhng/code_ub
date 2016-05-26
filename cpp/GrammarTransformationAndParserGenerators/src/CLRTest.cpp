#include "CLR.h"
#include "utility.h"

void CLRParser::testConstructCLRTable()
{
	g.computeFirst();
	g.computeFollow();
	constructCLRTable();
	fstream f("../src/LogCLRTable");
	f.clear();
	displayCLRTable(f);
	displayCLRTable();
	f.close();
}