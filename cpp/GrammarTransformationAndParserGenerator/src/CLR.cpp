#include "CLR.h"
#include "utility.h"

CLRActionEntry::CLRActionEntry(const ActionBehavior& b, int i)
{
	behavior = b;
	index = i;
}

CLRParser::CLRParser(const CFG& cfg)
{
	g = cfg;
}

bool CLRParser::constructCLRTable()
{
	bool succeed = true;

	if(!g.augmented)
	{
		g.augmentGrammar();
	}

	vector<string> v = g.v;
	vector<string> t = g.t;
	vector<Production> p = g.p;
	string s = g.s;

	g.constructCanonicalLR1Collection();
	vector<vector<LR1Item>> c = g.canonicalLR1Collection;

	action.assign(c.size(), vector<CLRActionEntry>(t.size() + 1, CLRActionEntry(error, -1)));
	goTo.assign(c.size(), vector<int>(v.size(), -1));

	for(size_t i = 0; i < c.size(); ++i)
	{
		vector<LR1Item> Ii = c[i];
		
		for(size_t j = 0; j < Ii.size(); ++j)
		{
			Production pr = p[Ii[j].productionIndex];
			int productionIndex = Ii[j].productionIndex;			
			string A = pr.left;
			int AIndex = g.getVariableIndex(A);

			// construct action
			if(Ii[j].dotPosition < pr.right.size())
			{
				string a = pr.right[Ii[j].dotPosition];
				if(in(a, t))
				{
					int aIndex = g.getTerminalIndex(a);
					vector<LR1Item> Ij = g.goTo(Ii, a);
					if(action[i][aIndex].behavior != error)
					{
						succeed = false;
					}
					action[i][aIndex] = CLRActionEntry(shift, g.getLR1ItemSetIndex(Ij));
				}
			}
			else if(Ii[j].dotPosition == pr.right.size() && A != s)
			{
				int aIndex = g.getTerminalIndex(Ii[j].lookahead);
				if(action[i][aIndex].behavior != error)
				{
					succeed = false;
				}
				action[i][aIndex] = CLRActionEntry(reduce, productionIndex);
			}
			else if(A == s)
			{
				if(action[i][t.size()].behavior != error)
				{
					succeed = false;
				}
				action[i][t.size()] = CLRActionEntry(accept, -1);
			}

			// construct goTo
			for(size_t i = 0; i < v.size(); ++i)
			{
				vector<LR1Item> Ij = g.goTo(Ii, A);
				if(goTo[i][AIndex] != -1)
				{
					succeed = false;
				}
				goTo[i][AIndex] = g.getLR1ItemSetIndex(Ij);
			}
		}
	}

	return succeed;
}

bool CLRParser::parse(const vector<string>& str)
{
	bool acc = true;

	vector<string> w = str;
	w.push_back("$");
	vector<int> stk = {0};
	auto a = w.begin();

	while(1)
	{
		int s = stk.front();
		int aIndex = g.getTerminalIndex(*a);
		if(aIndex == -1)
		{
			aIndex = g.t.size();
		}

		if(action[s][aIndex].behavior == shift)
		{
			stk.push_back(action[s][aIndex].index);
			++a;
		}
		else if(action[s][aIndex].behavior == reduce)
		{
			Production pr = g.p[action[s][aIndex].index];
			for(size_t i = 0; i < pr.right.size(); ++i)
			{
				stk.erase(stk.begin());
			}

			int t = stk.front();
			stk.push_back(goTo[t][g.getVariableIndex(pr.left)]);

			pr.display();
		}
		else if(action[s][aIndex].behavior == accept)
		{
			break;
		}
		else
		{
			acc = false;
			break;
		}
	}

	return acc;
}