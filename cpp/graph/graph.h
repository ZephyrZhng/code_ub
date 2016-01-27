#ifndef __GRAPH_H
#define __GRAPH_H

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

class Graph
{
public:
	Graph(string filePath);

	void display();

	vector<vector<int>> g;
	int row;
	int col;

private:
	
};

#endif /*__GRAPH_H*/