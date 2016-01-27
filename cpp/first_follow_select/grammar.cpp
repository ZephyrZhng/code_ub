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
void set_union(set<T>& s1, const set<T>& s2)
{
	for(auto& p: s2)
	{
		s1.insert(p);
	}
}

template<typename T>
bool set_contain(const set<T>& s, const T& t)
{
	return find(s.begin(), s.end(), t) != s.end();
}

PPart::PPart()
{

}

PPart::PPart(const string& l, const vector<string>& r): LPart(l), RPart(r)
{

}

Grammar::Grammar()
{
	ifstream f("./grammar.txt");
	assert(f);

	int VN_size;
	f >> VN_size;
	VN.resize(VN_size);
	for(int i = 0; i < VN_size; ++i)
	{
		f >> VN[i];
	}

	int VT_size;
	f >> VT_size;
	VT.resize(VT_size);
	for(int i = 0; i < VT_size; ++i)
	{
		f >> VT[i];
	}

	int P_size;
	f >> P_size;
	RawP.resize(P_size);
	P.resize(P_size);
	for(int i = 0; i < P_size; ++i)
	{
		f >> RawP[i];

		// deal with P
		vector<string> Pi = split(RawP[i], '-');
		Pi[1].erase(Pi[1].begin());
		vector<string> Pir = split(Pi[1], '|');
		P[i] = PPart(Pi[0], Pir);
	}

	f >> S;

	f.close();
}

Grammar::~Grammar()
{

}

void Grammar::display()
{
	auto lambda_ch = [](const char& ch){ cout << ch << " "; };
	auto lambda_str = [](const string& str){ cout << str << endl; };
	auto lambda_p_r = [](const string& str){ cout << str << " "; };
	auto lambda_p = [lambda_p_r](const PPart& pp)
	{
		cout << "LPart: " << pp.LPart << " " << "RPart: ";
		for_each(pp.RPart.begin(), pp.RPart.end(), lambda_p_r);
		cout << endl;
	};

	cout << "G:\n";
		
	cout << "VN: ";
	for_each(VN.begin(), VN.end(), lambda_ch);
	cout << endl;

	cout << "VT: ";
	for_each(VT.begin(), VT.end(), lambda_ch);
	cout << endl;

	cout << "raw P:\n";
	for_each(RawP.begin(), RawP.end(), lambda_str);

	cout << "dealt P:\n";
	for_each(P.begin(), P.end(), lambda_p);

	cout << "S: " << S << endl;
}

bool Grammar::terminal(char x)
{
	return find(VT.begin(), VT.end(), x) != VT.end();
}

bool Grammar::nonterminal(char x)
{
	return find(VN.begin(), VN.end(), x) != VN.end();
}

void Grammar::first(set<char>& firstx, char x)
{
	if(terminal(x))
	{
		firstx.clear();
		firstx.insert(x);
		return;
	}
	else
	{
		for(size_t ip = 0; ip < P.size(); ++ip)
		{
			if(P[ip].LPart.size() == 1 && *(P[ip].LPart.begin()) == x)
			{
				for(size_t ipr = 0; ipr < P[ip].RPart.size(); ++ipr)
				{
					if(P[ip].RPart[ipr] != "e")
					{
						int i = 0;
						for(auto p = P[ip].RPart[ipr].begin(); p != P[ip].RPart[ipr].end(); ++p)
						{
							set<char> firstyi;
							first(firstyi, *p);
							auto ep = find(firstyi.begin(), firstyi.end(), 'e');
							if(ep != firstyi.end())
							{
								firstyi.erase(ep);
								++i;
								set_union(firstx, firstyi);
							}
							else
							{
								set_union(firstx, firstyi);
								break;
							}
						}
						if(i == P[ip].RPart[ipr].size())
						{
							firstx.insert('e');
						}
					}
					else
					{
						firstx.insert('e');
					}
				}
			}
		}
	}
}

// iteration of first() function

void Grammar::follow(set<char>& followB, char B)
{
	if(B == S)
	{
		followB.insert('$');
	}
	for(size_t ip = 0; ip < P.size(); ++ip)
	{
		for(size_t ipr = 0; ipr < P[ip].RPart.size(); ++ipr)
		{
			auto p = find(P[ip].RPart[ipr].begin(), P[ip].RPart[ipr].end(), B);
			if(p != P[ip].RPart[ipr].end())
			{
				if(p != P[ip].RPart[ipr].end() - 1)
				{
					set<char> first_beta;
					first(first_beta, *(p + 1));
					auto ep = find(first_beta.begin(), first_beta.end(), 'e');
					if(ep != first_beta.end())
					{
						first_beta.erase(ep);
						set_union(followB, first_beta);

						set<char> followA;
						follow(followA, *(P[ip].LPart.begin()));
						set_union(followB, followA);
					}
					else
					{
						set_union(followB, first_beta);
					}
				}
				else
				{
					set<char> followA;
					follow(followA, *(P[ip].LPart.begin()));
					set_union(followB, followA);
				}
			}
		}
	}
}

// Segmentation fault (core dumped)

vector<char> Grammar::getVN()
{
	return VN;
}

vector<char> Grammar::getVT()
{
	return VT;
}

