#include "LL1Parser.h"
#include "utility.h"

LL1Parser::LL1Parser(const CFG& cfg)
{
	g = cfg;
}

bool LL1Parser::setM(int row, int column, int index)
{
	int succeed = false;
	if(m[row][column] == -1)
	{
		m[row][column] = index;
		succeed = true;
	}
	return succeed;
}

bool LL1Parser::constructLL1Table()
{
	m.clear();

	vector<string> v = g.v;
	vector<string> t = g.t;
	vector<Production> p = g.p;
	bool succeed = true;

	m.assign(v.size(), vector<int>(t.size() + 1, -1));
	// row: variables, column: terminals + $
	// elements: production indices

	for(size_t i = 0; i < p.size(); ++i)
	{
		// A -> alpha
		string A = p[i].left;
		int AIndex = g.getVariableIndex(A);
		vector<string> firstAlpha = g.computeFirst(p[i].right);

		for(size_t j = 0; j < firstAlpha.size(); ++j)
		{
			string a = firstAlpha[j];
			if(a != "")
			{
				if(!setM(AIndex, g.getTerminalIndex(a), i))
				{
					succeed = false;
				}
			}
		}

		if(in(string(""), firstAlpha))
		{
			vector<string> followA = g.follow[g.getFollowIndex(A)].second;
			for(size_t j = 0; j < followA.size(); ++j)
			{
				if(followA[j] != "$")
				{
					if(!setM(AIndex, g.getTerminalIndex(followA[j]), i))
					{
						succeed = false;
					}
				}
				else
				{
					if(!setM(AIndex, t.size(), i))
					{
						succeed = false;
					}
				}
			}
		}
	}

	return succeed;
}

bool LL1Parser::parse(const vector<string>& str)
{

}