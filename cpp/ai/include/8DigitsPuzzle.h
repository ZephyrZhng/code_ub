#ifndef AI_H
#define AI_H

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class Node
{
public:
	Node() = default;
	~Node() = default;
	Node(int d, const vector<int>& g);
	Node(const Node& n);

	void calWeight();

	int parent; // index in closed, -1 means no parent
	int depth;
	int weight; // depth + #grid with wrong number
	int lastMove; // 1: up, 2: right, 3: down, 4: left, 0: no last move
	vector<int> grid; // (0,0),(0,1),(0,2),(1,0),(1,1),(1,2),(2,0),(2,1),(2,2)

private:

};

class Puzzle
{
public:
	Puzzle() = default;
	~Puzzle() = default;
	Puzzle(const vector<int>& r);

	void addToOpen(const Node& n);
	void expand(int d, Node n, int bi, int m, int lm);
	void s(int d, fstream& f);
	void solve();

private:
	vector<int> root;
	vector<Node> open;
	vector<Node> closed;
};

#endif /*AI_H*/