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

int main(int argc, char** argv){
	int d;
	cout << "Input period: ";
	cin >> d;
	vector<int> f(d);
	cout << "Input a permutation of { 1, ... , " << d << " }: ";
	for(int i = 0; i < d; ++i){
		cin >> f[i];
	}
	string m, c;
	cout << "Input plaintext: ";
	cin >> m;
	c.resize(m.size());
	for(size_t i = 0; i < m.size(); ++i){
		c.push_back(m[f[i % d] + floor(i / d) * d - 1]);
	}
	cout << "Ciphertext is: " << c << endl;
	return 0;
}