#include "CFG.h"
#include "utility.h"

Production::Production(const string& l, const vector<string>& r)
{
	left = l;
	right = r;
}

void Production::display(ostream& os)
{
	os << left << " -> ";
	for(size_t j = 0; j < right.size(); ++j)
	{
		os << right[j] << " ";
	}
}

LR0Item::LR0Item(int pi, int dp)
{
	productionIndex = pi;
	dotPosition = dp;
}

bool operator ==(const LR0Item& l, const LR0Item& r)
{
	return l.productionIndex == r.productionIndex && l.dotPosition == r.dotPosition;
}

LR1Item::LR1Item(unsigned int pi, unsigned int dp, const string& la)
{
	productionIndex = pi;
	dotPosition = dp;
	lookahead = la;
}

bool operator ==(const LR1Item& l, const LR1Item& r)
{
	return l.productionIndex == r.productionIndex
		&& l.dotPosition == r.dotPosition
		&& l.lookahead == r.lookahead;
}

CFG::CFG()
{
	augmented = false;
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
	findGeneratingSymbols();

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
}

void CFG::putInCNF()
{
	eliminateUselessSymbols();
	eliminateEpsilonProductions();
	eliminateUnitProductions();

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
}

void CFG::displayGrammar(ostream& os)
{
	os << "G = (V, T, P, S)" << endl << endl;

	os << "V = {" << endl;
	for(size_t i = 0; i < v.size(); ++i)
	{
		os << "\t" << v[i] << "," << endl;
	}
	os << "}" << endl << endl;

	os << "T = {" << endl;
	for(size_t i = 0; i < t.size(); ++i)
	{
		os << "\t" << t[i] << "," << endl;
	}
	os << "}" << endl << endl;

	os << "P = {" << endl;
	for(size_t i = 0; i < p.size(); ++i)
	{
		os << "\t{ " << p[i].left << " -> ";
		for(size_t j = 0; j < p[i].right.size(); ++j)
		{
			os << p[i].right[j] << " ";
		}
		os << "}," << endl;
	}
	os << "}" << endl << endl;

	os << "S = " << s << endl << endl;

	os << endl;
}

void CFG::displayGeneratingSymbols(ostream& os)
{
	os << "generatingSymbols = {" << endl;
	for(size_t i = 0; i < generatingSymbols.size(); ++i)
	{
		os << "\t" << generatingSymbols[i] << "," << endl;
	}
	os << "}" << endl << endl;
}

void CFG::displayReachableSymbols(ostream& os)
{
	os << "reachableSymbols = {" << endl;
	for(size_t i = 0; i < reachableSymbols.size(); ++i)
	{
		os << "\t" << reachableSymbols[i] << "," << endl;
	}
	os << "}" << endl << endl;
}

void CFG::displayNullableSymbols(ostream& os)
{
	os << "nullableSymbols = {" << endl;
	for(size_t i = 0; i < nullableSymbols.size(); ++i)
	{
		os << "\t" << nullableSymbols[i] << "," << endl;
	}
	os << "}" << endl << endl;
}

void CFG::displayUnitPairs(ostream& os)
{
	os << "unitPairs = {" << endl;
	for(size_t i = 0; i < unitPairs.size(); ++i)
	{
		os << "\t(" << unitPairs[i].first << ", " << unitPairs[i].second << ")," << endl;
	}
	os << "}" << endl << endl;
}

int CFG::getVariableIndex(const string& a)
{
	auto it = find(v.begin(), v.end(), a);
	return it == v.end()? -1: it - v.begin();
}

int CFG::getTerminalIndex(const string& a)
{
	auto it = find(t.begin(), t.end(), a);
	return it == t.end()? -1: it - t.begin();
}

int CFG::getFirstIndex(const string& symbol)
{
	return find_if(
		first.begin(), 
		first.end(), 
		[symbol](pair<string, vector<string>> pr){
			return pr.first == symbol;
		}
	) - first.begin();
}

int CFG::getFollowIndex(const string& symbol)
{
	return find_if(
		follow.begin(), 
		follow.end(), 
		[symbol](pair<string, vector<string>> pr){
			return pr.first == symbol;
		}
	) - follow.begin();
}

void CFG::computeFirst()
// elements are terminals
// for all grammar symbols
{
	first.clear();

	for(size_t i = 0; i < t.size(); ++i)
	{
		first.push_back(make_pair(t[i], vector<string>({t[i]})));
	}

	for(size_t i = 0; i < v.size(); ++i)
	{
		first.push_back(make_pair(v[i], vector<string>()));
	}

	bool updated = false;
	do
	{
		updated = false;

		for(size_t i = 0; i < v.size(); ++i)
		{
			int xFirstIndex = getFirstIndex(v[i]);
			vector<string> firstX = first[xFirstIndex].second;

			for(size_t j = 0; j < p.size(); ++j)
			{
				if(p[j].left == v[i])
				// X -> Y1 ...
				{
					if(p[j].right.size() == 1 && p[j].right[0] == "")
					{
						if(add(firstX, {""}))
						{
							updated = true;
						}
					}
					else
					{
						size_t k = 0;
						for(; k < p[j].right.size(); )
						{
							vector<string> firstY = first[getFirstIndex(p[j].right[k])].second;

							del(firstY, string(""));
							if(add(firstX, firstY))
							{
								updated = true;
							}

							if(in(string(""), firstY))
							{
								++k;
							}
							else
							{
								break;
							}
						}
						if(k == p[j].right.size())
						{
							if(add(firstX, {""}));
						}
					}
				}
			}

			first[xFirstIndex].second = firstX;
		}
	}while(updated);
}

vector<string> CFG::computeFirst(const vector<string>& str)
// for any string of grammar symbols
{
	// str = Y1 ...
	vector<string> firstStr;

	if(str.size() == 1 && str[0] == "")
	{
		firstStr = {""};
	}
	else
	{
		size_t i = 0;
		for(; i < str.size(); )
		{
			vector<string> firstY;
			if(str[i] == "$")
			{
				firstY = {"$"};
			}
			else
			{
				firstY = first[getFirstIndex(str[i])].second;
			}

			bool epsilon = del(firstY, string(""));
			add(firstStr, firstY);

			if(epsilon)
			{
				++i;
			}
			else
			{
				break;
			}
		}
		if(i == str.size())
		{
			if(add(firstStr, {""}));
		}
	}
	
	return firstStr;
}

void CFG::computeFollow()
// elements are terminals
// for all nonterminals
{
	computeFirst();

	follow.clear();

	for(size_t i = 0; i < v.size(); ++i)
	{
		follow.push_back(make_pair(v[i], vector<string>()));
		// $ represents the end of input
	}

	follow[getFollowIndex(s)].second.push_back("$");

	bool updated = false;
	do
	{
		updated = false;

		for(size_t j = 0; j < p.size(); ++j)
		{
			// A -> alpha B beta
			// case1: A -> alpha B ==>==> follow(A) subset follow(B)
			// case2: A -> alpha B beta ==> (first(beta) except {epsilon}) subset follow(B)
			//                              epsilon in first(beta) ==> follow(A) subset follow(B)

			for(size_t k = 0; k < p[j].right.size(); ++k)
			{
				if(in(p[j].right[k], v))
				{
					int aFollowIndex = getFollowIndex(p[j].left);
					vector<string> followA = follow[aFollowIndex].second;

					int bFollowIndex = getFollowIndex(p[j].right[k]);
					vector<string> followB = follow[bFollowIndex].second;

					vector<string> beta;
					vector<string> firstBeta;

					if(k < p[j].right.size() - 1)
					{
						for(size_t l = k + 1; l < p[j].right.size(); ++l)
						{
							beta.push_back(p[j].right[l]);
						}

						firstBeta = computeFirst(beta);

						bool epsilon = del(firstBeta, string(""));

						bool added = add(followB, firstBeta);
						if(added)
						{
							updated = true;
						}

						if(epsilon)
						{
							bool added = add(followB, followA);
							if(added)
							{
								updated = true;
							}
						}
					}
					else
					{
						bool added = add(followB, followA);
						if(added)
						{
							updated = true;
						}
					}

					follow[bFollowIndex].second = followB;
				}
			}
		}
	}while(updated);
}

void CFG::displayFirst(ostream& os)
{
	for(size_t i = 0; i < first.size(); ++i)
	{
		os << "first(" << first[i].first << ") = {" << endl;

		for(size_t j = 0; j < first[i].second.size(); ++j)
		{
			os << "\t" << first[i].second[j] << "," << endl;
		}

		os << "}" << endl << endl;
	}
}

void CFG::displayFollow(ostream& os)
{
	for(size_t i = 0; i < follow.size(); ++i)
	{
		os << "follow(" << follow[i].first << ") = {" << endl;

		for(size_t j = 0; j < follow[i].second.size(); ++j)
		{
			os << "\t" << follow[i].second[j] << "," << endl;
		}

		os << "}" << endl << endl;
	}
}

void CFG::augmentGrammar()
{
	p.push_back(Production("_" + s, {s}));
	v.push_back("_" + s);
	s = "_" + s;
	augmented = true;
}

int CFG::getProductionIndex(const Production& production)
{
	return find_if(
		p.begin(), 
		p.end(), 
		[production](const Production& pr)
		{
			return pr.left == production.left
				&& pr.right == pr.right;
		}
	) - p.begin();
}

vector<LR0Item> CFG::closure(const vector<LR0Item>& is)
{
	vector<LR0Item> js = is;
	bool updated = false;

	do
	{
		updated = false;

		for(size_t i = 0; i < js.size(); ++i)
		{
			// A -> alpha . B beta
			Production pr = p[js[i].productionIndex];
			if(js[i].dotPosition < pr.right.size())
			{
				string b = pr.right[js[i].dotPosition];
				if(in(b, v))
				{
					for(size_t j = 0; j < p.size(); ++j)
					{
						if(p[j].left == b)
						{
							bool exist = false;

							for(size_t k = 0; k < js.size(); ++k)
							{
								if(js[k].productionIndex == j
									&& js[k].dotPosition == 0)
								{
									exist = true;
									break;
								}
							}

							if(!exist)
							{
								js.push_back(LR0Item(j, 0));
								updated = true;
							}
						}
					}
				}
			}
		}
	}while(updated);

	return js;
}

vector<LR0Item> CFG::goTo(const vector<LR0Item>& is, const string& x)
{
	vector<LR0Item> js;

	for(size_t i = 0; i < is.size(); ++i)
	{
		Production pr = p[is[i].productionIndex];
		if(!(pr.right.size() == 1 && pr.right[0] == "")
			&& is[i].dotPosition < pr.right.size()
			&& pr.right[is[i].dotPosition] == x)
		{
			js.push_back(LR0Item(is[i].productionIndex, is[i].dotPosition + 1));
		}
	}
	
	return closure(js);
}

void CFG::constructCanonicalLR0Collection()
{
	if(!augmented)
	{
		augmentGrammar();
	}

	canonicalLR0Collection.clear();

	canonicalLR0Collection = vector<vector<LR0Item>>(
	{
		closure(
			vector<LR0Item>(
			{
				LR0Item(
					find_if(
						p.begin(), 
						p.end(),
						[this](const Production& pr)
						{
							return pr.left == s; 
						}
					) - p.begin(),
					0
				)
			})
		)
	});

	bool updated = false;
	vector<string> symbols = v;
	symbols.insert(symbols.end(), t.begin(), t.end());

	do
	{
		updated = false;

		for(size_t i = 0; i < canonicalLR0Collection.size(); ++i)
		{
			for(size_t j = 0; j < symbols.size(); ++j)
			{
				vector<LR0Item> goToIX = goTo(canonicalLR0Collection[i], symbols[j]);
				if(goToIX.size() != 0 && !in(goToIX, canonicalLR0Collection))
				{
					canonicalLR0Collection.push_back(goToIX);
					updated = true;
				}
			}
		}
	}while(updated);
}

int CFG::getLR0ItemSetIndex(const vector<LR0Item>& is)
{
	auto it = find(canonicalLR0Collection.begin(), canonicalLR0Collection.end(), is);
	return it == canonicalLR0Collection.end()? -1: it - canonicalLR0Collection.begin();
}

void CFG::displayLR0ItemSet(const vector<LR0Item>& is, ostream& os)
{
	os << "{" << endl;
	for(size_t i = 0; i < is.size(); ++i)
	{
		Production pr = p[is[i].productionIndex];
		os << "\t" << pr.left << " -> ";
		for(size_t k = 0; k < pr.right.size(); ++k)
		{
			if(k == is[i].dotPosition)
			{
				os << ". ";
			}
			os << pr.right[k] << " ";
		}
		if(is[i].dotPosition == pr.right.size())
		{
			os << ". ";
		}
		os << ", " << endl;
	}
	os << "}" << endl << endl;
}

void CFG::displayCanonicalLR0Collection(ostream& os)
{
	os << "canonicalLR0Collection = {" << endl;

	for(size_t i = 0; i < canonicalLR0Collection.size(); ++i)
	{
		os << "\tI" << i << " = {" << endl;

		for(size_t j = 0; j < canonicalLR0Collection[i].size(); ++j)
		{
			os << "\t\t";
			Production pr = p[canonicalLR0Collection[i][j].productionIndex];
			os << pr.left << " -> ";
			for(size_t k = 0; k < pr.right.size(); ++k)
			{
				if(k == canonicalLR0Collection[i][j].dotPosition)
				{
					os << ". ";
				}
				os << pr.right[k] << " ";
			}
			if(canonicalLR0Collection[i][j].dotPosition == pr.right.size())
			{
				os << ". ";
			}
			os << ", " << endl;
		}

		os << "\t}" << endl << endl;
	}

	os << "}" << endl << endl;
}

vector<LR1Item> CFG::closure(const vector<LR1Item>& is)
{
	vector<LR1Item> js = is;
	bool updated = false;

	do
	{
		updated = false;

		for(size_t i = 0; i < js.size(); ++i)
		{
			// (A -> alpha . B beta, a)
			Production pr = p[js[i].productionIndex];

			if(js[i].dotPosition < pr.right.size())
			{
				string B = pr.right[js[i].dotPosition];
				if(in(B, v))
				{
					vector<string> betaa;
					for(size_t k = js[i].dotPosition + 1; k < pr.right.size(); ++k)
					{
						betaa.push_back(pr.right[k]);
					}
					betaa.push_back(js[i].lookahead);

					vector<string> firstBetaa = computeFirst(betaa);

					for(size_t j = 0; j < p.size(); ++j)
					{
						if(p[j].left == B)
						{
							for(size_t k = 0; k < firstBetaa.size(); ++k)
							{
								string b = firstBetaa[k];
								if(b != "")
								{
									bool exist = false;

									for(size_t l = 0; l < js.size(); ++l)
									{
										if(js[l].productionIndex == j
											&& js[l].dotPosition == 0
											&& js[l].lookahead == b)
										{
											exist = true;
											break;
										}
									}

									if(!exist)
									{
										js.push_back(LR1Item(j, 0, b));
										updated = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}while(updated);

	return js;
}

vector<LR1Item> CFG::goTo(const vector<LR1Item>& is, const string& x)
{
	vector<LR1Item> js;
	for(size_t i = 0; i < is.size(); ++i)
	{
		Production pr = p[is[i].productionIndex];
		if(!(pr.right.size() == 1 && pr.right[0] == "")
			&& is[i].dotPosition < pr.right.size()
			&& pr.right[is[i].dotPosition] == x)
		{
			js.push_back(
				LR1Item(
					is[i].productionIndex, 
					is[i].dotPosition + 1, 
					is[i].lookahead
				)
			);
		}
	}
	return closure(js);
}

void CFG::constructCanonicalLR1Collection()
{
	if(!augmented)
	{
		augmentGrammar();
	}

	canonicalLR1Collection.clear();

	canonicalLR1Collection = vector<vector<LR1Item>>(
	{
		closure(
			vector<LR1Item>(
			{
				LR1Item(
					find_if(
						p.begin(), 
						p.end(),
						[this](const Production& pr)
						{
							return pr.left == s; 
						}
					) - p.begin(),
					0,
					"$"
				)
			})
		)
	});

	bool updated = false;
	vector<string> symbols = v;
	symbols.insert(symbols.end(), t.begin(), t.end());

	do
	{
		updated = false;

		for(size_t i = 0; i < canonicalLR1Collection.size(); ++i)
		{
			for(size_t j = 0; j < symbols.size(); ++j)
			{
				vector<LR1Item> goToIX = goTo(canonicalLR1Collection[i], symbols[j]);
				if(goToIX.size() != 0 && !in(goToIX, canonicalLR1Collection))
				{
					canonicalLR1Collection.push_back(goToIX);
					updated = true;
				}
			}
		}
	}while(updated);
}

int CFG::getLR1ItemSetIndex(const vector<LR1Item>& is)
{
	auto it = find(canonicalLR1Collection.begin(), canonicalLR1Collection.end(), is);
	return it == canonicalLR1Collection.end()? -1: it - canonicalLR1Collection.begin();
}

void CFG::displayLR1ItemSet(const vector<LR1Item>& is, ostream& os)
{
	os << "{" << endl;
	for(size_t i = 0; i < is.size(); ++i)
	{
		Production pr = p[is[i].productionIndex];
		os << "\t" << pr.left << " -> ";
		for(size_t k = 0; k < pr.right.size(); ++k)
		{
			if(k == is[i].dotPosition)
			{
				os << ". ";
			}
			os << pr.right[k] << " ";
		}
		if(is[i].dotPosition == pr.right.size())
		{
			os << ". ";
		}
		os << ", " << is[i].lookahead << "," << endl;
	}
	os << "}" << endl << endl;
}

void CFG::displayCanonicalLR1Collection(ostream& os)
{
	os << "canonicalLR1Collection = {" << endl;

	for(size_t i = 0; i < canonicalLR1Collection.size(); ++i)
	{
		os << "\tI" << i << " = {" << endl;

		for(size_t j = 0; j < canonicalLR1Collection[i].size(); ++j)
		{
			os << "\t\t";
			Production pr = p[canonicalLR1Collection[i][j].productionIndex];
			os << pr.left << " -> ";
			for(size_t k = 0; k < pr.right.size(); ++k)
			{
				if(k == canonicalLR1Collection[i][j].dotPosition)
				{
					os << ". ";
				}
				os << pr.right[k] << " ";
			}
			if(canonicalLR1Collection[i][j].dotPosition == pr.right.size())
			{
				os << ". ";
			}
			os << ", " << canonicalLR1Collection[i][j].lookahead << "," << endl;
		}

		os << "\t}" << endl << endl;
	}

	os << "}" << endl << endl;
}

bool CFG::hasSameCore(const vector<LR1Item>& is1, const vector<LR1Item>& is2)
{
	bool hasSameCore = true;

	for(size_t i = 0; i < is1.size(); ++i)
	{
		LR1Item i1 = is1[i];
		if(find_if(
			is2.begin(),
			is2.end(),
			[i1](const LR1Item& i2)
			{
				return i1.productionIndex == i2.productionIndex
					&& i1.dotPosition == i2.dotPosition;
			}) == is2.end())
		{
			hasSameCore = false;
			break;
		}
	}

	return hasSameCore;
}
