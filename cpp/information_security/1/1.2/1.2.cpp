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
	string plaintext, ciphertext, key;
	cout << "Input plaintext: ";
	cin >> plaintext;
	cout << "Input key: ";
	cin >> key;
	// plaintext = "thenormaldeisiontablerepresentationhasfourseparatepartsinaspecificformat";
	// key = "computer";
	int dist = 26;
	vector<int> v(key.size());
	for(size_t i = 0; i < v.size(); ++i){
		v[i] = key[i] - 'a';
	}
	// display(v);
	int r = plaintext.size() / key.size();
	if(r * key.size() != plaintext.size()){
		++r;
	}
	int n = r * key.size();
	for(size_t i = 0; i < key.size(); ++i){
		int m = 26, idx;
		for(size_t j = 0; j < v.size(); ++j){
			if(v[j] >= 0 && v[j] < m){
				idx = j;
				m = v[j];
			}
		}
		v[idx] = -1;
		// cout << idx << " ";
		// display(v);
		for(auto p = plaintext.begin() + idx;;){
			int d = p - plaintext.begin();
			if(d < plaintext.size()){
				ciphertext.push_back(*p);
				// cout << *p;
			}else if(d < n){
				ciphertext.push_back('z');
				// cout << "z";
			}
			if(d < n){
				p += key.size();
			}else{
				break;
			}
		}
		// cout << endl;
	}
	cout << "Ciphertext is: " << ciphertext << endl;
	return 0;
}