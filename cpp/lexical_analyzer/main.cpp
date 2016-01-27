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

#include "lexical_analyzer.h"

using namespace std;

int main(int argc, char** argv)
{
	LexicalAnalyzer LexicalAnalyzer("./keyword.txt", "./symbol.txt");
	LexicalAnalyzer.showKeywordAndSymbol();

	LexicalAnalyzer.LexicalAnalyze("./test.cpp");

	LexicalAnalyzer.showTokenSeq();
	LexicalAnalyzer.showIDTab();
	LexicalAnalyzer.showChTab();
	LexicalAnalyzer.showStrTab();
	LexicalAnalyzer.showKeywordTab();
	LexicalAnalyzer.showSymbolTab();

	return 0;
}