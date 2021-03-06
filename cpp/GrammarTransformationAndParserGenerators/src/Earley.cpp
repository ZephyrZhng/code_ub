#include "Earley.h"
#include "utility.h"

EarleyState::EarleyState(unsigned int pi, unsigned int dp, unsigned int op)
{
	productionIndex = pi;
	dotPosition = dp;
	originPosition = op;
}

bool operator ==(const EarleyState& l, const EarleyState& r)
{
	return l.productionIndex == r.productionIndex
		&& l.dotPosition == r.dotPosition
		&& l.originPosition == r.originPosition;
}

EarleyParser::EarleyParser(const CFG& cfg)
{
	g = cfg;
}

void EarleyParser::predict(const EarleyState& s, int k)
{
	Production pr = g.p[s.productionIndex];
	string Y = pr.right[s.dotPosition];

	for(size_t i = 0; i < g.p.size(); ++i)
	{
		if(g.p[i].left == Y)
		{
			add(S[k], {EarleyState(i, 0, k)});
		}
	}
}

void EarleyParser::scan(const EarleyState& s, const vector<string>& w, int k)
{
	Production pr = g.p[s.productionIndex];
	string a = pr.right[s.dotPosition];
	int j = s.originPosition;

	if(a == w[k])
	{
		add(S[k + 1], {EarleyState(s.productionIndex, s.dotPosition + 1, j)});
	}
}

void EarleyParser::complete(const EarleyState& s, int k)
{
	int j = s.originPosition;
	Production prX = g.p[s.productionIndex];
	string X = prX.left;

	for(size_t i = 0; i < S[j].size(); ++i)
	{
		EarleyState sY = S[j][i];
		Production prY = g.p[sY.productionIndex];

		if(sY.dotPosition < prY.right.size()
			&& prY.right[sY.dotPosition] == X)
		{
			add(S[k], {EarleyState(sY.productionIndex, sY.dotPosition + 1, sY.originPosition)});
		}
	}
}

void EarleyParser::parse(const vector<string>& w)
{
	S.clear();
	g.augmentGrammar();
	S.assign(w.size() + 1, vector<EarleyState>());
	add(S[0], {EarleyState(g.p.size() - 1, 0, 0)});

	for(size_t k = 0; k <= w.size(); ++k)
	{
		for(size_t i = 0; i < S[k].size(); ++i)
		{
			cout << "(" << k << ", " << i << ") ";
			EarleyState s = S[k][i];
			Production pr = g.p[s.productionIndex];

			if(s.dotPosition < pr.right.size())
			{
				if(in(pr.right[s.dotPosition], g.v))
				{
					predict(s, k);
					cout << "predict" << endl;
				}
				else if(k < w.size())
				{
					scan(s, w, k);
					cout << "scan" << endl;
				}
				else
				{
					cout << "fuck" << endl;
				}
			}
			else
			{
				complete(s, k);
				cout << "complete" << endl;
			}
		}
	}
}

void EarleyParser::displayS(ostream& os)
{
	os << "S = {" << endl;

	for(size_t i = 0; i < S.size(); ++i)
	{
		for(size_t j = 0; j < S[i].size(); ++j)
		{
			os << "\t(" << i << ", " << j << ") " << "(";

			Production pr = g.p[S[i][j].productionIndex];
			os << pr.left << " -> ";
			for(size_t k = 0; k < pr.right.size(); ++k)
			{
				if(k == S[i][j].dotPosition)
				{
					os << ". ";
				}
				os << pr.right[k] << " ";
			}
			if(S[i][j].dotPosition == pr.right.size())
			{
				os << ". ";
			}
			os << ", " << S[i][j].originPosition << ")" << endl;
		}
	}

	os << "}" << endl << endl;
}