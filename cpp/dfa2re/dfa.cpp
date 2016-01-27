#include "dfa.h"

DFA::DFA()
{
	ifstream f("./dfa.txt");
	assert(f);

	string str;
	vector<string> vec;

	f >> KSize;
	K.push_back(string());
	for(int i = 0; i < KSize; ++i)
	{
		f >> str;
		K.push_back(str);
	}

	f >> SigmaSize;
	for(int i = 0; i < SigmaSize; ++i)
	{
		f >> str;
		Sigma.push_back(str);
	}

	f >> S;

	f >> FSize;
	for(int i = 0; i < FSize; ++i)
	{
		f >> str;
		F.push_back(str);
	}

	Delta.resize(KSize + 1);
	for(int i = 1; i <= KSize; ++i)
	{
		Delta[i].resize(SigmaSize);
		for(int j = 0; j < SigmaSize; ++j)
		{
			f >> Delta[i][j];
		}
	}

	f.close();
}

DFA::DFA(const DFA& dfa):
	Delta(dfa.Delta), K(dfa.K), Sigma(dfa.Sigma), S(dfa.S), F(dfa.F), 
	KSize(dfa.KSize), SigmaSize(dfa.SigmaSize), FSize(dfa.FSize)
{

}

string DFA::delta_x_y(int x, int y)
{
	return Delta[x][y];
}

void DFA::display()
{
	auto lambda = [](string str){cout << str << " ";};

	cout << "K:\t";
	for_each(K.begin() + 1, K.end(), lambda);
	cout << endl;

	cout << "Sigma:\t";
	for_each(Sigma.begin(), Sigma.end(), lambda);
	cout << endl;

	cout << "S:\t" << S << endl;

	cout << "F:\t";
	for_each(F.begin(), F.end(), lambda);
	cout << endl;

	cout << "Delta:\n\t";
	for_each(Sigma.begin(), Sigma.end(),
		[](string str)
		{
			cout << str << "\t";
		});
	cout << endl;
	for(auto i = Delta.begin() + 1; i != Delta.end(); ++i)
	{
		cout << K[i - Delta.begin()] << "\t";
		for_each((*i).begin(), (*i).end(),
			[](string str)
			{
				cout << str << "\t";
			});
		cout << endl;
	}
}

vector<string> DFA::get_K()
{
	return K;
}

vector<string> DFA::get_Sigma()
{
	return Sigma;
}

string DFA::get_S()
{
	return S;
}

vector<string> DFA::get_F()
{
	return F;
}

int DFA::get_KSize()
{
	return KSize;
}

int DFA::get_SigmaSize()
{
	return SigmaSize;
}

int DFA::get_FSize()
{
	return FSize;
}