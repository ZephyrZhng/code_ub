#include "LL1.h"
#include "utility"

void LL1Parser::testConstructLL1Table()
{
	constructLL1Table();
	fstream f("../src/LogLL1Table");
	displayLL1Table(f);
	displayLL1Table();
	f.close();
}

void LL1Parser::testParse()
{
	parse(vector<string>{"0", "+", "0", "*", "0"});
}
