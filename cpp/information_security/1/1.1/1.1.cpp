#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <numeric>
#include <limits>
#include <iterator>
#include <algorithm>

using namespace std;

int main(int argc, char** argv){
	cout << "Input plaintext: ";
	string plaintext, ciphertext;
	cin >> plaintext;
	int t = floor((plaintext.size() - 1) / 2);
	for(unsigned int i = 0; i < plaintext.size(); ++i){
		if(i <= t){
			ciphertext.push_back(plaintext[2 * i]);
		}else{
			ciphertext.push_back(plaintext[2 * (i - t) - 1]);
		}
	}
	cout << "Ciphertext is: " << ciphertext << endl;
	return 0;
}