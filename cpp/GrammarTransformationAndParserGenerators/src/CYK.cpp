#include "CYK.h"
#include "utility.h"

CYKParser::CYKParser(const CFG& cfg)
{
	g = cfg;
}

bool CYKParser::parse(const vector<string>& str)
{
	m.clear();

	g.putInCNF();

	vector<string> v = g.v;
	vector<Production> p = g.p;
	string s = g.s;

	n = str.size();
	r = v.size();
	m.assign(n, vector<vector<bool>>(n, vector<bool>(r, false)));

	for(int i = 0; i <= n - 1; ++i)
	{
		for(size_t j = 0; j < p.size(); ++j)
		{
			if(p[j].right.size() == 1 && p[j].right[0] == str[i])
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
					if(p[l].right.size() == 2
						&& in(p[l].right[0], v)
						&& in(p[l].right[1], v))
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

	bool accept;
	if(m[n - 1][0][g.getVariableIndex(s)])
	{
		accept = true;
	}
	else
	{
		accept = false;
	}
	return accept;
}

void CYKParser::displayCYKTable(ostream& os)
{
	t.clear();

	// m[n][n][r]
	// R_k in t[i][j] <==> m[i][j][k]
	t.assign(n, vector<vector<string>>(n, vector<string>()));
	for(int i = 0; i <= n - 1; ++i)
	{
		for(int j = 0; j <= n - i - 1; ++j)
		{
			for(int k = 0; k <= r - 1; ++k)
			{
				if(m[i][j][k])
				{
					t[i][j].push_back(g.v[k]);
				}
			}
		}
	}

	os << "CYKTable = {" << endl;
	for(int i = 0; i <= n - 1; ++i)
	{
		for(int j = 0; j <= n - i - 1; ++j)
		{
			os << "\t(" << i << ", " << j << ") { ";
			for(size_t k = 0; k < t[i][j].size(); ++k)
			{
				os << t[i][j][k] << ", ";
			}
			os << "}," << endl;
		}
	}
	os << "}" << endl << endl;
}