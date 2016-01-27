#include "pda.h"

DeltaRTuple::DeltaRTuple(const string& myNextState, const vector<string>& myStrToPush): nextState(myNextState), strToPush(myStrToPush)
{

}


PDA::PDA(const string& filePath)
{
	ifstream f(filePath);
	assert(f);

	// file format:
	// KSize K
	// SigmaSize Sigma
	// TauSize Tau
	// q0
	// Z0
	// FSize F
	// (realDeltaSize = KSize * (SigmaSize + 1) * TauSize)
	// DeltaSize Delta
	// curState inputSymbol stackTop DeltaRSetSize nextState strToPushSize strToPush[0] strToPush[1] ...
	//                                             nextState strToPushSize strToPush[0] strToPush[1] ...
	//                                             ...

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

	int TauSize;
	f >> TauSize;
	Tau.resize(TauSize);
	for(int i = 0; i < TauSize; ++i)
	{
		f >> Tau[i];
	}

	f >> q0 >> Z0;

	int FSize;
	f >> FSize;
	F.resize(FSize);
	for(int i = 0; i < FSize; ++i)
	{
		f >> F[i];
	}

	Delta.resize(KSize);
	for(int i = 0; i < KSize; ++i)
	{
		Delta[i].resize(SigmaSize + 1);
		for(int j = 0; j < SigmaSize + 1; ++j)
		{
			Delta[i][j].resize(TauSize);
		}
	}

	// read Delta
	int x = 0;
	int y = 0;
	int z = 0;
	int DeltaSize;
	string curState;
	string inputSymbol;
	string stackTop;
	int DeltaRSetSize;

	f >> DeltaSize;
	for(int i = 0; i < DeltaSize; ++i)
	{
		f >> curState;
		x = find(K.begin(), K.end(), curState) - K.begin();

		f >> inputSymbol;
		if(inputSymbol == "e")
		{
			y = SigmaSize;
		}
		else
		{
			y = find(Sigma.begin(), Sigma.end(), inputSymbol) - Sigma.begin();
		}

		f >> stackTop;
		z = find(Tau.begin(), Tau.end(), stackTop) - Tau.begin();

		f >> DeltaRSetSize;
		Delta[x][y][z].resize(DeltaRSetSize);
		for(int j = 0; j < DeltaRSetSize; ++j)
		{			
			string nextState;
			f >> nextState;

			int strToPushSize;
			f >> strToPushSize;
			vector<string> strToPush(strToPushSize);
			for(int k = 0; k < strToPushSize; ++k)
			{
				f >> strToPush[k];
			}

			Delta[x][y][z][j] = DeltaRTuple(nextState, strToPush);
		}
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

void PDA::display()
{
	cout << "PDA = (K,Sigma,Tau,Delta,q0,Z0,F)\n";

	cout << "K = ";
	displayVec(K);
	cout << endl;

	cout << "Sigma = ";
	displayVec(Sigma);
	cout << endl;

	cout << "Tau = ";
	displayVec(Tau);
	cout << endl;

	cout << "q0 = " << q0 << endl;

	cout << "Z0 = " << Z0 << endl;

	cout << "F = ";
	displayVec(F);
	cout << endl;

	cout << "Delta:\n";
	for(size_t i = 0; i < Delta.size(); ++i)
	{
		for(size_t j = 0; j < Delta[i].size(); ++j)
		{
			for(size_t k = 0; k < Delta[i][j].size(); ++k)
			{
				cout << "Delta(" << K[i] << ",";
				if(j == Sigma.size())
				{
					cout << "e";
				}
				else
				{
					cout << Sigma[j];
				}
				cout << "," << Tau[k] << ") = {";
				for(size_t l = 0; l < Delta[i][j][k].size(); ++l)
				{
					cout << "(" << Delta[i][j][k][l].nextState << ",";
					if(!Delta[i][j][k][l].strToPush.empty())
					{							
						for_each(Delta[i][j][k][l].strToPush.begin(), Delta[i][j][k][l].strToPush.end(),
							[](const string& str)
							{
								cout << str;
							});
					}
					else
					{
						cout << "e";
					}
					cout << ")";
					if(l != Delta[i][j][k].size() - 1)
					{
						cout << ",";
					}
				}
				cout << "}" << endl;
			}
		}
	}
}
