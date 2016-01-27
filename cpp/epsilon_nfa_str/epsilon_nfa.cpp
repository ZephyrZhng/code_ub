#include "epsilon_nfa.h"

EpsilonNFA::EpsilonNFA(const string& filePath)
{
	ifstream f(filePath);
	assert(f);

	// file format:
	// KSize K
	// SigmaSize Sigma
	// (DeltaSize = KSize * (SigmaSize + 1), the last col is epsilon col)
	// n ...
	// n ...
	// ...
	// q0
	// FSize F

	int KSize;
	f >> KSize;
	K.resize(KSize);
	for(int i = 0; i < KSize; ++i)
	{
		f >> K[i];
	}

	int SigmaSize;
	f >> SigmaSize;
	Sigma.resize(SigmaSize);
	for(int i = 0; i < SigmaSize; ++i)
	{
		f >> Sigma[i];
	}

	int n;
	Delta.resize(KSize);
	for(int i = 0; i < KSize; ++i)
	{
		Delta[i].resize(SigmaSize + 1);
		for(int j = 0; j < SigmaSize + 1; ++j)
		{
			f >> n;
			Delta[i][j].resize(n);
			for(int k = 0; k < n; ++k)
			{
				f >> Delta[i][j][k];
			}
		}
	}

	f >> q0;

	int FSize;
	f >> FSize;
	F.resize(FSize);
	for(int i = 0; i < FSize; ++i)
	{
		f >> F[i];
	}

	f.close();
}

template<typename T>
void displayVec(const vector<T>& vec)
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

void EpsilonNFA::display()
{
	cout << "e-NFA = (K,Sigma,Delta,q0,F)\n";

	auto lambda = [](const string& str){ cout << str << " "; };

	cout << "K = ";
	displayVec(K);
	cout << endl;

	cout << "Sigma = ";
	displayVec(Sigma);
	cout << endl;

	cout << "Delta:\n";
	for(size_t i = 0; i < Delta.size(); ++i)
	{
		for(size_t j = 0; j < Delta[i].size(); ++j)
		{
			cout << "Delta(" << K[i] << "," << ((j <= Delta[i].size() - 2)? Sigma[j]: "e") << ") = ";
			displayVec(Delta[i][j]);
			cout << "\n";
		}
	}

	cout << "q0 = " << q0 << endl;

	cout << "F = ";
	displayVec(F);
	cout << endl;
}