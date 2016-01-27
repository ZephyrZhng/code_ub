#include "nfa.h"

void NFA::display()
{
	auto lambda = [](string str){cout << str << " ";};
	cout << "s: ";
	for_each(s.begin(), s.end(), lambda);
	cout << endl;
	cout << "sigma: ";
	for_each(sigma.begin(), sigma.end(), lambda);
	cout << endl;
	cout << "delta:" << endl;
	for_each(delta.begin(), delta.end(), 
		[lambda](vector<vector<string>> vv)
		{
			for_each(vv.begin(), vv.end(), 
				[lambda](vector<string> v)
				{
					for_each(v.begin(), v.end(), lambda);
					cout << endl;
				});
		});
}

int NFA::get_s_num()
{
	return s_num;
}

int NFA::get_sigma_num()
{
	return sigma_num;
}