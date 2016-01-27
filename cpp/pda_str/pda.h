#ifndef __PDA_H
#define __PDA_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <vector>

using namespace std;

class DeltaRTuple
{
public:
	DeltaRTuple() = default;
	DeltaRTuple(const string& myNextState, const vector<string>& myStrToPush);
	~DeltaRTuple() = default;

	string nextState;
	vector<string> strToPush;

private:

};

typedef vector<DeltaRTuple> DeltaRSet;

class PDA
{
public:
	PDA() = default;
	PDA(const string& filePath);
	~PDA() = default;
	void display();

	vector<string> K;
	vector<string> Sigma;
	vector<string> Tau;
	string q0;
	string Z0;
	vector<string> F;
	vector<vector<vector<DeltaRSet>>> Delta;

private:

};

#endif /*__PDA_H*/