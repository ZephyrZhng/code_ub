#ifndef __LEXICAL_ANALYZER_H
#define __LEXICAL_ANALYZER_H

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

#include "../trie/trie.h"

using namespace std;

class Token
{
public:
	Token() = default;
	Token(int myType, int myVal);
	~Token() = default;

	int type;
	int val;

private:

};

class LexicalAnalyzer
{
public:
	LexicalAnalyzer() = default;
	LexicalAnalyzer(const string& keywordFilePath, const string& symbolFilePath);
	~LexicalAnalyzer() = default;

	void showKeywordAndSymbol();

	void LexicalAnalyze(const string& srcCodeFilePath);

	void readID(FILE* fp, char& ch);
	void readCh(FILE* fp, char& ch);
	void readStr(FILE* fp, char& ch);
	void readConst(FILE* fp, char& ch);
	void readSymbol(FILE* fp, char& ch);
	void readSpaceTabNewline(FILE* fp, char& ch);

	void showTokenSeq();
	void showIDTab();
	void showChTab();
	void showStrTab();
	void shiowConstTab();
	void showKeywordTab();
	void showSymbolTab();

private:
	vector<Token> tokenSeq;

	vector<string> idTab;
	// (0, index in idTab)

	vector<char> chTab; // dynamic
	// pay attention to \n \t \\ \' \"
	// (1, index in chTab)
	vector<string> strTab; // dynamic
	// (2, index in strTab)
	vector<double> constTab; // dynamic
	// (3, index in constTab)
	
	vector<string> keywordTab; // static
	// (index in keywordTab + 4, 0)
	// read it in readID function
	vector<string> symbolTab; // static
	// (index in symbolTab + keywordTab.size() + 4, 0)
	// read it in default condition

	Trie keywordTrie;
	Trie symbolTrie;
};

#endif /*__LEXICAL_ANALYZER_H*/