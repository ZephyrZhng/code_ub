#include "epsilon_nfa.h"

EpsilonNFA::EpsilonNFA()
{
	ifstream f("./epsilon_nfa.txt");

	string str;
	vector<string> vec;
	int num;
	f >> s_num;
	for(int i = 0; i < s_num; ++i)
	{
		f >> str;
		s.push_back(str);
	}
	f >> sigma_num;
	for(int i = 0; i < sigma_num; ++i)
	{
		f >> str;
		sigma.push_back(str);
	}
	delta.resize(s_num);
	for(int i = 0; i < s_num; ++i)
	{
		delta[i].resize(sigma_num + 1);
		for(int j = 0; j < delta[i].size(); ++j)
		{
			f >> num;
			for(int k = 0; k < num; ++k)
			{
				f >> str;
				delta[i][j].push_back(str);
			}
		}
	}

	f.close();
}

void EpsilonNFA::display()
{
	auto lambda = [](string str){cout << str << " ";};
	cout << "s: ";
	for_each(s.begin(), s.end(), lambda);
	cout << endl;
	cout << "sigma: ";
	for_each(sigma.begin(), sigma.end(), lambda);
	cout << endl;
	cout << "delta:" << endl;
	// for_each(delta.begin(), delta.end(), 
	// 	[lambda](vector<vector<string>> vv)
	// 	{
	// 		for_each(vv.begin(), vv.end(), 
	// 			[lambda](vector<string> v)
	// 			{
	// 				for_each(v.begin(), v.end(), lambda);
	// 				cout << endl;
	// 			});
	// 	});
	for(auto i = delta.begin(); i != delta.end(); ++i)
	{
		for(auto j = (*i).begin(); j != (*i).end(); ++j)
		{
			cout << "( " << (i - delta.begin()) << ", " << (j - (*i).begin()) << "): ";
			for(auto& k: *j)
			{
				cout << k << " ";
			}
			cout << endl;
		}
	}
}

int EpsilonNFA::get_s_num()
{
	return s_num;
}

int EpsilonNFA::get_sigma_num()
{
	return sigma_num;
}

vector<string> EpsilonNFA::delta_x_y(int x, int y)
{
	return delta[x][y];
}

void EpsilonNFA::set_delta_x_y(int x, int y, const vector<string>& _delta_x_y)
{
	delta[x][y].clear();
	copy(_delta_x_y.begin(), _delta_x_y.end(), delta[x][y].begin());
}