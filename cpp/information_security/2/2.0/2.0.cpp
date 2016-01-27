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

using namespace std;

int extendedEuclid(int a, int b, int& x, int& y){
	if(!b){
	   x = 1;
	   y = 0;
	   return a;
	}
	int r = extendedEuclid(b, a % b, x, y);
	int t = x;
	x = y;
	y = t - a / b * y;
	return r;
}

int main(int argc, char** argv){
	int a, b, x, y;
	cout << "Input a: ";
	cin >> a;
	cout << "Input b: ";
	cin >> b;
	extendedEuclid(a, b, x, y);
	cout << "( a * " << x << " ) mod b = 1" << endl;
	return 0;
}
