#ifndef UTILITY_H
#define UTILITY_H

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

using namespace std;

template<typename T>
bool in(const T& e, const vector<T>& v)
{
	return find(v.begin(), v.end(), e) != v.end();
}

#endif /*UTILITY_H*/