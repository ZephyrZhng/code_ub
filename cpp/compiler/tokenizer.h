#ifndef __TOKENIZER__
#define __TOKENIZER__

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <fstream>
#include <functional>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

namespace LEXEME
{
	enum Lexeme{ID, INTEGER_CONSTANT, REAL_CONSTANT, KEYWORD};
}

namespace TYPE
{
	enum Type{INTEGER, REAL, BOOLEAN};
}
	
namespace SYMBOL_CATEGORY
{
	enum SymbolCategory{VARIABLE};
}

using namespace LEXEME;
using namespace SYMBOL_CATEGORY;
using namespace TYPE;

class Token
{
public:
	Token() = default;
	Token(Lexeme l, int a): lexeme(l), attribute(a){}

	Lexeme lexeme;
	int attribute;

private:
};

class TypeListItem
{
public:
	TypeListItem() = default;
	TypeListItem(Type st, int stp): type(st), typeIndex(stp){}

	Type type;
	int typeIndex;

private:

};

class SymbolListItem
{
public:
	SymbolListItem() = default;
	SymbolListItem(string n, int t, SymbolCategory sc, int a):
		name(n), type(t), symbolCategory(sc), address(a) {}

	string name;
	int type;
	SymbolCategory symbolCategory;
	int address;

private:

};

class Tokenizer
{
public:
	Tokenizer();
	~Tokenizer();

	bool next(Token& t);

	bool isID(string s);
	bool isRealConstant(string s);
	bool isIntegerConstant(string s);
	bool isKeyword(string s);

	fstream f;

	vector<string> keywordList;
	vector<int> integerConstantList;
	vector<double> realConstantList;
	vector<SymbolListItem> symbolList;

	vector<TypeListItem> typeList;

	vector<Token> tokenSequence;

	void showIntegerConstantList();
	void showRealConstantList();
	void showSymbolList();

private:

};

#endif /*__TOKENIZER__*/