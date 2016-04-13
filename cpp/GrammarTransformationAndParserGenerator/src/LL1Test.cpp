#include "LL1.h"
#include "utility"

void LL1Parser::testConstructLL1Table()
{
	constructLL1Table();
	displayLL1Table();
}

void LL1Parser::testParse()
{
	parse(vector<string>{"0", "+", "0", "*", "0"});
}
