#ifndef TRIE_H
#define TRIE_H

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

class Node
{
public:
	char val;
	vector<Node*> sons;
	bool terminal;

	Node();
	Node(const char& ch);

private:

};

class Trie
{
public:
	Trie();
	~Trie();
	void destruct(Node* p);
	void insert(const string& str);
	void preorder_traverse(Node* p);
	void postorder_traverse(Node* p);
	void preorderTraverse();
	void postorderTraverse();
	void resetCurPtr();
	void preorder_traverse_display(vector<int>& stk, Node* p, bool b1);
	void display();

	bool match(char ch);

	Node* curPtr;
	Node* root;

private:

};

#endif /*TRIE_H*/