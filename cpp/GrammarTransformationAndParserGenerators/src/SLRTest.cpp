#include "SLR.h"
#include "utility.h"

void SLRParser::testConstructSLRTable()
{
	g.computeFirst();
	g.computeFollow();
	constructSLRTable();
	fstream f("../src/LogSLRTable");
	f.clear();
	displaySLRTable(f);
	displaySLRTable();
	f.close();
}

void SLRParser::testParse()
{
	parse({"0", "*", "0", "+", "0"});
}