#include "grammar.h"

ProductionRule::ProductionRule()
{

}

ProductionRule::ProductionRule(const string& l, const vector<rPart>& r): lPart(l), rPartSet(r)
{

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

	int PSize;
	f >> PSize;
	P.resize(PSize);
	int rPartSetSize;
	int rPartSize;
	for(int i = 0; i < PSize; ++i)
	{
		ProductionRule pr;
		f >> pr.lPart;
		f >> rPartSetSize;
		pr.rPartSet.resize(rPartSetSize);
		for(int j = 0; j < rPartSetSize; ++j)
		{
			f >> rPartSize;
			pr.rPartSet[j].resize(rPartSize);
			for(int k = 0; k < rPartSize; ++k)
			{
				f >> pr.rPartSet[j][k];
			}
		}
		P[i] = pr;
	}

	f >> S;

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

void Grammar::display()
{
	cout << "G = (VN,VT,P,S)\n";

	auto lambda = [](const string& str){ cout << str; };

	cout << "VN = ";
	displayVec(VN);
	cout << endl;

	cout << "VT = ";
	displayVec(VT);
	cout << endl;

	cout << "P:" << endl;
	for(size_t i = 0; i < P.size(); ++i)
	{
		cout << P[i].lPart << "->";
		for(size_t j = 0; j < P[i].rPartSet.size(); ++j)
		{
			for_each(P[i].rPartSet[j].begin(), P[i].rPartSet[j].end(), lambda);
			if(j != P[i].rPartSet.size() - 1)
			{
				cout << "|";
			}
		}
		cout << endl;
	}

	cout << "S = " << S << endl;
}