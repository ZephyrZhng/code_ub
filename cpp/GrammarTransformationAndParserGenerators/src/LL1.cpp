#include "LL1.h"
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

	g.computeFollow();

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

void LL1Parser::displayLL1Table(ostream& os)
{
	os << "LL1Table = {" << endl;
	for(size_t i = 0; i < m.size(); ++i)
	{
		for(size_t j = 0; j < m[i].size(); ++j)
		{
			os << "\t(" << g.v[i] << ", " << (j == g.t.size()? "$": g.t[j]) << ") ";
			if(m[i][j] != -1)
			{
				g.p[m[i][j]].display(os);
			}
			else
			{
				os << "error";
			}
			os << "," << endl;
		}
	}
	os << "}" << endl;
}

bool LL1Parser::parse(const vector<string>& str)
{
	fstream f("../src/LogLL1");
	f.clear();

	bool accept = true;

	vector<string> w = str;
	w.push_back("$");
	vector<string> stk = {g.s, "$"}; // stk.front(): top, stk.back(): bottom
	int ip = 0;
	string x = stk.front();

	while(x != "$")
	{
		f << "stk: ";
		display(stk, f);
		f << endl;

		cout << "stk: ";
		display(stk);
		cout << endl;

		string a = w[ip];
		int xIndex = g.getVariableIndex(x);
		int aIndex = g.getTerminalIndex(a);
		if(aIndex == -1)
		{
			aIndex = g.t.size();
		}

		f << "x: " << x << endl << "a: " << a << endl;
		cout << "x: " << x << endl << "a: " << a << endl;

		if(x == a)
		{
			f << "match " << a << endl;
			cout << "match " << a << endl;
			stk.erase(stk.begin());
			++ip;
		}
		else if(in(x, g.t))
		{
			f << "error" << endl;
			cout << "error" << endl;
			accept = false;
			break;
		}
		else if(m[xIndex][aIndex] == -1)
		{
			f << "error" << endl;
			cout << "error" << endl;
			accept = false;
			break;
		}
		else
		{
			Production pr = g.p[m[xIndex][aIndex]];
			pr.display(f);
			f << endl;
			pr.display();
			cout << endl;

			stk.erase(stk.begin());
			if(!(pr.right.size() == 1 && pr.right[0] == ""))
			{
				for(auto it = pr.right.rbegin(); it != pr.right.rend(); ++it)
				{
					stk.insert(stk.begin(), *it);
				}
			}
		}

		f << endl;
		cout << endl;

		x = stk.front();
	}

	f.close();

	return accept;
}