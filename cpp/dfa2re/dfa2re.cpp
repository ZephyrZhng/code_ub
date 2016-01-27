#include "dfa2re.h"

DFA2REConverter::DFA2REConverter(const DFA& _dfa): dfa(_dfa)
{
	// r[k][i][j]
	int n = dfa.get_KSize();
	r.resize(n + 1); // 0 <= k <= n
	for(int i = 0; i <= n; ++i)
	{
		r[i].resize(n + 1); // 1 <= i <= n
		for(int j = 0; j <= n; ++j)
		{
			r[i][j].resize(n + 1); // 1 <= j <= n
		}
	}
}

vector<string> DFA2REConverter::get_edge_from_qi_to_qj(int i, int j)
{
	vector<string> ret;
	int SigmaSize = dfa.get_SigmaSize();
	vector<string> K = dfa.get_K();
	vector<string> Sigma = dfa.get_Sigma();
	for(int col = 0; col < SigmaSize; ++col)
	{
		if(dfa.delta_x_y(i, col) == K[j])
		{
			ret.push_back(Sigma[col]);
		}
	}
	return ret;
}

bool DFA2REConverter::check_concatenate(string re)
{
	int match = 0;
	for(auto i = re.begin(); i != re.end(); ++i)
	{
		if(*i == '(')
		{
			++match;
		}
		else if(*i == ')')
		{
			--match;
		}
		if(*i == '+' && match == 0)
		{
			return false;
		}
	}
	return true;
}

string DFA2REConverter::simplify_closure(string re)
{
	if(re == "phi" || re == "epsilon")
	{
		return "epsilon";
	}
	else if(re.size() == 1)
	{
		return re + "*";
	}
	else
	{
		return "(" + re + ")*";
	}
}

string DFA2REConverter::simplify_concatenate(string re1, string re2)
{
	if(re1 == "epsilon")
	{
		return re2;
	}
	else if(re2 == "epsilon")
	{
		return re1;
	}
	else if(re1 == "phi" || re2 == "phi")
	{
		return "phi";
	}
	else
	{
		if(!check_concatenate(re1))
		{
			re1 = "(" + re1 + ")";
		}
		if(!check_concatenate(re2))
		{
			re2 = "(" + re2 + ")";
		}
		return re1 + re2;
	}
}

string DFA2REConverter::simplify_union(string re1, string re2)
{
	if(re1 == "phi")
	{
		return re2;
	}
	else if(re2 == "phi")
	{
		return re1;
	}
	else if(re1 == re2)
	{
		return re1;
	}
	else
	{
		return re1 + "+" + re2;
	}
}

string DFA2REConverter::r_k_i_j(int k, int i, int j)
{
	if(k == 0)
	{
		string ret;
		vector<string> r_0_i_j = get_edge_from_qi_to_qj(i, j);
		if(i != j)
		{
			if(r_0_i_j.empty())
			{
				ret = "phi";
			}
			else
			{
				ret = r_0_i_j[0];
				for_each(r_0_i_j.begin() + 1, r_0_i_j.end(),
					[&ret](string str)
					{
						ret += "+";
						ret += str;
					});
			}
		}
		else // i == j
		{
			ret = "epsilon";
			for_each(r_0_i_j.begin(), r_0_i_j.end(),
				[&ret](string str)
				{
					ret += "+";
					ret += str;
				});
		}
		return ret;
	}
	else // k != 0
	{
		string re_closure = simplify_closure(r_k_i_j(k - 1, k, k));
		string re_concatenate = simplify_concatenate(simplify_concatenate(r_k_i_j(k - 1, i, k), re_closure), r_k_i_j(k - 1, k, j));
		string re_union = simplify_union(re_concatenate, r_k_i_j(k - 1, i, j));
		return re_union;
	}
}

string DFA2REConverter::to_re()
{
	int n = dfa.get_KSize();

	for(int k = 0; k <= n; ++k)
	{
		for(int i = 1; i <= n; ++i)
		{
			for(int j = 1; j <= n; ++j)
			{
				r[k][i][j] = r_k_i_j(k, i, j);
			}
		}
	}

	// for(int k = 0; k <= n; ++k)
	// {
	// 	for(int i = 1; i <= n; ++i)
	// 	{
	// 		for(int j = 1; j <= n; ++j)
	// 		{
	// 			cout << "(" << k << ", " << i << ", " << j << "): ";
	// 			cout << r[k][i][j] << endl;
	// 		}
	// 	}
	// }

	/*test get_edge_from_qi_to_qj()*/
	// vector<string> vec = get_edge_from_qi_to_qj(3, 2);
	// for_each(vec.begin(), vec.end(),
	// 	[](string str)
	// 	{
	// 		cout << str << " ";
	// 	});
	// cout << endl;

	string ret;
	int FSize = dfa.get_FSize();
	vector<string> K = dfa.get_K();
	vector<string> F = dfa.get_F();
	int S_index = find(K.begin(), K.end(), dfa.get_S()) - K.begin();

	ret = r[n][S_index][find(K.begin(), K.end(), F[0]) - K.begin()];
	for(int i = 1; i < FSize; ++i)
	{
		ret = simplify_union(ret, r[n][S_index][find(K.begin(), K.end(), F[i]) - K.begin()]);
	}

	return ret;
}