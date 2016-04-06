#include "CYK.h"
#include "utility.h"

bool CYKParser::parse(const vector<string>& str)
{
	g.putInCNF();

	vector<string> v = g.v;
	vector<Production> p = g.p;
	string s = g.s;
	int n = str.size();
	int r = v.size();
	vector<vector<vector<bool>>> m(n, vector<vector<bool>>(n, vector<bool>(r, false)));

	for(int i = 0; i <= n - 1; ++i)
	{
		for(size_t j = 0; j < p.size(); ++j)
		{
			if(p[j].right.size() == 1 && in(p[j].right[0], g.t))
			{
				m[0][i][g.getVariableIndex(p[j].left)] = true;
			}
		}
	}

	for(int i = 1; i <= n - 1; ++i)
	{
		for(int j = 0; j <= n - i - 1; ++j)
		{
			for(int k = 0; k <= i - 1; ++k)
			{
				for(size_t l = 0; l < p.size(); ++l)
				{
					if(g.isUnitProduction(p[l]))
					{
						if(m[k][j][g.getVariableIndex(p[l].right[0])]
							&& m[i - k - 1][j + k + 1][g.getVariableIndex(p[l].right[1])])
						{
							m[i][j][g.getVariableIndex(p[l].left)] = true;
						}
					}
				}
			}
		}
	}

	bool ret;
	if(m[n - 1][0][g.getVariableIndex(s)])
	{
		ret = true; 
	}
	else
	{
		ret = false;
	}
	return ret;
}