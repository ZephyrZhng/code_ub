#ifndef __NUMBER_H
#define __NUMBER_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

using namespace std;

class Number
{
public:
	Number();
	Number(int _r);
	void setLen(int len);
	bool isMax();

	friend void operator ++(Number& n);

	void display();

	vector<int> num;

private:
	int r;
};

#endif /*__NUMBER_H*/