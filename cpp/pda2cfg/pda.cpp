#include "pda.h"
#include "number.h"
#include "production_rules.h"

delta_r_tuple::delta_r_tuple()
{

}

delta_r_tuple::delta_r_tuple(const char& s, const string& str): state(s), strToPush(str)
{

}

PDA::PDA()
{
	ifstream f("./pda.txt");
	assert(f);

	int K_size;
	f >> K_size;
	K.resize(K_size);
	for(int i = 0; i < K_size; ++i)
	{
		f >> K[i];
	}

	int Sigma_size;
	f >> Sigma_size;
	Sigma.resize(Sigma_size);
	for(int i = 0; i < Sigma_size; ++i)
	{
		f >> Sigma[i];
	}

	int Tau_size;
	f >> Tau_size;
	Tau.resize(Tau_size);
	for(int i = 0; i < Tau_size; ++i)
	{
		f >> Tau[i];
	}

	f >> q0 >> Z0;

	int F_size;
	f >> F_size;
	F.resize(F_size);
	for(int i = 0; i < F_size; ++i)
	{
		f >> F[i];
	}

	delta.resize(K_size);
	for(int i = 0; i < K_size; ++i)
	{
		delta[i].resize(Sigma_size + 1);
		for(int j = 0; j < Sigma_size + 1; ++j)
		{
			delta[i][j].resize(Tau_size);
		}
	}

	// read delta
	int x = 0;
	int y = 0;
	int z = 0;
	int delta_size;
	int delta_r_set_size;
	char ch;
	string str;

	f >> delta_size;
	for(int i = 0; i < delta_size; ++i)
	{
		f >> ch;
		x = find(K.begin(), K.end(), ch) - K.begin();
		f >> ch;
		if(ch == 'e')
		{
			y = Sigma_size;
		}
		else
		{
			y = find(Sigma.begin(), Sigma.end(), ch) - Sigma.begin();
		}
		f >> ch;
		z = find(Tau.begin(), Tau.end(), ch) - Tau.begin();

		f >> delta_r_set_size;
		for(int j = 0; j < delta_r_set_size; ++j)
		{
			f >> ch >> str;
			delta[x][y][z].push_back(delta_r_tuple(ch, str));
		}
	}

	f.close();
}

void PDA::display()
{
	auto lambda = [](const char& ch){ cout << ch << " "; };

	cout << "K: ";
	for_each(K.begin(), K.end(), lambda);
	cout << endl;

	cout << "Sigma: ";
	for_each(Sigma.begin(), Sigma.end(), lambda);
	cout << endl;

	cout << "Tau: ";
	for_each(Tau.begin(), Tau.end(), lambda);
	cout << endl;

	cout << "q0: " << q0 << endl << "Z0: " << Z0 << endl;

	cout << "F: ";
	if(F.empty())
	{
		cout << "phi" << endl;
	}
	else
	{
		for_each(F.begin(), F.end(), lambda);
		cout << endl;
	}

	cout << "delta:\n";
	for(size_t i = 0; i < delta.size(); ++i)
	{
		for(size_t j = 0; j < delta[i].size(); ++j)
		{
			for(size_t k = 0; k < delta[i][j].size(); ++k)
			{
				cout << "delta(" << K[i] << ",";
				if(j == Sigma.size())
				{
					cout << "e";
				}
				else
				{
					cout << Sigma[j];
				}
				cout << "," << Tau[k] << ")={";
				for(size_t l = 0; l < delta[i][j][k].size(); ++l)
				{
					cout << "(" << delta[i][j][k][l].state << "," << delta[i][j][k][l].strToPush << ")";
					if(l != delta[i][j][k].size() - 1)
					{
						cout << ",";
					}
				}
				cout << "}" << endl;
			}
		}
	}
}

string ch2str(char ch)
{
	string str;
	str.insert(str.begin(), ch);
	return str;
}

Grammar PDA::toCFG()
{
	Grammar CFG;
	vector<production_rules> P;

	for(int i = 0; i < K.size(); ++i)
	{
		string s1 = ch2str(q0);
		string s2 = ch2str(Z0);
		string s3 = ch2str(K[i]);
		P.push_back(production_rules("S", char(), vector<string>({ s1 + s2 + s3 })));
		
		// cout << i << ". ";
		// production_rules pp("S", char(), vector<string>({ s1 + s2 + s3 }));
		// cout << "lPart: " << pp.lPart << " rInputSymbol: " << pp.rInputSymbol << " ";
		// cout << "rStates: ";
		// for_each(pp.rStates.begin(), pp.rStates.end(),
		// 	[](const string& str)
		// 	{
		// 		cout << str << " ";
		// 	});
		// cout << endl;
	}

	for(size_t i = 0; i < delta.size(); ++i)
	{
		for(size_t j = 0; j < delta[i].size(); ++j)
		{
			for(size_t k = 0; k < delta[i][j].size(); ++k)
			{
				if(!delta[i][j][k].empty())
				{
					for(size_t l = 0; l < delta[i][j][k].size(); ++l)
					{
						string q = ch2str(K[i]);
						char a = Sigma[j];
						string A = ch2str(Tau[k]);

						char pda_r_state = delta[i][j][k][l].state;
						string pda_r_strToPush = delta[i][j][k][l].strToPush;
						int len = pda_r_strToPush.size();

						if(pda_r_strToPush == "e")
						{
							production_rules pr;
							pr.lPart = q + A + pda_r_state;
							pr.rInputSymbol = (a == '\0')? 'e': a;
							P.push_back(pr);
							break;
						}

						for(size_t m = 0; m < K.size(); ++m)
						{
							vector<string> cfg_rStates;
							production_rules pr;
							string cfg_lPart = q + A + ch2str(K[m]);

							Number n(K.size());
							n.setLen(len - 1);
							vector<vector<int>> matrix;

							bool b = false;
							for(;;)
							{
								matrix.push_back(n.num);

								// for_each(n.num.begin(), n.num.end(), [](int i){cout<<i;}); cout << endl;

								if(n.isMax())
								{
									b = true;
								}
								++n;

								if(b)
								{
									break;
								}
							}

							for(size_t row = 0; row < matrix.size(); ++row)
							{
								cfg_rStates.clear();
								char cfg_rInputSymbol = (a == '\0')? 'e': a;
								for(int col = 0; col < len; ++col)
								{
									string cfg_rStates_elem;
									if(col == 0)
									{
										cfg_rStates_elem += ch2str(pda_r_state);
									}
									else
									{
										cfg_rStates_elem += K[matrix[row][col - 1]];
									}
									cfg_rStates_elem += ch2str(*(pda_r_strToPush.begin() + col));
									if(col == len - 1)
									{
										cfg_rStates_elem += ch2str(K[m]);
									}
									else
									{										
										cfg_rStates_elem += ch2str(K[matrix[row][col]]);
									}									
									cfg_rStates.push_back(cfg_rStates_elem);
								}
								pr.lPart = cfg_lPart;
								pr.rInputSymbol = cfg_rInputSymbol;
								pr.rStates = cfg_rStates;
								P.push_back(pr);
							}
						}
					}
				}
			}
		}
	}

	for(size_t i = 0; i < P.size(); ++i)
	{
		cout << P[i].lPart << "->" << P[i].rInputSymbol;
		for_each(P[i].rStates.begin(), P[i].rStates.end(),
			[](const string& str)
			{
				cout << "[" << str << "]";
			});
		cout << endl;
	}

	return CFG;
}

// TODO: simplify CFG