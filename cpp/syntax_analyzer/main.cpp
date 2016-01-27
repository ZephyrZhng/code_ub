#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <vector>

#include "syntax_analyzer.h"

int main(int argc, char** argv)
{
	SyntaxAnalyzer SyntaxAnalyzer("../grammar_ch/grammar.txt");

	SyntaxAnalyzer.buildLL1ParseTab(); cout << endl;
	SyntaxAnalyzer.ll1SyntaxAnalyze("0+(0+0*(0+(0+0+0*0*0)*0+0+(0+0)+0)+0)*0"); cout << endl;
	SyntaxAnalyzer.ll1SyntaxAnalyze("0+(0+0*(0+(0+0+0*0*0)*0+0+(0+0)+0)+0)0"); cout << endl;

	SyntaxAnalyzer.calLR1ItemSetFamily(); cout << endl;
	SyntaxAnalyzer.buildLR1ParseTab(); cout << endl;
	// SyntaxAnalyzer.lr1SyntaxAnalyze("0+(0+0*(0+(0+0+0*0*0)*0+0+(0+0)+0)+0)*0"); cout << endl;
	// SyntaxAnalyzer.lr1SyntaxAnalyze("0+(0+0*(0+(0+0+0*0*0)*0+0+(0+0)+0)+0)0"); cout << endl;

	// SyntaxAnalyzer.calLR0ItemSetFamily(); cout << endl;
	// SyntaxAnalyzer.buildLR0ParseTab(); cout << endl;
	// SyntaxAnalyzer.lr0SyntaxAnalyze("0+(0+0*(0+(0+0+0*0*0)*0+0+(0+0)+0)+0)*0"); cout << endl;
	// SyntaxAnalyzer.lr0SyntaxAnalyze("0+(0+0*(0+(0+0+0*0*0)*0+0+(0+0)+0)+0)0"); cout << endl;

	// Z->AA
	//  ->AaA
	//  ->aAaA
	//  ->aAaaA
	//  ->abaaA
	//  ->abaab

	return 0;
}