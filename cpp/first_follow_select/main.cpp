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

int main(int argc, char** arddgv)
{
	Grammar G;
	G.display();

	auto lambda = [](const char& ch){ cout << ch << " "; };

	vector<char> VN = G.getVN();
	for(size_t i = 0; i < VN.size(); ++i)
	{
		cout << "first(" << VN[i] << "): ";
		set<char> firstx;
		G.first(firstx, VN[i]);
		for_each(firstx.begin(), firstx.end(), lambda);
		cout << endl;
	}

	vector<char> VT = G.getVT();
	for(size_t i = 0; i < VT.size(); ++i)
	{
		cout << "first(" << VT[i] << "): ";
		set<char> firstx;
		G.first(firstx, VT[i]);
		for_each(firstx.begin(), firstx.end(), lambda);
		cout << endl;
	}

	for(size_t i = 0; i < VN.size(); ++i)
	{
		cout << "follow(" << VN[i] << "): ";
		set<char> followB;
		G.follow(followB, VN[i]);
		for_each(followB.begin(), followB.end(), lambda);
		cout << endl;
	}

	return 0;
}