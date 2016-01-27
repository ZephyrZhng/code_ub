#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <vector>

using namespace std;

int cnt = 0;

void disp(const vector<int>& a)
{
	for(auto& p: a)
		cout << p << " ";
	cout << endl;
}

int my_partition(vector<int>& a, int p, int r, int x)
{
	swap(*find(a.begin(), a.end(), x), a[p]);

	int i = p;
	int j = r;
	while(i < j)
	{
		while(i < j && a[j] >= x) --j;
		a[i] = a[j];
		while(i < j && a[i] <= x) ++i;
		a[j] = a[i];
	}
	a[i] = x;
	return i;
}

int select(vector<int> a, int p, int r, int k)
{
	++cnt;
	cout << "call select " << cnt << ":" << endl;

	if(r - p + 1 <= 5)
	{
		cout << "recursion exit:" << endl;
		vector<int> my_a(r - p + 1);
		copy(a.begin() + p, a.begin() + r + 1, my_a.begin());
		cout << "the sub-vector whose length is no greater than 5 is:" << endl;
		disp(my_a);
		sort(my_a.begin(), my_a.end());
		cout << "return: " << my_a[p + k - 1] << endl << endl;

		cout << endl << "left select from recursion exit" << cnt << endl;
		return my_a[p + k - 1];
	}

	cout << "find median of medians:" << endl;
	vector<int> medians;
	for(int i = 0; i < (r - p + 1) / 5; ++i)
	{
		vector<int> my5;
		for(int j = 0; j < 5; ++j)
		{
			my5.push_back(a[p + 5 * i + j]);
		}
		sort(my5.begin(), my5.end());
		medians.push_back(my5[2]);
	}
	cout << "median is in:" << endl;
	disp(medians);
	int x = select(medians, 0, medians.size() - 1, (medians.size() - 1) / 2 + 1);
	cout << "median is: " << x << endl << endl;

	vector<int> my_a(a);
	int i = my_partition(my_a, p, r, x);
	int j = i - p + 1;
	cout << "after partition, a becomes:" << endl;
	disp(my_a);
	cout << endl;

	if(k <= j)
	{
		vector<int> l_a(i - p + 1);
		copy(my_a.begin() + p, my_a.begin() + i + 1, l_a.begin());
		cout << "next, find in left part:" << endl;
		disp(l_a);
		cout << endl;

		cout << endl << "left select from left recursion" << cnt << endl;
		return select(l_a, 0, l_a.size() - 1, k);
	}
	else
	{
		vector<int> r_a(r - i);
		copy(my_a.begin() + i + 1, my_a.begin() + r + 1, r_a.begin());
		cout << "next, find in right part:" << endl;
		disp(r_a);
		cout << endl;

		cout << endl << "left select from right recursion" << cnt << endl;
		return select(r_a, 0, r_a.size() - 1, k - j);
	}
}

int main()
{
	vector<int> a = {
		29,22,28,14,45,
		10,44,23, 9,39,
		38,52, 6, 5,50,
		37,11,26, 3,15,
		 2,53,40,54,25,
		42,12,19,30,16,
		18,13, 1,48,41,
		24,43,46,47,17,
		34,20,31,32,33,
		35, 4,49,51, 7,
		36,27, 8,21
	};

	cout << "a is:" << endl;
	disp(a);
	cout << endl;

	cout << "the 15th least element of a is: " << select(a, 0, a.size() - 1, 15) << endl << endl;

	cout << "sorted a is:" << endl;
	sort(a.begin(), a.end());
	disp(a);
	cout << "the 15th least element of a is: " << a[14] << endl;

	return 0;
}