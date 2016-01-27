#ifndef __PARSER__
#define __PARSER__

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

#include "tokenizer.h"

using namespace std;

namespace OPERATION
{
	enum Operation{ADD, SUBTRACT, MULTIPLY, DIVIDE, ASSIGN, SET, WHILE, WHILE_BEGIN, WHILE_END, THEN, ELSE, IF_END};
};

namespace ATTRIBUTE
{
	enum Attribute{INTEGER_NUMBER, REAL_NUMBER, BOOLEAN, ID};
}

using namespace OPERATION;
using namespace ATTRIBUTE;

class Quaternion
{
public:
	Quaternion() = default;
	Quaternion(Operation o, int o1, Attribute a1, int o2, Attribute a2, int r):
		operation(o), operand1(o1), attribute1(a1), operand2(o2), attribute2(a2), result(r) {}

	Operation operation;
	int operand1;
	Attribute attribute1;
	int operand2;
	Attribute attribute2;
	int result;

private:

};

class SemanticsItem
{
public:
	SemanticsItem() = default;
	SemanticsItem(int sp, Attribute a): semanticsIndex(sp), attribute(a){}

	int semanticsIndex;
	Attribute attribute;

private:

};

class Parser: public Tokenizer
{
public:
	Parser()
	{
		temporaryIndex = 1;
	}

	bool PROGRAM();
	bool DECLARATION_LIST();
	bool DECLARATION();
	bool __DECLARATION__();
	bool STATEMENT_LIST();
	bool STATEMENT();
	bool __STATEMENT__();
	bool IF();
	bool WHILE();
	bool ASSIGN();
	bool EXPRESSION();
	bool __EXPRESSION__();
	bool TERM();
	bool __TERM__();
	bool FACTOR();
	bool TYPE();
	bool ID(bool r);
	bool NUMBER();
	bool SET();
	bool LOGIC();

	bool parse();

	void GENERATE_ADD();
	void GENERATE_SUBTRACT();
	void GENERATE_MULTIPLY();
	void GENERATE_DIVIDE();
	void PUSH_ID();
	void PUSH_INTEGER_NUMBER();
	void PUSH_REAL_NUMBER();
	void PUSH_BOOLEAN_CONSTANT();

	void GENERATE_THEN();
	void GENERATE_ELSE();
	void GENERATE_IF_END();

	void GENERATE_WHILE();
	void GENERATE_WHILE_BEGIN();
	void GENERATE_WHILE_END();

	void GENERATE_ASSIGN();
	void GENERATE_SET();

	vector<Quaternion> quaternionSequence;
	vector<SemanticsItem> semantics;

	int temporaryIndex;

	void showQuaternion(Quaternion q);
	void showQuaternionSequence();

private:

};

#endif /*__PARSER__*/