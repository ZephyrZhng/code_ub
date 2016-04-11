#ifndef SLR_H
#define SLR_H

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "CFG.h"

using namespace std;

class SLRParser
{
public:
	SLRParser() = default;
	~SLRParser() = default;
	SLRParser(const CFG& cfg);

	bool parse(const vector<string>& str);

private:
	CFG g;

};

#endif /*SLR_H*/