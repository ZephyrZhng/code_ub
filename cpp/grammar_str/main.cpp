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

int main(int argc, char** argv)
{
	Grammar g("./grammar.txt");
	g.display();

	return 0;
}