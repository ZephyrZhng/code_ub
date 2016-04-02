#include "simplifyCFG.h"

template<typename T>
void display(const vector<T>& v)
{
	for(auto& p: v)
	{
		cout << p << " ";
	}
	cout << endl;
}

string char2string(char ch)
{
	string str;
	str.insert(str.begin(), ch);
	return str;
}

void split( const string& str,
			const string::const_iterator& begin_it, const string::const_iterator& end_it,
			const char& ch, vector<string>& v)
{
	auto p = begin_it;
	auto q = find(p, end_it, ch);
	while(end_it - p >= 0)
	{
		q = find(p, end_it, ch);
		v.push_back(str.substr(p - str.begin(), q - p));
		p = q + 1;
	}
}

void _split(string str, vector<string>& substr_v)
{
	auto border_it = find(str.begin(), str.end(), '-');
	string right_substr = str.substr(border_it - str.begin() + 2, str.end() - border_it);
	split(right_substr, right_substr.begin(), right_substr.end(), '|', substr_v);
}

void merge(string& str, char ch, const vector<string>& v)
{
	if(!v.empty())
	{
		string _str;
		_str.insert(_str.begin(), ch);
		for(size_t i = 0; i < v.size(); ++i)
		{
			str += v[i];
			str += _str;
		}
		str.erase(str.end() - 1);
	}
}

ContextFreeGrammar::ContextFreeGrammar(const string& cfgFiltPath)
{
	ifstream f(cfgFiltPath);
	assert(f);

	char ch;
	string str;

	int vn_size;
	f >> vn_size;
	for(int i = 0; i < vn_size; ++i)
	{
		f >> ch;
		vn.insert(ch);
	}

	int vt_size;
	f >> vt_size;
	for(int i = 0; i < vt_size; ++i)
	{
		f >> ch;
		vt.insert(ch);
	}

	int p_size;
	f >> p_size;
	for(int i = 0; i < p_size; ++i)
	{
		f >> str;
		p.insert(str);
	}

	f >> s;

	f.close();

	c = 'A';
	for(auto vn_it = vn.begin(); vn_it != vn.end(); ++vn_it)
	{
		if(*vn_it != 'S' && *vn_it > c)
		{
			c = *vn_it;
		}
	}
	++c;
	displayCFG();
}

template<typename T>
void ContextFreeGrammar::displaySet(const set<T>& Set)
{
	for(auto& p: Set)
	{
		cout << p << endl;
	}
}

void ContextFreeGrammar::displayCFG()
{
	cout << "vn:" << endl;
	displaySet(vn);
	cout << "------------------" << endl;
	cout << "vt:" << endl;
	displaySet(vt);
	cout << "------------------" << endl;
	cout << "p:" << endl;
	displaySet(p);
	cout << "------------------" << endl;
}

bool ContextFreeGrammar::isNonTerminal(char ch)
{
	return find(vn.begin(), vn.end(), ch) != vn.end();
}

bool ContextFreeGrammar::isTerminal(char ch)
{
	return find(vt.begin(), vt.end(), ch) != vt.end();
}

bool ContextFreeGrammar::canDeriveNonTerminalString(char ch)
{
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		if(*((*p_it).begin()) == ch)
		{
			vector<string> substr_v;
			_split(*p_it, substr_v);

			for(size_t i = 0; i < substr_v.size(); ++i)
			{
				bool flag = true;
				for(auto substr_it = substr_v[i].begin(); substr_it != substr_v[i].end(); ++substr_it)
				{
					if(find(vt.begin(), vt.end(), *substr_it) == vt.end()
						&& *substr_it != 'e')
					{
						flag = false;
					}
				}
				if(flag)
				{
					return true;
				}
			}
		}
	}
	return false;
}


template<typename T>
void ContextFreeGrammar::setReplace(set<T>& destSet, const set<T>& srcSet)
{
	destSet.clear();
	for(auto p = srcSet.begin(); p != srcSet.end(); ++p)
	{
		destSet.insert(*p);
	}
}

template<typename T>
bool ContextFreeGrammar::setEqual(set<T>& s1, set<T>& s2)
{
	if(s1.size() != s2.size())
	{
		return false;
	}
	auto p = s1.begin();
	auto q = s2.begin();
	for(; p != s1.end() && q != s2.end(); ++p, ++q)
	{
		if(*p != *q)
		{
			return false;
		}
	}
	return true;
}

void ContextFreeGrammar::_eliminateUselessSymbolics()
{
	for(auto p_it = p.begin(); p_it != p.end(); )
	{
		int pos = distance(p.begin(), p_it);
		bool flag = 1;
		if(find(vn.begin(), vn.end(), *((*p_it).begin())) == vn.end())
		{
			p.erase(p_it);
			flag = 0;
		}
		if(!flag)
		{
			p_it = p.begin();
			for(int i = 0; i < pos; ++i)
			{
				p_it = next(p_it);
			}
		}
		else
		{
			++p_it;
		}
	}

	set<string> new_p;
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		vector<string> substr_v;
		_split(*p_it, substr_v);

		for(size_t i = 0; i < substr_v.size(); )
		{
			bool flag = 1;
			for(auto substr_it = substr_v[i].begin(); substr_it != substr_v[i].end(); ++substr_it)
			{
				if(find(vn.begin(), vn.end(), *substr_it) == vn.end()
					&& find(vt.begin(), vt.end(), *substr_it) == vt.end()
					&& *substr_it != 'e')
				{
					substr_v.erase(substr_v.begin() + i);
					flag = 0;
					break;
				}
			}
			if(flag)
			{
				++i;
			}
		}

		string new_string;
		merge(new_string, '|', substr_v);
		new_string.insert(new_string.begin(), '>');
		new_string.insert(new_string.begin(), '-');
		new_string.insert(new_string.begin(), *((*p_it).begin()));
		new_p.insert(new_string);
	}

	setReplace(p, new_p);
}

void ContextFreeGrammar::eliminateUselessSymbolics1()
{
	cout << "eliminate useless symbolics 1:" << endl;

	set<char> OLD_VN;
	set<char> NEW_VN;
	for(auto vn_it = vn.begin(); vn_it != vn.end(); ++vn_it)
	{
		if(canDeriveNonTerminalString(*vn_it))
		{
			NEW_VN.insert(*vn_it);
		}
	}
	while(!setEqual(OLD_VN, NEW_VN))
	{
		setReplace(OLD_VN, NEW_VN);
		for(auto vn_it = vn.begin(); vn_it != vn.end(); ++vn_it)
		{
			bool flag = false;
			for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
			{
				if(*((*p_it).begin()) == *vn_it)
				{
					vector<string> substr_v;
					_split(*p_it, substr_v);

					for(size_t i = 0; i < substr_v.size(); ++i)
					{
						bool _flag = true;
						for(auto substr_it = substr_v[i].begin(); substr_it != substr_v[i].end(); ++substr_it)
						{
							if(find(OLD_VN.begin(), OLD_VN.end(), *substr_it) == OLD_VN.end()
								&& find(vt.begin(), vt.end(), *substr_it) == vt.end()
								&& *substr_it != 'e')
							{
								_flag = false;
								break;
							}
						}
						if(_flag)
						{
							flag = true;
							goto isrt;
						}
					}
				}
			}
			isrt:
			if(flag)
			{
				NEW_VN.insert(*vn_it);
			}
		}
	}
	setReplace(vn, NEW_VN);

	_eliminateUselessSymbolics();
	displayCFG();
}

void ContextFreeGrammar::eliminateUselessSymbolics2()
{
	cout << "eliminate useless symbolics 2:" << endl;

	set<char> OLD_VN;
	set<char> NEW_VN;
	set<char> OLD_VT;
	set<char> NEW_VT;
	NEW_VN.insert('S');

	while(!setEqual(OLD_VN, NEW_VN) || !setEqual(OLD_VT, NEW_VT))
	{
		setReplace(OLD_VN, NEW_VN);
		setReplace(OLD_VT, NEW_VT);
		for(auto OLD_VN_it = OLD_VN.begin(); OLD_VN_it != OLD_VN.end(); ++OLD_VN_it)
		{
			for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
			{
				if(*((*p_it).begin()) == *OLD_VN_it)
				{
					for(auto right_it = (*p_it).begin() + 3; right_it != (*p_it).end(); ++right_it)
					{
						if(find(vn.begin(), vn.end(), *right_it) != vn.end())
						{
							NEW_VN.insert(*right_it);
						}
						if(find(vt.begin(), vt.end(), *right_it) != vt.end())
						{
							NEW_VT.insert(*right_it);
						}
					}
					break;
				}
			}
		}
	}

	setReplace(vn, NEW_VN);
	setReplace(vt, NEW_VT);

	_eliminateUselessSymbolics();
	displayCFG();
}

void ContextFreeGrammar::getV0(set<char>& v0)
{
	set<char> OLD_V0;
	set<char> NEW_V0;
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		if(find((*p_it).begin(), (*p_it).end(), 'e') != (*p_it).end())
		{
			NEW_V0.insert(*((*p_it).begin()));
		}
	}

	while(!setEqual(OLD_V0, NEW_V0))
	{
		setReplace(OLD_V0, NEW_V0);
		for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
		{
			vector<string> substr_v;
			_split(*p_it, substr_v);

			bool flag = false;
			for(size_t i = 0; i < substr_v.size(); ++i)
			{
				bool _flag = true;
				for(auto substr_it = substr_v[i].begin(); substr_it != substr_v[i].end(); ++substr_it)
				{
					if(find(OLD_V0.begin(), OLD_V0.end(), *substr_it) == OLD_V0.end())
					{
						_flag = false;
						break;
					}
				}
				if(_flag)
				{
					flag = true;
					break;
				}
			}
			if(flag)
			{
				NEW_V0.insert(*((*p_it).begin()));
			}
		}
	}

	setReplace(v0, NEW_V0);
}

void ContextFreeGrammar::_eliminateEpsilonProductionRules()
{
	set<string> new_p(p);
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		vector<string> substr_v;
		_split(*p_it, substr_v);

		for(size_t i = 0; i < substr_v.size(); )
		{
			if(substr_v[i].size() == 1 && *(substr_v[i].begin()) == 'e')
			{
				substr_v.erase(substr_v.begin() + i);
			}
			else
			{
				++i;
			}
		}

		new_p.erase(find(new_p.begin(), new_p.end(), *p_it));
		if(!substr_v.empty())
		{
			string new_string;
			merge(new_string, '|', substr_v);
			new_string.insert(new_string.begin(), '>');
			new_string.insert(new_string.begin(), '-');
			new_string.insert(new_string.begin(), *((*p_it).begin()));
			new_p.insert(new_string);
		}
	}
	setReplace(p, new_p);
}

void ContextFreeGrammar::eliminateEpsilonProductionRules()
{
	cout << "eliminate epsilon production rules:" << endl;

	set<char> v0;
	getV0(v0);

	_eliminateEpsilonProductionRules();

	set<string> new_p(p);
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		vector<string> substr_v;
		_split(*p_it, substr_v);

		set<string> newlyAddedProductionRulesSet;
		for(size_t i = 0; i < substr_v.size(); ++i)
		{
			vector<char> epsilonSymbolicsVector;
			for(auto substr_it = substr_v[i].begin(); substr_it != substr_v[i].end(); ++substr_it)
			{
				if(find(v0.begin(), v0.end(), *substr_it) != v0.end())
				{
					epsilonSymbolicsVector.push_back(*substr_it);
				}
			}

			if(!epsilonSymbolicsVector.empty())
			{
				for(size_t num = 1; num < epsilonSymbolicsVector.size(); ++num)
				{
					vector<int> flag_v;
					flag_v.insert(flag_v.end(), num, 1);
					flag_v.insert(flag_v.end(), epsilonSymbolicsVector.size() - num, 0);
					vector<char> _epsilonSymbolicsVector;
					do
					{
						for(size_t iteration_index = 0;
							iteration_index < flag_v.size();
							++iteration_index)
						{
							if(flag_v[iteration_index])
							{
								_epsilonSymbolicsVector.push_back(epsilonSymbolicsVector[iteration_index]);
							}
						}

						string curProductionRule(substr_v[i]);
						for(auto it = _epsilonSymbolicsVector.begin();
							it != _epsilonSymbolicsVector.end();
							++it)
						{
							curProductionRule.erase(
								find(
									curProductionRule.begin(),
									curProductionRule.end(),
									*it
								)
							);
						}

						if(!curProductionRule.empty()
							&& find(substr_v.begin(), substr_v.end(), curProductionRule) == substr_v.end())
						{
							newlyAddedProductionRulesSet.insert(curProductionRule);
						}

						_epsilonSymbolicsVector.clear();
					}while(prev_permutation(flag_v.begin(), flag_v.end()));
				}
			}
		}

		new_p.erase(find(new_p.begin(), new_p.end(), *p_it));
		string new_string = *p_it;
		for(auto it = newlyAddedProductionRulesSet.begin();
			it != newlyAddedProductionRulesSet.end();
			++it)
		{
			new_string += char2string('|');
			new_string += *it;
		}
		new_p.insert(new_string);
	}

	setReplace(p, new_p);
	displayCFG();
}

void ContextFreeGrammar::_eliminateSingleProductionRules(char ch, set<string>& Set)
{
	auto p_it = p.begin();
	for(; p_it != p.end(); ++p_it)
	{
		if(*((*p_it).begin()) == ch)
		{
			break;
		}
	}

	vector<string> substr_v;
	_split(*p_it, substr_v);

	set<char> singleSymbolicsSet;
	for(size_t i = 0; i < substr_v.size(); ++i)
	{
		if(substr_v[i].size() == 1
			&& find(vn.begin(), vn.end(), *(substr_v[i].begin())) != vn.end())
		{
			singleSymbolicsSet.insert(*(substr_v[i].begin()));
		}
		else
		{
			Set.insert(substr_v[i]);
		}
	}

	if(!singleSymbolicsSet.empty())
	{
		for(auto it = singleSymbolicsSet.begin(); it != singleSymbolicsSet.end(); ++it)
		{
			_eliminateSingleProductionRules(*it, Set);
		}
	}
}

void ContextFreeGrammar::eliminateSingleProductionRules()
{
	cout << "eliminate single production rules:" << endl;

	set<string> new_p(p);
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		vector<string> substr_v;
		_split(*p_it, substr_v);

		set<char> singleSymbolicsSet;
		for(size_t i = 0; i < substr_v.size(); )
		{
			if(substr_v[i].size() == 1
				&&  find(vn.begin(), vn.end(), *(substr_v[i].begin())) != vn.end())
			{
				singleSymbolicsSet.insert(*(substr_v[i].begin()));
				substr_v.erase(substr_v.begin() + i);
			}
			else
			{
				++i;
			}
		}

		if(!singleSymbolicsSet.empty())
		{
			new_p.erase(find(new_p.begin(), new_p.end(), *p_it));
			string appendStr;
			merge(appendStr, '|', substr_v);
			appendStr.insert(appendStr.begin(), '>');
			appendStr.insert(appendStr.begin(), '-');
			appendStr.insert(appendStr.begin(), *((*p_it).begin()));

			for(auto it = singleSymbolicsSet.begin(); it != singleSymbolicsSet.end(); ++it)
			{
				set<string> appendSet;
				_eliminateSingleProductionRules(*it, appendSet);
				for(auto appendSet_it = appendSet.begin(); appendSet_it != appendSet.end(); ++appendSet_it)
				{
					if(find(substr_v.begin(), substr_v.end(), *appendSet_it) == substr_v.end())
					{
						if(*(appendStr.end() - 1) != '>')
						{
							appendStr += '|';
						}
						appendStr += *appendSet_it;
					}
				}
			}
			new_p.insert(appendStr);
		}
	}
	setReplace(p, new_p);
	displayCFG();
}

void ContextFreeGrammar::toCNF()
{
	cout << "to CNF:" << endl;

	set<string> new_p(p);
	set<string> appendSet;
	map<char, char> m;
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		vector<string> substr_v;
		_split(*p_it, substr_v);

		for(size_t i = 0; i < substr_v.size(); ++i)
		{
			if(substr_v[i].size() > 1
				&& (find(vt.begin(), vt.end(), *(substr_v[i].begin())) != vt.end()
					|| find(vt.begin(), vt.end(), *(substr_v[i].begin() + 1)) != vt.end()))
			{
				for(auto substr_it = substr_v[i].begin(); substr_it != substr_v[i].end(); ++substr_it)
				{
					if(find(vt.begin(), vt.end(), *substr_it) != vt.end())
					{
						if(!m.count(*substr_it))
						{
							m[*substr_it] = c++;
							vn.insert(m[*substr_it]);
						}
						*substr_it = m[*substr_it];
					}
				}
			}
		}

		for(size_t i = 0; i < substr_v.size(); ++i)
		{
			if(substr_v[i].size() > 2)
			{
				char last   = *(substr_v[i].end() - 1);
				auto cur_it = substr_v[i].end() - 2;
				while(cur_it - substr_v[i].begin() >= 1)
				{
					char new_c = c++;
					vn.insert(new_c);
					string appendStr;
					appendStr.insert(appendStr.begin(), last);
					appendStr.insert(appendStr.begin(), *cur_it);
					appendStr.insert(appendStr.begin(), '>');
					appendStr.insert(appendStr.begin(), '-');
					appendStr.insert(appendStr.begin(), new_c);
					appendSet.insert(appendStr);
					last = new_c;
					--cur_it;
				}
				substr_v[i].erase(substr_v[i].begin() + 1, substr_v[i].end());
				substr_v[i].push_back(last);
			}
		}

		new_p.erase(find(new_p.begin(), new_p.end(), *p_it));
		string new_string;
		merge(new_string, '|', substr_v);
		new_string.insert(new_string.begin(), '>');
		new_string.insert(new_string.begin(), '-');
		new_string.insert(new_string.begin(), *((*p_it).begin()));
		new_p.insert(new_string);
	}

	for(auto m_it = m.begin(); m_it != m.end(); ++m_it)
	{
		string str;
		str.insert(str.begin(), m_it->first);
		str.insert(str.begin(), '>');
		str.insert(str.begin(), '-');
		str.insert(str.begin(), m_it->second);
		new_p.insert(str);
	}

	for(auto appendSet_it = appendSet.begin(); appendSet_it != appendSet.end(); ++appendSet_it)
	{
		new_p.insert(*appendSet_it);
	}

	setReplace(p, new_p);
	displayCFG();
}

int ContextFreeGrammar::order(char a, char b)
{
	if(find(vt.begin(), vt.end(), b) != vt.end())
	{
		return -1;
	}
	if(a == 'S' && b != 'S')	return -1;
	if(a != 'S' && b == 'S')	return 1;
	if(a >   b )				return 1 ;
	if(a ==  b )				return 0 ;
	if(a <   b )				return -1;
}

int ContextFreeGrammar::checkSmall2Big(string str)
{
	vector<string> substr_v;
	_split(str, substr_v);

	bool flag = false;
	size_t i  = 0;
	for(; i < substr_v.size(); ++i)
	{
		if(order(*(str.begin()), *(substr_v[i].begin())) == 1)
		{
			flag = true;
			break;
		}
	}

	return flag? i: -1;
}

void ContextFreeGrammar::checkLeftRecursion(string str, vector<int>& left_recursion_pos_v)
{
	vector<string> substr_v;
	_split(str, substr_v);

	left_recursion_pos_v.clear();

	for(size_t i = 0; i < substr_v.size(); ++i)
	{
		if(*(substr_v[i].begin()) == *(str.begin()))
		{
			left_recursion_pos_v.push_back(i);
		}
	}
}

bool ContextFreeGrammar::checkGNFForm(string str)
{
	vector<string> substr_v;
	_split(str, substr_v);

	for(size_t i = 0; i < substr_v.size(); ++i)
	{
		if(find(vt.begin(), vt.end(), *(substr_v[i].begin())) == vt.end())
		{
			return false;
		}
	}
	return true;
}

void ContextFreeGrammar::toGNF()
{
	cout << "to GNF:" << endl;
	set<string> new_p;
	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		string str          = *p_it;
		int small2big_pos = checkSmall2Big(str);
		while(small2big_pos != -1)
		{
			vector<string> substr_v;
			_split(str, substr_v);

			set<string>::iterator _p_it = p.begin();
			for(; _p_it != p.end() && *((*_p_it).begin()) != *(substr_v[small2big_pos].begin()); ++_p_it);

			string appendStr = substr_v[small2big_pos];
			appendStr.erase(appendStr.begin());
			substr_v.erase(substr_v.begin() + small2big_pos);

			vector<string> _substr_v;
			_split(*_p_it, _substr_v);

			for(size_t i = 0; i < _substr_v.size(); ++i)
			{
				substr_v.push_back(string(_substr_v[i] + appendStr));
			}

			str.clear();
			merge(str, '|', substr_v);
			str.insert(str.begin(), '>');
			str.insert(str.begin(), '-');
			str.insert(str.begin(), *((*p_it).begin()));
			small2big_pos = checkSmall2Big(str);
		}
		new_p.insert(str);
	}

	setReplace(p, new_p);
	new_p.clear();

	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		vector<int> left_recursion_pos_v;
		string str = *p_it;
		checkLeftRecursion(str, left_recursion_pos_v);
		if(!left_recursion_pos_v.empty())
		{
			vector<string> substr_v;
			_split(str, substr_v);

			vector<string> left_recursion_v;
			vector<string> non_left_recursion_v;
			for(size_t i = 0; i < left_recursion_pos_v.size(); ++i)
			{
				left_recursion_v.push_back(substr_v[left_recursion_pos_v[i]]);
				substr_v.erase(substr_v.begin() + left_recursion_pos_v[i]);
			}
			for(size_t i = 0; i < substr_v.size(); ++i)
			{
				non_left_recursion_v.push_back(substr_v[i]);
			}

			vector<string> new_left_recursion_v;
			vector<string> new_non_left_recursion_v;
			for(size_t i = 0; i < non_left_recursion_v.size(); ++i)
			{
				new_non_left_recursion_v.push_back(non_left_recursion_v[i]);
				new_non_left_recursion_v.push_back(non_left_recursion_v[i] + char2string(c));
			}
			for(size_t i = 0; i < left_recursion_v.size(); ++i)
			{
				left_recursion_v[i].erase(left_recursion_v[i].begin());
				new_left_recursion_v.push_back(left_recursion_v[i]);
				new_left_recursion_v.push_back(left_recursion_v[i] + char2string(c));
			}

			string original_string;
			string new_string;
			merge(original_string, '|', new_non_left_recursion_v);
			merge(new_string, '|', new_left_recursion_v);
			original_string.insert(original_string.begin(), '>');
			original_string.insert(original_string.begin(), '-');
			original_string.insert(original_string.begin(), *((*p_it).begin()));
			new_string.insert(new_string.begin(), '>');
			new_string.insert(new_string.begin(), '-');
			new_string.insert(new_string.begin(), c);
			new_p.insert(original_string);
			new_p.insert(new_string);
			vn.insert(c);

			++c;
			left_recursion_pos_v.clear();
		}
		else
		{
			new_p.insert(*p_it);
		}
	}

	setReplace(p, new_p);
	new_p.clear();

	set<string> okGNFProductionRulesSet;
	set<string> nonOKGNFProductionRulesSet;

	for(auto p_it = p.begin(); p_it != p.end(); ++p_it)
	{
		if(checkGNFForm(*p_it))
		{
			okGNFProductionRulesSet.insert(*p_it);
		}
		else
		{
			nonOKGNFProductionRulesSet.insert(*p_it);
		}
	}

	while(!nonOKGNFProductionRulesSet.empty())
	{
		set<string> _nonOKGNFProductionRulesSet;
		for(auto it = nonOKGNFProductionRulesSet.begin();
			it != nonOKGNFProductionRulesSet.end();
			++it)
		{
			vector<string> substr_v;
			_split(*it, substr_v);

			for(size_t i = 0; i < substr_v.size(); )
			{
				bool delt = false;
				if(find(vt.begin(), vt.end(), *(substr_v[i].begin())) == vt.end())
				{
					for(auto ok_set_it = okGNFProductionRulesSet.begin();
						ok_set_it != okGNFProductionRulesSet.end();
						++ok_set_it)
					{
						if(*(substr_v[i].begin()) == *((*ok_set_it).begin()))
						{
							string str = substr_v[i];
							str.erase(str.begin());
							vector<string> _substr_v;
							_split(*ok_set_it, _substr_v);
							for(size_t j = 0; j < _substr_v.size(); ++j)
							{
								substr_v.push_back(_substr_v[j] + str);
							}
							substr_v.erase(substr_v.begin() + i);
							delt = true;
							break;
						}
					}
				}
				if(!delt)
				{
					++i;
				}
			}

			string new_string;
			merge(new_string, '|', substr_v);
			new_string.insert(new_string.begin(), '>');
			new_string.insert(new_string.begin(), '-');
			new_string.insert(new_string.begin(), *((*it).begin()));

			if(checkGNFForm(new_string))
			{
				okGNFProductionRulesSet.insert(new_string);
			}
			else
			{
				_nonOKGNFProductionRulesSet.insert(new_string);
			}
		}
		setReplace(nonOKGNFProductionRulesSet, _nonOKGNFProductionRulesSet);
	}

	setReplace(p, okGNFProductionRulesSet);
	displayCFG();
}

void ContextFreeGrammar::simplifyCFG()
{
	set<string> 	old_p;
	set<char> 		old_vn;
	int i = 0;
	while(!setEqual(p, old_p) || !setEqual(vn, old_vn))
	{
		cout << "round " << i << endl;
		setReplace(old_p,  p);
		setReplace(old_vn, vn);
		eliminateUselessSymbolics1();
		eliminateUselessSymbolics2();
		eliminateEpsilonProductionRules();
		eliminateSingleProductionRules();
		++i;
	}
	toCNF();
	toGNF();
}