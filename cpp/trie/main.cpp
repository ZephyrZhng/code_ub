#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

#include "trie.h"

using namespace std;

int main(int argc, char** argv)
{
	Trie trie;

	// trie.insert("fuckyou");
	// trie.insert("fuckyourmother");
	// trie.insert("fuckher");
	// trie.insert("fuckherfather");
	// trie.insert("shitonyourhead");
	// trie.insert("shitonhisdesk");

	trie.insert("bcc");
	trie.insert("bc");
	trie.insert("bca");
	trie.insert("bcb");
	trie.insert("c");
	trie.insert("cd");
	trie.insert("bcca");
	trie.insert("abcdefg");
	trie.insert("abcddefg");

	trie.preorderTraverse();
	trie.postorderTraverse();

	cout << trie.match('b');
	cout << trie.match('c');
	cout << trie.match('c');
	cout << trie.match('a');
	cout << trie.match('b');
	cout << trie.match('b') << endl;

	trie.display();

	return 0;
}