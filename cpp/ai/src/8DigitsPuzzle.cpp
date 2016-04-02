#include "8DigitsPuzzle.h"

Node::Node(int d, const vector<int>& g)
{
	parent = -1;
	depth = 0;
	lastMove = 0;
	grid = g;
	calWeight();
}

Node::Node(const Node& n)
{
	parent = n.parent;
	depth = n.depth;
	weight = n.weight;
	lastMove = n.lastMove;
	grid = n.grid;
}

void Node::calWeight()
{
	weight = depth;
	if(grid[0] != 1) ++weight;
	if(grid[1] != 2) ++weight;
	if(grid[2] != 3) ++weight;
	if(grid[3] != 8) ++weight;
	// if(grid[4] != 0) ++weight; // 0 represents blank
	if(grid[5] != 4) ++weight;
	if(grid[6] != 7) ++weight;
	if(grid[7] != 6) ++weight;
	if(grid[8] != 5) ++weight;
}

Puzzle::Puzzle(const vector<int>& r)
{
	root = r;
}

void Puzzle::addToOpen(const Node& n)
{
	for(size_t i = 0; i < open.size(); ++i)
	{
		if(open[i].weight >= n.weight)
		{
			open.insert(open.begin() + i, n);
			return;
		}
	}
	open.push_back(n);
}

void Puzzle::expand(int d, Node n, int bi, int m, int lm)
{
	n.grid[bi] = n.grid[m];
	n.grid[m] = 0;
	n.calWeight();
	n.lastMove = lm;
	addToOpen(n);
}

void Puzzle::s(int d, fstream& f)
{
	Node currNode(open.front());
	open.erase(open.begin());

	closed.push_back(currNode);
	currNode.parent = closed.size() - 1;

	if(currNode.grid[0] == 1 && currNode.grid[1] == 2 && currNode.grid[2] == 3
		&& currNode.grid[3] == 8 && currNode.grid[4] == 0 && currNode.grid[5] == 4
		&& currNode.grid[6] == 7 && currNode.grid[7] == 6 && currNode.grid[8] == 5)
		return;

	f << "---------------------- depth = " << d << " ----------------------" << endl;

	int blankIndex = find(currNode.grid.begin(), currNode.grid.end(), 0) - currNode.grid.begin();
	f << "blankIndex: " << blankIndex << endl;
	f << "currNode.lastMove: " << currNode.lastMove << endl;
	f << "currNode.parent: " << currNode.parent << endl;
	f << "currNode.depth: " << currNode.depth << endl;
	f << "--------------------" << endl;
	++currNode.depth;

	// 1: up, 2: right, 3: down, 4: left, 0: iniNode
	if(blankIndex == 0) // blank is currently at (0,0)
	{
		if(currNode.lastMove == 2) // blank comes from right, move blank down
		{
			expand(d, currNode, blankIndex, 3, 1);
		}
		else if(currNode.lastMove == 3) // blank comes from down, move blank right
		{
			expand(d, currNode, blankIndex, 1, 4);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 3, 1);
			expand(d, currNode, blankIndex, 1, 4);
		}
	}
	else if(blankIndex == 1) // blank is currently at (0,1)
	{
		if(currNode.lastMove == 4) // blank comes from left, move blank right or down
		{
			expand(d, currNode, blankIndex, 2, 4);
			expand(d, currNode, blankIndex, 4, 1);
		}
		else if(currNode.lastMove == 2) // blank comes from right, move blank left or down
		{
			expand(d, currNode, blankIndex, 0, 2);
			expand(d, currNode, blankIndex, 4, 1);
		}
		else if(currNode.lastMove == 3) // blank comes from down, move blank left or right
		{
			expand(d, currNode, blankIndex, 0, 2);
			expand(d, currNode, blankIndex, 2, 4);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 0, 2);
			expand(d, currNode, blankIndex, 2, 4);
			expand(d, currNode, blankIndex, 4, 1);
		}
	}
	else if(blankIndex == 2) // blank is currently at (0,2)
	{
		if(currNode.lastMove == 4) // blank comes from left, move down
		{
			expand(d, currNode, blankIndex, 5, 1);
		}
		else if(currNode.lastMove == 3) // blank comes from down, move left
		{
			expand(d, currNode, blankIndex, 1, 2);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 1, 2);
			expand(d, currNode, blankIndex, 5, 1);
		}
	}
	else if(blankIndex == 3) // blank is currently at (1,0)
	{
		if(currNode.lastMove == 1) // blank comes from up, move right or down
		{
			expand(d, currNode, blankIndex, 4, 4);
			expand(d, currNode, blankIndex, 6, 1);
		}
		else if(currNode.lastMove == 2) // blank comes from right, move up or down
		{
			expand(d, currNode, blankIndex, 0, 3);
			expand(d, currNode, blankIndex, 6, 1);
		}
		else if(currNode.lastMove == 3) // blank comes from down, move up or right
		{
			expand(d, currNode, blankIndex, 0, 3);
			expand(d, currNode, blankIndex, 4, 4);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 0, 3);
			expand(d, currNode, blankIndex, 6, 1);
			expand(d, currNode, blankIndex, 4, 4);
		}
	}
	else if(blankIndex == 4) // blank is currently at (1,1)
	{
		if(currNode.lastMove == 1) // blank comes from up, move right, down or left
		{
			expand(d, currNode, blankIndex, 5, 4);
			expand(d, currNode, blankIndex, 7, 1);
			expand(d, currNode, blankIndex, 3, 2);
		}
		else if(currNode.lastMove == 2) // blank comes from right, move up, down or left
		{
			expand(d, currNode, blankIndex, 1, 3);
			expand(d, currNode, blankIndex, 7, 1);
			expand(d, currNode, blankIndex, 3, 2);
		}
		else if(currNode.lastMove == 3) // blank comes from down, move up, right or left
		{
			expand(d, currNode, blankIndex, 1, 3);
			expand(d, currNode, blankIndex, 5, 4);
			expand(d, currNode, blankIndex, 3, 2);
		}
		else if(currNode.lastMove == 4) // blank comes from left, move up, right or down
		{
			expand(d, currNode, blankIndex, 1, 3);
			expand(d, currNode, blankIndex, 5, 4);
			expand(d, currNode, blankIndex, 7, 1);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 1, 3);
			expand(d, currNode, blankIndex, 3, 2);
			expand(d, currNode, blankIndex, 5, 4);
			expand(d, currNode, blankIndex, 7, 1);
		}
	}
	else if(blankIndex == 5) // blank is currently at (1,2)
	{
		if(currNode.lastMove == 1) // blank comes form up, move left or down
		{
			expand(d, currNode, blankIndex, 4, 2);
			expand(d, currNode, blankIndex, 6, 1);
		}
		else if(currNode.lastMove == 4) // blank comes from left, move up or down
		{
			expand(d, currNode, blankIndex, 2, 3);
			expand(d, currNode, blankIndex, 6, 1);
		}
		else if(currNode.lastMove == 3) // blank comes from down, move up or left
		{
			expand(d, currNode, blankIndex, 2, 3);
			expand(d, currNode, blankIndex, 4, 2);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 2, 3);
			expand(d, currNode, blankIndex, 4, 2);
			expand(d, currNode, blankIndex, 6, 1);
		}
	}
	else if(blankIndex == 6) // blank is currently at (2,0)
	{
		if(currNode.lastMove == 1) // blank comes from up, move right
		{
			expand(d, currNode, blankIndex, 7, 4);
		}
		else if(currNode.lastMove == 2) // blank comes from right, move up
		{
			expand(d, currNode, blankIndex, 3, 3);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 3, 3);
			expand(d, currNode, blankIndex, 7, 4);
		}
	}
	else if(blankIndex == 7) // blank is currently at (2,1)
	{
		if(currNode.lastMove == 1) // blank comes from up, move left or right
		{
			expand(d, currNode, blankIndex, 6, 2);
			expand(d, currNode, blankIndex, 8, 4);
		}
		else if(currNode.lastMove == 2) // blank comes from right, move up or left
		{
			expand(d, currNode, blankIndex, 4, 3);
			expand(d, currNode, blankIndex, 6, 2);
		}
		else if(currNode.lastMove == 4) // blank comes from left, move up or right
		{
			expand(d, currNode, blankIndex, 4, 3);
			expand(d, currNode, blankIndex, 8, 4);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 4, 3);
			expand(d, currNode, blankIndex, 6, 2);
			expand(d, currNode, blankIndex, 8, 4);
		}
	}
	else if(blankIndex == 8) // blank is currently at (2,2)
	{
		if(currNode.lastMove == 1)
		{
			expand(d, currNode, blankIndex, 7, 2);
		}
		else if(currNode.lastMove == 4)
		{
			expand(d, currNode, blankIndex, 5, 3);
		}
		else if(currNode.lastMove == 0)
		{
			expand(d, currNode, blankIndex, 5, 3);
			expand(d, currNode, blankIndex, 7, 2);
		}
	}

	
	for(size_t i = 0; i < open.size(); ++i)
	{
		f << "open[" << i << "] ";
		for(size_t j = 0; j < open[i].grid.size(); ++j)
		{
			f << open[i].grid[j];
		}
		f << " weight: " << open[i].weight;
		f << " lastMove: " << open[i].lastMove;
		f << " depth: " << open[i].depth;
		f << " parent: " << open[i].parent;
		f << endl;
	}
	for(size_t i = 0; i < closed.size(); ++i)
	{
		f << "closed[" << i << "] ";
		for(size_t j = 0; j < closed[i].grid.size(); ++j)
		{
			f << closed[i].grid[j];
		}
		f << " parent: " << closed[i].parent;
		f << endl;
	}
	f << endl;

	s(open.front().depth, f);
}

void Puzzle::solve()
{
	Node r = Node(0, root);
	open.push_back(r);

	fstream f("../src/8DigitsPuzzleLog");
	s(0, f);	

	vector<Node> solution;
	for(size_t i  = closed.size() - 1; ; i = closed[i].parent)
	{
		solution.insert(solution.begin(), closed[i]);
		if(closed[i].parent == -1) break;
	}

	for(size_t i = 0; i < solution.size(); ++i)
	{
		f << "[" << i << "] ";
		for(size_t j = 0; j < solution[i].grid.size(); ++j)
		{
			f << solution[i].grid[j];
		}
		f << endl;
	}

	f.close();
}