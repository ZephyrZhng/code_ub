#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

#include "pda.h"
#include "number.h"

int main(int argc, char** argv)
{
	PDA pda;
	pda.display();

	// Number n(2);
	// for(int i = 0; i < 10; ++i, ++n)
	// {
	// 	cout << i << ": ";
	// 	n.display();
	// 	cout << endl;
	// }

	pda.toCFG();

	return 0;
}