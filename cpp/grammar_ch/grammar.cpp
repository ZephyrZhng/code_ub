#include "grammar.h"

vector<string> split(const string& s, char delim)
{
	stringstream ss(s);
	vector<string> elems;
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

template<typename T>
void setUnion(vector<T>& va, const vector<T>& vb)
{
	for(size_t i = 0; i < vb.size(); ++i)
	{
		if(find(va.begin(), va.end(), vb[i]) == va.end())
		{
			va.push_back(vb[i]);
		}
	}
}

template<typename T>
void setUnion(set<T>& sa, const set<T>& sb)
{
	for(auto itr = sb.begin(); itr != sb.end(); ++itr)
	{
		sa.insert(*itr);
	}
}

template<typename T>
void display(const vector<T>& vec)
{
	cout << "{";
	for(size_t i = 0; i < vec.size(); ++i)
	{
		cout << vec[i];
		if(i != vec.size() - 1)
		{
			cout << ",";
		}
	}
	cout << "}";
}

template<typename T>
void display(const set<T>& s)
{
	cout << "{";
	for(auto p = s.begin(); p != s.end(); ++p)
	{
		cout << *p;
		if(p != prev(s.end()))
		{
			cout << ",";
		}
	}
	cout << "}";
}

ProductionRule::ProductionRule()
{

}

ProductionRule::ProductionRule(const char& l, const vector<string>& r): lPart(l), rPartSet(r)
{

}

void ProductionRule::display()
{
	cout << lPart << "->";
	for(size_t i = 0; i < rPartSet.size(); ++i)
	{
		cout << rPartSet[i];
		if(i != rPartSet.size() - 1)
		{
			cout << "|";
		}
	}
}

Grammar::Grammar(const string& filePath)
{
	ifstream f(filePath);
	assert(f);

	// file format
	// VNSize VN
	// VTSize VT
	// PSize
	// P
	// S

	int VNSize;
	f >> VNSize;
	VN.resize(VNSize);
	for(int i = 0; i < VNSize; ++i)
	{
		f >> VN[i];
	}

	int VTSize;
	f >> VTSize;
	VT.resize(VTSize);
	for(int i = 0; i < VTSize; ++i)
	{
		f >> VT[i];
	}

	struct functor: public binary_function<ProductionRule, char, bool>{
	public:
		bool operator ()(const ProductionRule pr, const char ch) const {
			return (pr.lPart == ch);
		}
	};

	int PSize;
	f >> PSize;
	string str;
	for(int i = 0; i < PSize; ++i)
	{
		f >> str;
		vector<string> v_pr = split(str, '>');
		v_pr[0].erase(v_pr[0].end() - 1);
		vector<string> v_pr_r = split(v_pr[1], '|');

		functor myFunctor;
		auto itr = find_if(P.begin(), P.end(), bind2nd(myFunctor, *(v_pr[0].begin())));
		if(itr == P.end())
		{
			ProductionRule pr;
			pr.lPart = *(v_pr[0].begin());
			for(size_t j = 0; j < v_pr_r.size(); ++j)
			{
				pr.rPartSet.push_back(v_pr_r[j]);
			}
			P.push_back(pr);
		}
		else
		{
			for(size_t j = 0; j < v_pr_r.size(); ++j)
			{
				P[itr - P.begin()].rPartSet.push_back(v_pr_r[j]);
			}
		}
	}

	f >> S;

	f.close();

	sort(VN.begin(), VN.end());
	sort(VT.begin(), VT.end());
	for(size_t i = 0; i < P.size(); ++i)
	{
		sort(P[i].rPartSet.begin(), P[i].rPartSet.end());
	}
	sort(P.begin(), P.end(),
		[](ProductionRule pr1, ProductionRule pr2)
		{
			return pr1.lPart < pr2.lPart?
				true: (pr1.lPart > pr2.lPart?
					false: pr1.rPartSet[0] < pr2.rPartSet[0]);
		});

	nullable.assign(VN.size(), false);
	first.assign(VN.size() + VT.size(), set<char>());
	follow.assign(VN.size(), set<char>());

	display();

	calNullableFirstFollow();
}

void Grammar::calNullableFirstFollow()
{
	// calculate nullable

	for(size_t i = 0; i < P.size(); ++i)
	{
		for(size_t j = 0; j < P[i].rPartSet.size(); ++j)
		{
			if(P[i].rPartSet[j].size() == 1 && *(P[i].rPartSet[j].begin()) == 'e')
			{
				nullable[i] = true;
				break;
			}
		}
	}

	bool updated = true;
	while(updated)
	{
		updated = false;

		for(size_t i = 0; i < P.size(); ++i)
		{
			for(size_t j = 0; j < P[i].rPartSet.size(); ++j)
			{
				bool allNullable = true;
				for(size_t k = 0; k < P[i].rPartSet[j].size(); ++k)
				{
					auto ptr = find(VN.begin(), VN.end(), P[i].rPartSet[j][k]);
					if(ptr == VN.end() || !nullable[ptr - VN.begin()])
					{
						allNullable = false;
						break;
					}
				}

				if(allNullable && !nullable[i])
				{
					updated = true;
					nullable[i] = true;
					break;
				}
			}
		}
	}

	// calculate first

	for(size_t i = 0; i < VT.size(); ++i)
	{
		first[i].insert(VT[i]);
	}

	updated = true;
	while(updated)
	{
		updated = false;

		for(size_t i = 0; i < P.size(); ++i)
		{
			for(size_t j = 0; j < P[i].rPartSet.size(); ++j)
			{
				int idx = find(VN.begin(), VN.end(), P[i].lPart) - VN.begin() + VT.size();

				int l = 0;
				for(size_t k = 0; k < P[i].rPartSet[j].size(); ++k)
				{
					auto ptr = find(VN.begin(), VN.end(), P[i].rPartSet[j][k]);
					if(ptr == VN.end() || !nullable[ptr - VN.begin()])
					{
						break;
					}
					else
					{
						++l;

						int oldSize = first[idx].size();
						int newSize = 0;
						auto ptr = find(VN.begin(), VN.end(), P[i].rPartSet[j][k]);
						setUnion(first[idx], first[ptr - VN.begin() + VT.size()]);
						newSize = first[idx].size();
						if(oldSize != newSize)
						{
							updated = true;
						}
					}
				}

				/////////////////////////////////
				// cout << P[i].lPart << "->" << P[i].rPartSet[j] << " " << l << endl;

				int oldSize = first[idx].size();
				int newSize = 0;
				if(l >= P[i].rPartSet[j].size())
				{
					first[idx].insert('e');
					newSize = first[idx].size();
				}
				else
				{
					auto ptr = find(VN.begin(), VN.end(), P[i].rPartSet[j][l]);
					if(ptr == VN.end())
					{
						if(P[i].rPartSet[j][l] != 'e')
						{
							setUnion(first[idx], first[find(VT.begin(), VT.end(), P[i].rPartSet[j][l]) - VT.begin()]);

							/////////////////////////////////
							// cout << i << " " << idx << " ";
							// ::display(first[find(VT.begin(), VT.end(), P[i].rPartSet[j][l]) - VT.begin()]); cout << endl << endl;
						}
						else
						{
							setUnion(first[idx], set<char>({'e'}));
						}
					}
					else
					{
						setUnion(first[idx], first[ptr - VN.begin() + VT.size()]);
					}

					newSize = first[idx].size();
				}
			
				if(oldSize != newSize)
				{
					updated = true;
				}
			}
		}
	}

	// sth about $ and e

	follow[find(VN.begin(), VN.end(), S) - VN.begin()].insert('$');

	updated = true;
	while(updated)
	{
		updated = false;

		for(size_t i = 0; i < P.size(); ++i)
		{
			for(size_t j = 0; j < P[i].rPartSet.size(); ++j)
			{
				int idx = find(VN.begin(), VN.end(), P[i].lPart) - VN.begin();

				for(size_t k = 0; k < P[i].rPartSet[j].size(); ++k)
				{
					bool allNullable1 = true;
					for(size_t cnt = k + 1; cnt < P[i].rPartSet[j].size(); ++cnt)
					{
						if(P[i].rPartSet[j][cnt] != 'e')
						{
							auto cntPtr = find(VN.begin(), VN.end(), P[i].rPartSet[j][cnt]);
							if(cntPtr == VN.end() || !nullable[cntPtr - VN.begin()])
							{
								allNullable1 = false;
								break;
							}
						}
						else
						{
							break;
						}
					}
					if(allNullable1)
					{
						auto kPtr = find(VN.begin(), VN.end(), P[i].rPartSet[j][k]);
						if(kPtr != VN.end())
						{
							int oldSize = follow[kPtr - VN.begin()].size();
							setUnion(follow[kPtr - VN.begin()], follow[idx]);
							int newSize = follow[kPtr - VN.begin()].size();
							if(oldSize != newSize)
							{
								updated = true;
							}
						}
					}

					for(size_t l = k + 1; l < P[i].rPartSet[j].size(); ++l)
					{
						bool allNullable2 = true;
						for(int cnt = k + 1; cnt < l - 1; ++cnt)
						{
							if(P[i].rPartSet[j][cnt] != 'e')
							{
								auto cntPtr = find(VN.begin(), VN.end(), P[i].rPartSet[j][cnt]);
								if(cntPtr == VN.end() || !nullable[cntPtr - VN.begin()])
								{
									allNullable2 = false;
									break;
								}
							}
							else
							{
								break;
							}
						}
						if(allNullable2)
						{
							auto kPtr = find(VN.begin(), VN.end(), P[i].rPartSet[j][k]);
							if(kPtr != VN.end())
							{
								auto lPtr = find(VN.begin(), VN.end(), P[i].rPartSet[j][l]);
								int lIdx = (lPtr == VN.end())?
									(find(VT.begin(), VT.end(), P[i].rPartSet[j][l]) - VT.begin()):
									(lPtr - VN.begin() + VT.size());
								int oldSize = follow[kPtr - VN.begin()].size();
								set<char> __first__ = first[lIdx];
								__first__.erase('e');
								setUnion(follow[kPtr - VN.begin()], __first__);
								int newSize = follow[kPtr - VN.begin()].size();
								if(oldSize != newSize)
								{
									updated = true;
								}
							}
						}
					}
				}
			}
		}
	}

	// display nullable
	cout << "nullable" << endl;
	for(size_t i = 0; i < nullable.size(); ++i)
	{
		cout << VN[i] << ": " << ((nullable[i])? "nullable": "not nullable") << endl;
	}
	cout << endl;

	// display first
	cout << "first" << endl;
	for(size_t i = 0; i < first.size(); ++i)
	{
		if(i < VT.size())
		{
			cout << VT[i] << ": ";
			::display(first[i]);
			cout << endl;
		}
		else
		{
			cout << VN[i - VT.size()] << ": ";
			::display(first[i]);
			cout << endl;
		}
	}
	cout << endl;

	// display follow
	cout << "follow" << endl;
	for(size_t i = 0; i < follow.size(); ++i)
	{
		cout << VN[i] << ": ";
		::display(follow[i]);
		cout << endl;
	}
	cout << endl;
}

void Grammar::display()
{
	cout << "G = (VN,VT,P,S)\n";

	auto lambda = [](const string& str){ cout << str; };

	cout << "VN = ";
	::display(VN);
	cout << endl;

	cout << "VT = ";
	::display(VT);
	cout << endl;

	cout << "P:" << endl;
	for(size_t i = 0; i < P.size(); ++i)
	{
		cout << P[i].lPart << "->";
		for(size_t j = 0; j < P[i].rPartSet.size(); ++j)
		{
			cout << P[i].rPartSet[j];
			if(j != P[i].rPartSet.size() - 1)
			{
				cout << "|";
			}
		}
		cout << endl;
	}

	cout << "S = " << S << endl << endl;
}

bool Grammar::isNonTerminal(char ch)
{
	return find(VN.begin(), VN.end(), ch) != VN.end();
}

int Grammar::getProductionRuleByLeft(ProductionRule& pr, char lPart)
{
	auto ptr = find_if(P.begin(), P.end(),
		[lPart](const ProductionRule& pr)
		{
			return pr.lPart == lPart;
		});
	if(ptr == P.end())
	{
		pr = ProductionRule();
		return -1;
	}
	else
	{
		pr = *ptr;
		return ptr - P.begin();
	}
}

set<char> Grammar::getFirst(char ch)
{
	auto vnPtr = find(VN.begin(), VN.end(), ch);
	auto vtPtr = find(VT.begin(), VT.end(), ch);

	if(vnPtr != VN.end())
	{
		return first[vnPtr - VN.begin() + VT.size()];
	}
	else if(vtPtr != VT.end())
	{
		return first[vtPtr - VT.begin()];
	}
	else if(ch == 'e')
	{
		return set<char>({'e'});
	}
	else if(ch == '$')
	{
		return set<char>({'$'});
	}
	else
	{
		return set<char>();
	}
}

set<char> Grammar::getFirst(string str)
{
	set<char> ret;
	for(auto& p: str)
	{
		set<char> firsti = getFirst(p);
		setUnion(ret, firsti);
		if(find(firsti.begin(), firsti.end(), 'e') == firsti.end())
		{
			break;
		}
	}
	return ret;
}

set<char> Grammar::getFollow(char ch)
{
	auto ptr = find(VN.begin(), VN.end(), ch);
	if(ptr != VN.end())
	{
		return follow[ptr - VN.begin()];
	}
	else
	{
		return set<char>();
	}
}