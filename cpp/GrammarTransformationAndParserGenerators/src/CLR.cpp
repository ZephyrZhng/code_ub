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
	goTo.assign(c.size(), vector<int>(v.size() - 1, -1));

	for(size_t i = 0; i < c.size(); ++i)
	{
		vector<LR1Item> Ii = c[i];
		
		// construct action
		for(size_t j = 0; j < Ii.size(); ++j)
		{
			Production pr = p[Ii[j].productionIndex];
			int productionIndex = Ii[j].productionIndex;			
			string A = pr.left;

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
				if(aIndex == -1)
				{
					aIndex = t.size();
				}
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
		}
		
		// construct goTo
		for(size_t j = 0; j < v.size() - 1; ++j)
		{
			string A = v[j];
			vector<LR1Item> Ij = g.goTo(Ii, A);
			if(goTo[i][j] != -1)
			{
				succeed = false;
			}
			goTo[i][j] = g.getLR1ItemSetIndex(Ij);
		}
	}

	return succeed;
}

bool CLRParser::parse(const vector<string>& str)
{
	bool acc = true;
	fstream f("../src/LogCLR");
	f.clear();

	vector<string> w = str;
	w.push_back("$");
	vector<int> stk = {0};
	auto a = w.begin();

	while(1)
	{
		f << "stk: ";
		display(stk, f);
		f << endl;
		f << "a: " << *a << endl;

		int s = stk.front();
		int aIndex = g.getTerminalIndex(*a);
		if(aIndex == -1)
		{
			aIndex = g.t.size();
		}

		f << "s: " << s << endl;

		if(action[s][aIndex].behavior == shift)
		{
			f << "shift" << endl;
			stk.insert(stk.begin(), action[s][aIndex].index);
			++a;
		}
		else if(action[s][aIndex].behavior == reduce)
		{
			Production pr = g.p[action[s][aIndex].index];

			f << "reduce ";
			pr.display(f);
			f << endl;

			for(size_t i = 0; i < pr.right.size(); ++i)
			{
				stk.erase(stk.begin());
			}

			int t = stk.front();
			stk.insert(stk.begin(), goTo[t][g.getVariableIndex(pr.left)]);
		}
		else if(action[s][aIndex].behavior == accept)
		{
			f << "accept" << endl;
			break;
		}
		else
		{
			f << "error" << endl;
			acc = false;
			break;
		}

		f << endl;
	}

	f.close();

	return acc;
}

void CLRParser::displayCLRTable(ostream& os)
{
	os << "action = {" << endl;
	for(size_t i = 0; i < action.size(); ++i)
	{
		for(size_t j = 0; j < action[i].size(); ++j)
		{
			os << "\t(" << i << ", " << (j == action[i].size() - 1? "$": g.t[j]) << ") ";
			if(action[i][j].behavior == shift)
			{
				os << "shift " << action[i][j].index;
			}
			else if(action[i][j].behavior == reduce)
			{
				os << "reduce ";
				g.p[action[i][j].index].display(os);
			}
			else if(action[i][j].behavior == accept)
			{
				os << "accept";
			}
			else if(action[i][j].behavior == error)
			{
				os << "error";
			}
			os << ", " << endl;
		}
	}
	os << "}" << endl << endl;

	os << "goTo = {" << endl;
	for(size_t i = 0; i < goTo.size(); ++i)
	{
		for(size_t j = 0; j < goTo[i].size(); ++j)
		{
			os << "\t(" << i << ", " << g.v[j] << ") ";
			if(goTo[i][j] != -1)
			{
				os << goTo[i][j];
			}
			else
			{
				os << "error";
			}
			os << "," << endl;
		}
	}
	os << "}" << endl << endl;
}
