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
	bool accept = true;

	vector<string> w = str;
	w.push_back("$");
	vector<string> stk = {"S", "$"}; // stk.front(): top, stk.back(): bottom
	int ip = 0;
	string x = stk.front();

	while(x != "$")
	{
		string a = w[ip];
		if(x == a)
		{
			stk.erase(stk.begin());
			++ip;
		}
		else if(in(x, g.t))
		{
			accept = false;
			break;
		}
		else if(m[g.getVariableIndex(x)][g.getTerminalIndex(a)] == -1)
		{
			accept = false;
			break;
		}
		else
		{
			Production pr = g.p[m[g.getVariableIndex(x)][g.getTerminalIndex(a)]];
			stk.erase(stk.begin());
			for(size_t i = pr.right.size() - 1; i >= 0; ++i)
			{
				stk.insert(stk.begin(), pr.right[i]);
			}
		}
		x = stk.front();
	}

	return accept;
}