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

#include "pda.h"

using namespace std;

int main(int argc, char** argv)
{
	PDA pda("./pda.txt");
	pda.display();

	return 0;
}