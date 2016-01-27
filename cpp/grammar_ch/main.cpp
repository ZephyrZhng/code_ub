#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <vector>

#include "grammar.h"

using namespace std;

template<typename T>
void display(const set<T>& s);

int main(int argc, char** argv)
{
	Grammar g("./grammar.txt");

	// g.display();
	
	// g.calNullableFirstFollow();
	
	cout << g.isNonTerminal('Z') << g.isNonTerminal('A') << g.isNonTerminal('d') << g.isNonTerminal('e') << endl;
	
	ProductionRule pr;
	int idx = g.getProductionRuleByLeft(pr, 'Z');
	if(idx >= 0)
	{
		cout << idx << ": ";
		pr.display();
		cout << endl;
	}
	idx = g.getProductionRuleByLeft(pr, 'B');
	if(idx >= 0)
	{
		cout << idx << ": ";
		pr.display();
		cout << endl;
	}

	display(g.getFirst('Z')); cout << endl;
	display(g.getFirst('A')); cout << endl;
	display(g.getFollow('Z')); cout << endl;
	display(g.getFirst('A')); cout << endl;
	display(g.getFirst("XYZ")); cout << endl;

	return 0;
}