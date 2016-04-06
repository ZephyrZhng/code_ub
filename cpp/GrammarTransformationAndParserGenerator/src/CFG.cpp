#include "CFG.h"

Production::Production(const string& l, const vector<string>& r)
{
	left = l;
	right = r;
}

CFG::CFG()
{
	// // test eliminateUselessSymbols
	// v = {
	// 	"S",
	// 	"A",
	// 	"B",
	// 	"C",
	// };
	// // variables begin with "_" are built-in variables

	// t = {
	// 	"a",
	// 	"b",
	// };

	// p = {
	// 	Production("S", {"A"}),
	// 	Production("S", {"B"}),
	// 	Production("A", {"a", "B"}),
	// 	Production("A", {"b", "S"}),
	// 	Production("A", {"b"}),
	// 	Production("B", {"A", "B"}),
	// 	Production("B", {"B", "a"}),
	// 	Production("C", {"A", "S"}),
	// 	Production("C", {"b"}),
	// };

	// s = "S";

	// // test eliminateUselessSymbols
	// v = {
	// 	"S",
	// 	"A",
	// 	"B",
	// };

	// t = {
	// 	"a",
	// 	"b",
	// };

	// p = {
	// 	Production("S", {"A", "B"}),
	// 	Production("S", {"a"}),
	// 	Production("A", {"a"}),
	// };

	// s = "S";

	// // test eliminateUnitPair
	// v = {
	// 	"A",
	// 	"B",
	// 	"C",
	// 	"D",
	// };

	// t = {
	// 	"a",
	// 	"b",
	// 	"c",
	// };

	// p = {
	// 	Production("A", {"B"}),
	// 	Production("B", {"C"}),
	// 	Production("C", {"D"}),
	// 	Production("B", {"a"}),
	// 	Production("C", {"b"}),
	// 	Production("D", {"c"}),
	// };

	// s = "A";

	// test putInCNF
	v = {
		"E",
		"T",
		"F",
	};

	t = {
		"a",
		"b",
		"+",
		"*",
		"(",
		")",
	};

	p = {
		Production("E", {"E", "+", "T"}),
		Production("E", {"T"}),
		Production("T", {"T", "*", "F"}),
		Production("T", {"F"}),
		Production("F", {"(", "E", ")"}),
		Production("F", {"a"}),
		Production("F", {"b"}),
	};

	s = "E";
}

template<typename T>
bool in(const T& e, const vector<T>& v)
{
	return find(v.begin(), v.end(), e) != v.end();
}

void CFG::findGeneratingSymbols()
{
	generatingSymbols.clear();

	generatingSymbols = t;
	bool updated = false;
	do
	{
		updated = false;
		for(size_t i = 0; i < p.size(); ++i)
		{
			bool isGenerating = true;
			for(size_t j = 0; j < p[i].right.size(); ++j)
			{
				if(!in(p[i].right[j], generatingSymbols))
				{
					isGenerating = false;
					break;
				}
			}
			if(isGenerating && !in(p[i].left, generatingSymbols))
			{
				generatingSymbols.push_back(p[i].left);
				updated = true;
			}
		}
	}while(updated);
}

void CFG::findReachableSymbols()
{
	reachableSymbols.clear();

	reachableSymbols.push_back(s);
	bool updated = false;
	do
	{
		updated = false;
		for(size_t i = 0; i < p.size(); ++i)
		{
			if(in(p[i].left, reachableSymbols))
			{
				for(size_t j = 0; j < p[i].right.size(); ++j)
				{
					if(!in(p[i].right[j], reachableSymbols))
					{
						reachableSymbols.push_back(p[i].right[j]);
						updated = true;
					}
				}
			}
		}
	}while(updated);
}

void CFG::eliminateUselessSymbols()
{
	cout << "------------ eliminateUselessSymbols begin ------------" << endl;

	findGeneratingSymbols();
	displayGeneratingSymbols();
	// generating symbols can be in v
	for(size_t i = 0; i < v.size(); )
	{
		if(!in(v[i], generatingSymbols))
		{
			v.erase(v.begin() + i);
		}
		else
		{
			++i;
		}
	}
	for(size_t i = 0; i < p.size(); )
	{
		bool isGenerating = true;
		if(!in(p[i].left, generatingSymbols))
		{
			isGenerating = false;
		}
		else
		{			
			for(size_t j = 0; j < p[i].right.size(); ++j)
			{
				if(!in(p[i].right[j], generatingSymbols))
				{
					isGenerating = false;
					break;
				}
			}
		}
		if(!isGenerating)
		{
			p.erase(p.begin() + i);
		}
		else
		{
			++i;
		}
	}

	findReachableSymbols();
	displayReachableSymbols();
	// reachable symbols can be in v or t
	for(size_t i = 0; i < v.size(); )
	{
		if(!in(v[i], reachableSymbols))
		{
			v.erase(v.begin() + i);
		}
		else
		{
			++i;
		}
	}
	for(size_t i = 0; i < t.size(); )
	{
		if(!in(t[i], reachableSymbols))
		{
			t.erase(t.begin() + i);
		}
		else
		{
			++i;
		}
	}
	for(size_t i = 0; i < p.size(); )
	{
		bool isReachable = true;
		if(!in(p[i].left, reachableSymbols))
		{
			isReachable = false;
		}
		else
		{
			for(size_t j = 0; j < p[i].right.size(); ++j)
			{
				if(!in(p[i].right[j], reachableSymbols))
				{
					isReachable = false;
					break;
				}
			}
		}
		if(!isReachable)
		{
			p.erase(p.begin() + i);
		}
		else
		{
			++i;
		}
	}

	displayGrammar();

	cout << "------------ eliminateUselessSymbols end ------------" << endl << endl << endl;
}

void CFG::findNullableSymbols()
{
	nullableSymbols.clear();

	for(size_t i = 0; i < p.size(); ++i)
	{
		if(p[i].right.size() == 1 && p[i].right[0] == "")
		{
			nullableSymbols.push_back(p[i].left);
		}
	}

	bool updated = false;
	do
	{
		updated = false;
		for(size_t i = 0; i < p.size(); ++i)
		{
			bool isNullable = true;
			for(size_t j = 0; j < p[i].right.size(); ++j)
			{
				if(!in(p[i].right[j], nullableSymbols))
				{
					isNullable = false;
					break;
				}
			}
			if(isNullable)
			{
				if(!in(p[i].left, nullableSymbols))
				{
					nullableSymbols.push_back(p[i].left);
					updated = true;
				}
			}
		}
	}while(updated);
}

void CFG::eliminateEpsilonProductions()
{
	cout << "------------ eliminateEpsilonProductions begin ------------" << endl;

	findNullableSymbols();
	for(size_t i = 0; i < p.size(); ++i)
	{
		vector<string> ns;
		for(size_t j = 0; j < p[i].right.size(); ++j)
		{
			if(in(p[i].right[j], nullableSymbols))
			{
				ns.push_back(p[i].right[j]);
			}
		}

		vector<bool> flag(ns.size());
		size_t j = 0;
		if(ns.size() == p[i].right.size())
		{
			j = 1;
		}
		for(; j < ns.size(); ++j)
		// j represents number of nullable symbols to be present
		{
			flag.clear();
			fill(flag.begin(), flag.end() - ns.size() + j, true);
			do
			{
				vector<string> presentNullableSymbols;
				for(size_t k = 0; k < ns.size(); ++k)
				{
					if(flag[k])
					{
						presentNullableSymbols.push_back(ns[i]);
					}
				}

				bool exist = false;
				for(size_t k = 0; k < p.size(); ++k)
				{
					if(p[k].left == p[i].left && p[k].right == presentNullableSymbols)
					{
						exist = true;
						break;
					}
				}
				if(!exist)
				{
					p.push_back(Production(p[i].left, presentNullableSymbols));
				}
			}while(prev_permutation(flag.begin(), flag.end()));
		}
	}

	if(in(s, nullableSymbols))
	{
		bool exist = false;
		for(size_t i = 0; i < p.size(); ++i)
		{
			if(p[i].left == s && p[i].right.size() == 1 && p[i].right[0] == "")
			{
				exist = true;
				break;
			}
		}
		if(!exist)
		{
			p.push_back(Production(s, {""}));
		}
	}

	displayGrammar();

	cout << "------------ eliminateEpsilonProductions end ------------" << endl << endl << endl;
}

bool CFG::isUnitProduction(const Production& production)
{
	return production.right.size() == 1 && in(production.right[0], v);
}

void CFG::findUnitPairs()
{
	unitPairs.clear();

	for(size_t i = 0; i < v.size(); ++i)
	{
		unitPairs.push_back(make_pair(v[i], v[i]));
	}

	bool updated = false;
	do
	{
		updated = false;
		for(size_t i = 0; i < unitPairs.size(); ++i)
		{
			for(size_t j = 0; j < p.size(); ++j)
			{
				if(p[j].left == unitPairs[i].second && isUnitProduction(p[j]))
				{
					bool exist = false;
					for(size_t k = 0; k < unitPairs.size(); ++k)
					{
						if(unitPairs[k].first == unitPairs[i].first
							&& unitPairs[k].second == p[j].right[0])
						{
							exist = true;
							break;
						}
					}
					if(!exist)
					{
						unitPairs.push_back(make_pair(unitPairs[i].first, p[j].right[0]));
						updated = true;
					}
				}
			}
		}
	}while(updated);
}

void CFG::eliminateUnitProductions()
{
	cout << "------------ eliminateUnitProductions begin ------------" << endl;

	findUnitPairs();

	vector<Production> nonUnitProductions;
	for(size_t i = 0; i < unitPairs.size(); ++i)
	{
		for(size_t j = 0; j < p.size(); ++j)
		{
			if(p[j].left == unitPairs[i].second && !isUnitProduction(p[j]))
			{
				nonUnitProductions.push_back(Production(unitPairs[i].first, p[j].right));
			}
		}
	}
	p.clear();
	p = nonUnitProductions;

	displayGrammar();

	cout << "------------ eliminateUnitProductions end ------------" << endl << endl << endl;
}

void CFG::putInCNF()
{
	eliminateUselessSymbols();
	eliminateEpsilonProductions();
	eliminateUnitProductions();

	cout << "------------ putInCNF begin ------------" << endl;

	vector<string> terminalsInBody;
	for(size_t i = 0; i < p.size(); ++i)
	{
		if(p[i].right.size() >= 2)
		{
			for(size_t j = 0; j < p[i].right.size(); ++j)
			{
				if(in(p[i].right[j], t) && !in(p[i].right[j], terminalsInBody))
				{
					terminalsInBody.push_back(p[i].right[j]);
				}
			}
		}
	}

	for(size_t i = 0; i < p.size(); ++i)
	{
		if(p[i].right.size() >= 2)
		{
			for(size_t j = 0; j < p[i].right.size(); ++j)
			{
				if(in(p[i].right[j], terminalsInBody))
				{
					p[i].right[j] = "_" + p[i].right[j];
				}
			}
		}
	}

	for(size_t i = 0; i < terminalsInBody.size(); ++i)
	{
		p.push_back(Production("_" + terminalsInBody[i], {terminalsInBody[i]}));
		v.push_back("_" + terminalsInBody[i]);
	}

	int count = 0;
	for(size_t i = 0; i < p.size(); )
	{
		if(p[i].right.size() >= 3)
		{
			for(size_t j = 0; j < p[i].right.size() - 1; ++j)
			{
				if(j == 0)
				{
					v.push_back("_" + to_string(count));
					p.push_back(Production(p[i].left, {p[i].right[j], "_" + to_string(count)}));
				}
				else if(j == p[i].right.size() - 2)
				{
					p.push_back(Production("_" + to_string(count), {p[i].right[j], p[i].right[j + 1]}));
					++count;
				}
				else
				{
					v.push_back("_" + to_string(count + 1));
					p.push_back(Production("_" + to_string(count), {p[i].right[j], "_" + to_string(count + 1)}));
					++count;
				}
			}
			p.erase(p.begin() + i);
		}
		else
		{
			++i;
		}
	}

	displayGrammar();

	cout << "------------ putInCNF end ------------" << endl << endl << endl;
}

void CFG::displayGrammar()
{
	cout << "G = (V, T, P, S)" << endl << endl;

	cout << "V = {" << endl;
	for(size_t i = 0; i < v.size(); ++i)
	{
		cout << "\t" << v[i] << "," << endl;
	}
	cout << "}" << endl << endl;

	cout << "T = {" << endl;
	for(size_t i = 0; i < t.size(); ++i)
	{
		cout << "\t" << t[i] << "," << endl;
	}
	cout << "}" << endl << endl;

	cout << "P = {" << endl;
	for(size_t i = 0; i < p.size(); ++i)
	{
		cout << "\t{ " << p[i].left << " -> ";
		for(size_t j = 0; j < p[i].right.size(); ++j)
		{
			cout << p[i].right[j] << " ";
		}
		cout << "}," << endl;
	}
	cout << "}" << endl << endl;

	cout << "S = " << s << endl << endl;

	cout << endl;
}

void CFG::displayGeneratingSymbols()
{
	cout << "generatingSymbols = {" << endl;
	for(size_t i = 0; i < generatingSymbols.size(); ++i)
	{
		cout << "\t" << generatingSymbols[i] << "," << endl;
	}
	cout << "}" << endl << endl;
}

void CFG::displayReachableSymbols()
{
	cout << "reachableSymbols = {" << endl;
	for(size_t i = 0; i < reachableSymbols.size(); ++i)
	{
		cout << "\t" << reachableSymbols[i] << "," << endl;
	}
	cout << "}" << endl << endl;
}

void CFG::displayNullableSymbols()
{
	cout << "nullableSymbols = {" << endl;
	for(size_t i = 0; i < nullableSymbols.size(); ++i)
	{
		cout << "\t" << nullableSymbols[i] << "," << endl;
	}
	cout << "}" << endl << endl;
}

void CFG::displayUnitPairs()
{
	cout << "unitPairs = {" << endl;
	for(size_t i = 0; i < unitPairs.size(); ++i)
	{
		cout << "\t(" << unitPairs[i].first << ", " << unitPairs[i].second << ")," << endl;
	}
	cout << "}" << endl << endl;
}

string CFG::getS()
{
	return s;
}

vector<string> CFG::getV()
{
	return v;
}
