#include "graph.h"

Graph::Graph(string filePath)
{
	ifstream f(filePath);
	assert(f);

	// file format
	// row col
	// g(w)

	f >> row >> col;
	g.resize(row);
	for(int i = 0; i < row; ++i)
	{
		g[i].resize(col);
		for(int j = 0; j < col; ++j)
		{
			f >> g[i][j];
		}
	}

	f.close();
}

void Graph::display()
{
	cout << "row: " << row << endl << "col: " << col << endl;
	for_each(g.begin(), g.end(),
		[](const vector<int>& vec)
		{
			for_each(vec.begin(), vec.end(),
				[](const int& i)
				{
					cout << i << " ";
				});
			cout << endl;
		});
}