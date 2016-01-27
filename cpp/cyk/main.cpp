#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <vector>

#include "../grammar_str/grammar.h"
#include "../grammar_str/grammar.cpp"

using namespace std;

void cykVij(vector<vector< vector<string> >>& V, const vector<string>& x, const Grammar& G)
{
	for(size_t i = 0; i < x.size(); ++i)
	{
		for(size_t j = 0; j < G.P.size(); ++j)
		{
			for(size_t k = 0; k < G.P[j].rPartSet.size(); ++k)
			{
				if(G.P[j].rPartSet[k].size() == 1 &&
					G.P[j].rPartSet[k][0] == x[i])
				{
					V[i][0].push_back(G.P[j].lPart);
					break;
				}
			}
		}
	}
	ofstream f("./log.txt");
	assert(f);
	for(size_t j = 1; j < x.size(); ++j)
	{
		f << "j = " << j << endl;
		for(size_t i = 0; i < x.size() - j; ++i)
		{
			f << "\ti = " << i << endl;
			for(size_t k = 0; k <= j - 1; ++k)
			{
				f << "\t\tk = " << k << endl;
				for(size_t l = 0; l < G.P.size(); ++l)
				{
					f << "\t\t\tl = " << l << "(take #" << l << " production rule)" << endl;
					for(size_t m = 0; m < G.P[l].rPartSet.size(); ++m)
					{
						f << "\t\t\t\tm = " << m << "(take #" << m << " right part)" << endl;
						f << "\t\t\t\t\tproduction rule: " << G.P[l].lPart << "->";
						for_each(G.P[l].rPartSet[m].begin(), G.P[l].rPartSet[m].end(),
							[&f](const string& str)
							{
								f << str;
							});
						f << endl;
						f << "\t\t\t\t\tfind in V[" << i << "][" << k << "] and V[" << (i + k + 1) << "][" << (j - k - 1) << "]" << endl;
						if(G.P[l].rPartSet[m].size() == 2)
						{
							f << "\t\t\t\t\tlook for " << G.P[l].rPartSet[m][0] << " and " << G.P[l].rPartSet[m][1] << endl;
						}
						if(G.P[l].rPartSet[m].size() == 2 &&
							find(V[i][k].begin(), V[i][k].end(), G.P[l].rPartSet[m][0]) != V[i][k].end() &&
							find(V[i + k + 1][j - k - 1].begin(), V[i + k + 1][j - k - 1].end(), G.P[l].rPartSet[m][1]) != V[i + k + 1][j - k - 1].end())
						{
							if(find(V[i][j].begin(), V[i][j].end(), G.P[l].lPart) == V[i][j].end())
							{
								f << "\t\t\t\t\tadd " << G.P[l].lPart << " to V[" << i << "][" << j << "]" << endl;
								V[i][j].push_back(G.P[l].lPart);
							}
						}
					}
				}
			}
		}
	}
	f.close();
}

bool cyk(const vector<string>& x, const Grammar& G)
{
	vector<vector< vector<string> >> V;
	V.resize(x.size());
	for(size_t i = 0; i < x.size(); ++i)
	{
		V[i].resize(x.size() - i);
	}
	cykVij(V, x, G);
	return find(V[0][x.size() - 1].begin(), V[0][x.size() - 1].end(), "S") != V[0][x.size() - 1].end();
}

int main(int argc, char** argv)
{
	Grammar g("./grammar.txt");
	g.display();

	vector<string> x = {"b","a","a","b","a"};
	vector<vector< vector<string> >> V;
	V.resize(x.size());
	for(size_t i = 0; i < x.size(); ++i)
	{
		V[i].resize(x.size() - i);
	}
	cykVij(V, x, g);
	auto lambda = [](const string& str){ cout << str; };

	for(int i = 0; i < x.size(); ++i)
	{
		for(size_t j = 0; j < V[i].size(); ++j)
		{
			cout << "V[" << i << "][" << j << "] = ";
			displayVec(V[i][j]);
			cout << endl;
		}
	}	

	cout << "\""; for_each(x.begin(), x.end(), lambda); cout << "\"";
	cout << (cyk(x, g)? " is": "is not") << " in L(G)." << endl;
	
	return 0;
}