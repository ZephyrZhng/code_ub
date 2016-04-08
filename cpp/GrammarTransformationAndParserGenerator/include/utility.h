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

template<typename T>
bool add(vector<T>& v1, const vector<T>& v2)
{
	bool ret = false;
	for(size_t i = 0; i < v2.size(); ++i)
	{
		if(!in(v2[i], v1))
		{
			v1.push_back(v2[i]);
			ret = true;
		}
	}
	return ret;
}

template<typename T>
bool del(vector<T>& v, const T& e)
{
	bool ret = false;
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(v[i] == e)
		{
			v.erase(v.begin() + i);
			ret = true;
		}
		else
		{
			++i;
		}
	}
	return ret;
}

#endif /*UTILITY_H*/