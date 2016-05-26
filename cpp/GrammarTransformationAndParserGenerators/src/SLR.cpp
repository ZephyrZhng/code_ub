#include "SLR.h"
#include "utility.h"

SLRActionEntry::SLRActionEntry(const ActionBehavior& b, int i)
{
	behavior = b;
	index = i;
}

SLRParser::SLRParser(const CFG& cfg)
{
	g = cfg;
}

bool SLRParser::constructSLRTable()
{
	g.computeFollow();
	
	bool succeed = true;

	if(!g.augmented)
	{
		g.augmentGrammar();
	}

	vector<string> v = g.v;
	vector<string> t = g.t;
	vector<Production> p = g.p;
	string s = g.s;

	g.constructCanonicalLR0Collection();
	vector<vector<LR0Item>> c = g.canonicalLR0Collection;

	action.assign(c.size(), vector<SLRActionEntry>(t.size() + 1, SLRActionEntry(error, -1)));
	goTo.assign(c.size(), vector<int>(v.size() - 1, -1));

	for(size_t i = 0; i < c.size(); ++i)
	{
		vector<LR0Item> Ii = c[i];
		
		// construct action
		for(size_t j = 0; j < Ii.size(); ++j)
		{
			Production pr = p[Ii[j].productionIndex];
			int productionIndex = Ii[j].productionIndex;			
			string A = pr.left;
			// int AIndex = g.getVariableIndex(A);

			if(Ii[j].dotPosition < pr.right.size())
			{
				string a = pr.right[Ii[j].dotPosition];
				if(in(a, t))
				{
					int aIndex = g.getTerminalIndex(a);
					vector<LR0Item> Ij = g.goTo(Ii, a);
					if(action[i][aIndex].behavior != error)
					{
						succeed = false;
					}
					action[i][aIndex] = SLRActionEntry(shift, g.getLR0ItemSetIndex(Ij));
				}
			}
			else if(Ii[j].dotPosition == pr.right.size() && A != s)
			{
				pair<string, vector<string>> followA = g.follow[g.getFollowIndex(A)];
				for(size_t k = 0; k < followA.second.size(); ++k)
				{
					int aIndex = g.getTerminalIndex(followA.second[k]);
					if(aIndex == -1)
					{
						aIndex = t.size();
					}
					if(action[i][aIndex].behavior != error)
					{
						succeed = false;
					}
					action[i][aIndex] = SLRActionEntry(reduce, productionIndex);
				}
			}
			else if(A == s)
			{
				if(action[i][t.size()].behavior != error)
				{
					succeed = false;
				}
				action[i][t.size()] = SLRActionEntry(accept, -1);
			}
		}

		// construct goTo
		for(size_t j = 0; j < v.size() - 1; ++j)
		// notice that augmented grammar has one more element in v, "_" + s
		{
			string A = v[j];
			vector<LR0Item> Ij = g.goTo(Ii, A);
			if(goTo[i][j] != -1)
			{
				succeed = false;
			}
			goTo[i][j] = g.getLR0ItemSetIndex(Ij);
		}
	}

	return succeed;
}

bool SLRParser::parse(const vector<string>& str)
{
	bool acc = true;
	fstream f("../src/LogSLR");
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

		cout << "stk: ";
		display(stk);
		cout << endl;
		cout << "a: " << *a << endl;

		int s = stk.front();
		int aIndex = g.getTerminalIndex(*a);
		if(aIndex == -1)
		{
			aIndex = g.t.size();
		}

		f << "s: " << s << endl;
		cout << "s: " << s << endl;

		if(action[s][aIndex].behavior == shift)
		{
			f << "shift" << endl;
			cout << "shift" << endl;
			stk.insert(stk.begin(), action[s][aIndex].index);
			++a;
		}
		else if(action[s][aIndex].behavior == reduce)
		{
			Production pr = g.p[action[s][aIndex].index];

			f << "reduce ";
			pr.display(f);
			f << endl;

			cout << "reduce ";
			pr.display();
			cout << endl;

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
			cout << "accept" << endl;
			break;
		}
		else
		{
			f << "error" << endl;
			cout << "error" << endl;
			acc = false;
			break;
		}

		f << endl;
		cout << endl;
	}

	f.close();

	return acc;
}

void SLRParser::displaySLRTable(ostream& os)
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
