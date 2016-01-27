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

template<typename T>
void display(const vector<T>& v){
	for_each(v.begin(), v.end(),
		[](T t){
			cout << t << " ";
		});
	cout << endl;
}

int main(int argc, char** argv){
	string plaintext, ciphertext;
	int column;
	cout << "Input plaintext: ";
	cin >> plaintext;
	// plaintext = "datasecurity";
	cout << "Input column number: ";
	cin >> column;
	vector<int> permutation(column);
	cout << "Input a permutation of { 1, ... , " << column << " }: ";
	for(size_t i = 0; i < column; ++i){
		cin >> permutation[i];
	}
	int dist = 26;
	int row = plaintext.size() / column;
	if(row * column != plaintext.size()){
		++row;
	}
	int n = row * column;
	for(size_t i = 0; i < column; ++i){
		int m = 26, idx;
		for(size_t j = 0; j < permutation.size(); ++j){
			if(permutation[j] >= 0 && permutation[j] < m){
				idx = j;
				m = permutation[j];
			}
		}
		permutation[idx] = -1;
		for(auto p = plaintext.begin() + idx;;){
			int d = p - plaintext.begin();
			if(d < plaintext.size()){
				ciphertext.push_back(*p);
			}else if(d < n){
				ciphertext.push_back('z');
			}
			if(d < n){
				p += column;
			}else{
				break;
			}
		}
	}
	cout << "Ciphertext is: " << ciphertext << endl;
	return 0;
}