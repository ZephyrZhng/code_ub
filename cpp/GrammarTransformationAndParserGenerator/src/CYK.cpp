#include "CYK.h"

bool CYKParser::parse(const vector<string>& s)
{
	g.putInCNF();

	vector<string> V = g.getV();
	string S = g.getS();
	int n = s.size();
	int r = V.size();

	vector<vector<vector<bool>>> P(n, vector<vector<bool>>(n, vector<bool>(r, false)));


}