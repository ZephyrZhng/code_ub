#include "parser.h"

bool Parser::PROGRAM()
{
	string pr = "Parsing failed at [ PROGRAM -> program begin DELCARATION_LIST STATEMENT_LIST end ], ";

	Token tk1;
	next(tk1);
	if(tk1.lexeme == KEYWORD &&
		keywordList[tk1.attribute] == "program")
	{
		Token tk2;
		next(tk2);
		if(tk2.lexeme == KEYWORD &&
			keywordList[tk2.attribute] == "begin")
		{
			Token tk3;
			next(tk3);
			if(DECLARATION_LIST())
			{
				if(STATEMENT_LIST())
				{
					if(tokenSequence.back().lexeme == KEYWORD &&
						keywordList[tokenSequence.back().attribute] == "end")
					{
						cout << "Parsing [ PROGRAM -> program begin DELCARATION_LIST STATEMENT_LIST end ] succeed!" << endl;
						Token tk4;
						next(tk4);
						return true;
					}
					else
					{
						cerr << "[ end ] did not appear!" << endl;
						return false;
					}
				}
				else
				{
					cerr << pr << "error occurs when parsing [ STATEMENT_LIST ]!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "error occurs when parsing [ DECLARATION_LIST ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "[ begin ] did not appear!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ program ] did not appear!" << endl;
		return false;
	}
}

bool Parser::DECLARATION_LIST()
{
	string pr = "Parsing failed at [ DECLARATION_LIST -> declaration begin DECLARATION end ], ";
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "declaration")
	{
		Token tk1;
		next(tk1);
		if(tk1.lexeme == KEYWORD &&
			keywordList[tk1.attribute] == "begin")
		{
			Token tk2;
			next(tk2);
			if(DECLARATION())
			{
				if(tokenSequence.back().lexeme == KEYWORD &&
					keywordList[tokenSequence.back().attribute] == "end")
				{
					cout << "Parsing [ DECLARATION_LIST -> declaration begin DECLARATION end ] succeed!" << endl;
					Token tk3;
					next(tk3);
					return true;
				}
				else
				{
					cerr << pr << "[ end ] did not appear!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "error occurs when parsing [ DECLARATION ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "[ begin ] did not appear!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ declaration ] did not appear!" << endl;
		return false;
	}
}

bool Parser::DECLARATION()
{
	string pr = "Parsing failed at [ DECLARATION -> TYPE ID __DECLARATION__ ], ";
	if(TYPE())
	{
		if(ID(true))
		{
			if(__DECLARATION__())
			{
				cout << "Parsing [ DECLARATION -> TYPE ID __DECLARATION__ ] succeed!" << endl;
				return true;
			}
			else
			{
				cerr << pr << "error occurs when parsing [ __DECLARATION__ ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "error occurs when parsing [ ID ]!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "error occurs when parsing [ TYPE ]!" << endl;
		return false;
	}
}

bool Parser::__DECLARATION__()
{
	if(DECLARATION())
	{
		if(__DECLARATION__())
		{
			cout << "Parsing [ __DECLARATION__ -> DECLARATION __DECLARATION__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << "Parsing failed at [ __DECLARATION__ -> DECLARATION __DECLARATION__ ], "
			     << "error occurs when parsing [ __DECLARATION__ ]!" << endl;
			return false;
		}
	}
	else
	{
		cout << "Parsing [ __DECLARATION__ ->  ] succeed!" << endl;
		return true;
	}
}

bool Parser::STATEMENT_LIST()
{
	string pr = "Parsing failed at [ STATEMENT_LIST -> statement begin STATEMENT end ], ";
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "statement")
	{
		Token tk0;
		next(tk0);
		if(tk0.lexeme == KEYWORD &&
			keywordList[tk0.attribute] == "begin")
		{
			Token tk1;
			next(tk1);
			if(STATEMENT())
			{
				if(tokenSequence.back().lexeme == KEYWORD &&
					keywordList[tokenSequence.back().attribute] == "end")
				{
					cout << "Parsing [ STATEMENT_LIST -> statement begin STATEMENT end ] succeed!" << endl;
					Token tk2;
					next(tk2);
					return true;
				}
				else
				{
					cerr << pr << "[ end ] did not appear!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "error occurs when parsing [ STATEMENT ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "[ begin ] did not appear!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ statement ] did not appear!" << endl;
		return false;
	}
}

bool Parser::STATEMENT()
{
	if(IF())
	{
		if(__STATEMENT__())
		{
			cout << "Parsing [ STATEMENT -> IF __STATEMENT__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << "Parsing failed at [ STATEMENT -> IF __STATEMENT__ ], "
			     << "error occurs when parsing [ __STATEMENT__ ]!" << endl;
			return false;
		}
	}
	else if(WHILE())
	{
		if(__STATEMENT__())
		{
			cout << "Parsing [ STATEMENT -> WHILE __STATEMENT__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << "Parsing failed at [ STATEMENT -> WHILE __STATEMENT__ ], "
			     << "error occurs when parsing [ __STATEMENT__ ]!" << endl;
			return false;
		}
	}
	else if(ASSIGN())
	{
		if(__STATEMENT__())
		{
			cout << "Parsing [ STATEMENT -> ASSIGN __STATEMENT__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << "Parsing failed at [ STATEMENT -> ASSIGN __STATEMENT__ ], "
			     << "error occurs when parsing [ __STATEMENT__ ]!" << endl;
			return false;
		}
	}
	else if(SET())
	{
		if(__STATEMENT__())
		{
			cout << "Parsing [ STATEMENT -> SET __STATEMENT__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << "Parsing failed at [ STATEMENT -> SET __STATEMENT__ ], "
			     << "error occurs when parsing [ __STATEMENT__ ]!" << endl;
			return false;
		}
	}
	else
	{
		cerr << "Parsing failed at [ STATEMENT -> IF __STATEMENT__ ], [ STATEMENT -> WHILE __STATEMENT__ ], "
		     << "[ STATEMENT -> ASSIGN __STATEMENT__ ] and [ STATEMENT -> SET __STATEMENT__ ]!"
		     << endl;
		return false;
	}
}

bool Parser::__STATEMENT__()
{
	if(STATEMENT())
	{
		if(__STATEMENT__())
		{
			cout << "Parsing [ __STATEMENT__ -> STATEMENT __STATEMENT__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << "Parsing failed at [ __STATEMENT__ -> STATEMENT __STATEMENT__ ], "
			     << "error occurs when parsing [ __STATEMENT__ ]!" << endl;
			return false;
		}
	}
	else
	{
		cout << "Parsing [ __STATEMENT__ ->  ] succeed!" << endl;
		return true;
	}
}

bool Parser::IF()
{
	string pr = "Parsing failed at [ IF -> if begin EXPRESSION end then begin STATEMENT end else then begin STATEMENT end ], ";
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "if")
	{
		Token tk1;
		next(tk1);
		if(tk1.lexeme == KEYWORD &&
			keywordList[tk1.attribute] == "begin")
		{
			Token tk2;
			next(tk2);
			if(LOGIC())
			{
				if(tokenSequence.back().lexeme == KEYWORD &&
					keywordList[tokenSequence.back().attribute] == "end")
				{
					Token tk3;
					next(tk3);
					if(tk3.lexeme == KEYWORD &&
						keywordList[tk3.attribute] == "then")
					{
						GENERATE_THEN();
						Token tk4;
						next(tk4);
						if(tk4.lexeme == KEYWORD &&
							keywordList[tk4.attribute] == "begin")
						{
							Token tk5;
							next(tk5);
							if(STATEMENT())
							{
								if(tokenSequence.back().lexeme == KEYWORD &&
									keywordList[tokenSequence.back().attribute] == "end")
								{
									Token tk6;
									next(tk6);
									if(tk6.lexeme == KEYWORD &&
										keywordList[tk6.attribute] == "else")
									{
										GENERATE_ELSE();
										Token tk7;
										next(tk7);
										if(tk7.lexeme == KEYWORD &&
											keywordList[tk7.attribute] == "then")
										{
											Token tk8;
											next(tk8);
											if(tk8.lexeme == KEYWORD &&
												keywordList[tk8.attribute] == "begin")
											{
												Token tk9;
												next(tk9);
												if(STATEMENT())
												{
													if(tokenSequence.back().lexeme == KEYWORD &&
														keywordList[tokenSequence.back().attribute] == "end")
													{
														GENERATE_IF_END();
														cout << "Parsing "
														     << "[ IF -> "
														     << "if begin EXPRESSION end then begin STATEMENT end else then begin STATEMENT end ] "
														     << "succeed!" << endl;
														Token tk10;
														next(tk10);
														return true;
													}
													else
													{
														cerr << pr << "[ end ] did not appear!" << endl;
														return false;
													}
												}
												else
												{
													cerr << pr << "error occurs when parsing [ STATEMENT ]!" << endl;
													return false;
												}
											}
											else
											{
												cerr << pr << "[ begin ] did not appear!" << endl;
												return false;
											}
										}
										else
										{
											cerr << pr << "[ then ] did not appear!" << endl;
											return false;
										}
									}
									else
									{
										cerr << pr << "[ else ] did not appear!" << endl;
										return false;
									}
								}
								else
								{
									cerr << pr << "[ end ] did not appear!" << endl;
									return false;
								}
							}
							else
							{
								cerr << pr << "error occurs when parsing [ STATEMENT ]!" << endl;
								return false;
							}
						}
						else
						{
							cerr << pr << "[ begin ] did not appear!" << endl;
							return false;
						}
					}
					else
					{
						cerr << pr << "[ then ] did not appear!" << endl;
						return false;
					}
				}
				else
				{
					cerr << pr << "[ end ] did not appear!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "error occurs when parsing [ EXPRESSION ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "[ begin ] did not appear!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ if ] did not appear!" << endl;
		return false;
	}
}

bool Parser::WHILE()
{
	string pr = "Parsing failed at [ WHILE -> while begin EXPRESSION end then begin STATEMENT end ], ";
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "while")
	{
		GENERATE_WHILE();
		Token tk1;
		next(tk1);
		if(tk1.lexeme == KEYWORD &&
			keywordList[tk1.attribute] == "begin")
		{
			Token tk2;
			next(tk2);
			if(LOGIC())
			{
				if(tokenSequence.back().lexeme == KEYWORD &&
					keywordList[tokenSequence.back().attribute] == "end")
				{
					Token tk3;
					next(tk3);
					if(tk3.lexeme == KEYWORD &&
						keywordList[tk3.attribute] == "then")
					{
						GENERATE_WHILE_BEGIN();
						Token tk4;
						next(tk4);
						if(tk4.lexeme == KEYWORD &&
							keywordList[tk4.attribute] == "begin")
						{
							Token tk5;
							next(tk5);
							if(STATEMENT())
							{
								if(tokenSequence.back().lexeme == KEYWORD &&
									keywordList[tokenSequence.back().attribute] == "end")
								{
									GENERATE_WHILE_END();
									cout << "Parsing [ WHILE -> while begin EXPRESSION end then begin STATEMENT end ] succeed!" << endl;
									Token tk6;
									next(tk6);
									return true;
								}
								else
								{
									cerr << pr << "[ end ] did not appear!" << endl;
									return false;
								}
							}
							else
							{
								cerr << "error occurs when parsing [ STATEMENT ]!" << endl;
								return false;
							}
						}
						else
						{
							cerr << pr << "[ begin ] did not appear!" << endl;
							return false;
						}
					}
					else
					{
						cerr << pr << "[ then ] did not appear!" << endl;
						return false;
					}
				}
				else
				{
					cerr << pr << "[ end ] did not appear!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "error occurs when parsing [ EXPRESSION ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "[ begin ] did not appear!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ while ] did not appear!" << endl;
		return false;
	}
}

bool Parser::ASSIGN()
{
	string pr = "Parsing failed at [ ASSIGN -> assign begin EXPRESSION end to ID ], ";
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "assign")
	{
		Token tk1;
		next(tk1);
		if(tk1.lexeme == KEYWORD &&
			keywordList[tk1.attribute] == "begin")
		{
			Token tk2;
			next(tk2);
			if(EXPRESSION())
			{
				if(tokenSequence.back().lexeme == KEYWORD &&
					keywordList[tokenSequence.back().attribute] == "end")
				{
					Token tk3;
					next(tk3);
					if(tk3.lexeme == KEYWORD &&
						keywordList[tk3.attribute] == "to")
					{
						Token tk4;
						next(tk4);
						if(ID(false))
						{
							GENERATE_ASSIGN();
							cout << "Parsing [ ASSIGN -> assign begin EXPRESSION end to ID ] succeed!" << endl;
							return true;
						}
						else
						{
							cerr << pr << "error occurs when parsing [ ID ]!" << endl;
							return false;
						}
					}
					else
					{
						cerr << pr << "[ to ] did not appear!" << endl;
						return false;
					}
				}
				else
				{
					cerr << pr << "[ end ] did not appear!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "error occurs when parsing [ EXPRESSION ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "[ begin ] did not appear!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ assign ] did not appear!" << endl;
		return false;
	}
}

bool Parser::SET()
{
	string pr = "Parsing failed at [ SET -> set ID as begin LOGIC end ], ";
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "set")
	{
		Token tk1;
		next(tk1);
		if(ID(false))
		{
			PUSH_ID();
			if(tokenSequence.back().lexeme == KEYWORD &&
				keywordList[tokenSequence.back().attribute] == "as")
			{
				Token tk2;
				next(tk2);
				if(tk2.lexeme == KEYWORD &&
					keywordList[tk2.attribute] == "begin")
				{
					Token tk3;
					next(tk3);
					if(LOGIC())
					{
						if(tokenSequence.back().lexeme == KEYWORD &&
							keywordList[tokenSequence.back().attribute] == "end")
						{
							GENERATE_SET();
							cout << "Parsing [ SET -> set ID as begin LOGIC end ] succeed!" << endl;
							Token tk4;
							next(tk4);
							return true;
						}
						else
						{
							cerr << pr << "[ end ] did not appear!" << endl;
							return false;
						}
					}
					else
					{
						cerr << pr << "error occurs when parsing [ LOGIC ]!" << endl;
						return false;
					}
				}
				else
				{
					cerr << pr << "[ begin ] did not appear!" << endl;
					return false;
				}
			}
			else
			{
				cerr << pr << "[ as ] did not appear!" << endl;
				return false;
			}
		}
		else
		{
			cerr << pr << "error occurs when parsing [ ID ]!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "[ set ] did not appear!" << endl;
		return false;
	}
}

bool Parser::EXPRESSION()
{
	string pr = "Parsing failed at [ EXPRESSION -> TERM __EXPRESSION__ ], ";
	if(TERM())
	{
		if(__EXPRESSION__())
		{
			cout << "Parsing [ EXPRESSION -> TERM __EXPRESSION__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << pr << "error occurs when parsing [ __EXPRESSION__ ]!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "error occurs when parsing [ TERM ]!" << endl;
		return false;
	}
}

bool Parser::__EXPRESSION__()
{
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "add")
	{
		Token tk1;
		next(tk1);
		if(TERM())
		{
			GENERATE_ADD();
			if(__EXPRESSION__())
			{
				cout << "Parsing [ __EXPRESSION__ -> add TERM __EXPRESSION__ ] succeed!" << endl;
				return true;
			}
			else
			{
				cerr << "Parsing failed at [ __EXPRESSION__ -> add TERM __EXPRESSION__ ], "
				     << "error occurs when parsing [ __EXPRESSION__ ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << "Parsing failed at [ __EXPRESSION__ -> add TERM __EXPRESSION__ ], "
			     << "error occurs when parsing [ TERM ]!" << endl;
			return false;
		}
	}
	else if(tokenSequence.back().lexeme == KEYWORD && 
		keywordList[tokenSequence.back().attribute] == "subtract")
	{
		Token tk2;
		next(tk2);
		if(TERM())
		{
			GENERATE_SUBTRACT();
			if(__EXPRESSION__())
			{
				cout << "Parsing [ __EXPRESSION__ -> subtract TERM __EXPRESSION__ ] succeed!" << endl;
				return true;
			}
			else
			{
				cerr << "Parsing failed at [ __EXPRESSION__ -> subtract TERM __EXPRESSION__ ], "
				     << "error occurs when parsing [ __EXPRESSION__ ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << "Parsing failed at [ __EXPRESSION__ -> subtract TERM __EXPRESSION__ ], "
			     << "error occurs when parsing [ TERM ]!" << endl;
			return false;
		}
	}
	else
	{
		cout << "Parsing [ __EXPRESSION__ ->  ] succeed!" << endl;
		return true;
	}
}

bool Parser::TERM()
{
	string pr = "Parsing failed at [ TERM -> FACTOR __TERM__ ], ";
	if(FACTOR())
	{
		if(__TERM__())
		{
			cout << "Parsing [ TERM -> FACTOR __TERM__ ] succeed!" << endl;
			return true;
		}
		else
		{
			cerr << pr << "error occurs when parsing [ __TERM__ ]!" << endl;
			return false;
		}
	}
	else
	{
		cerr << pr << "error occurs when parsing [ FACTOR ]!" << endl;
		return false;
	}
}

bool Parser::__TERM__()
{
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "multiply")
	{
		Token tk1;
		next(tk1);
		if(FACTOR())
		{
			GENERATE_MULTIPLY();
			if(__TERM__())
			{
				cout << "Parsing [ __TERM__ -> multiply FACTOR __TERM__ ] succeed!" << endl;
				return true;
			}
			else
			{
				cerr << "Parsing failed at [ __TERM__ -> multiply FACTOR __TERM__ ], "
				     << "error occurs when parsing [ __EXPRESSION__ ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << "Parsing failed at [ __TERM__ -> multiply FACTOR __TERM__ ], "
			     << "error occurs when parsing [ TERM ]!" << endl;
			return false;
		}
	}
	else if(tokenSequence.back().lexeme == KEYWORD && 
		keywordList[tokenSequence.back().attribute] == "divide")
	{
		Token tk2;
		next(tk2);
		if(FACTOR())
		{
			GENERATE_DIVIDE();
			if(__TERM__())
			{
				cout << "Parsing [ __TERM__ -> divide FACTOR __TERM__ ] succeed!" << endl;
				return true;
			}
			else
			{
				cerr << "Parsing failed at [ __TERM__ -> divide FACTOR __TERM__ ], "
				     << "error occurs when parsing [ __EXPRESSION__ ]!" << endl;
				return false;
			}
		}
		else
		{
			cerr << "Parsing failed at [ __TERM__ -> divide FACTOR __TERM__ ], "
			     << "error occurs when parsing [ TERM ]!" << endl;
			return false;
		}
	}
	else
	{
		cout << "Parsing [ __TERM__ ->  ] succeed!" << endl;
		return true;
	}
}

bool Parser::FACTOR()
{
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "begin")
	{
		Token tk1;
		next(tk1);
		if(EXPRESSION())
		{
			if(tokenSequence.back().lexeme == KEYWORD &&
				keywordList[tokenSequence.back().attribute] == "end")
			{
				cout << "Parsing [ FACTOR -> begin EXPRESSION end ] succeed!" << endl;
				Token tk2;
				next(tk2);
				return true;
			}
			else
			{
				cerr << "Parsing failed at [ FACTOR -> begin EXPRESSION end ], "
				     << "[ end ] did not appear!" << endl;
				return false;
			}
		}
		else
		{
			cerr << "Parsing failed at [ FACTOR -> begin EXPRESSION end ], "
			     << "error occurs when parsing [ EXPRESSION ]!" << endl;
			return false;
		}
	}
	else if(ID(false))
	{
		PUSH_ID();
		cout << "Parsing [ FACTOR -> ID ] succeed!" << endl;
		return true;
	}
	else if(NUMBER())
	{
		if(tokenSequence[tokenSequence.size() - 2].lexeme == LEXEME::INTEGER_CONSTANT)
		{
			PUSH_INTEGER_NUMBER();
		}
		else if(tokenSequence[tokenSequence.size() - 2].lexeme == LEXEME::REAL_CONSTANT)
		{
			PUSH_REAL_NUMBER();
		}
		cout << "Parsing [ FACTOR -> NUMBER ] succeed!" << endl;
		return true;
	}
	else
	{
		cerr << "Parsing failed at [ FACTOR -> begin EXPRESSION end ], [ FACTOR -> ID ] and [ FACTOR -> NUMBER ]!" << endl;
		return false;
	}
}

bool Parser::LOGIC()
{
	if(ID(false))
	{
		PUSH_ID();

		cout << "Parsing [ LOGIC -> ID ] succeed!" << endl;
		return true;
	}
	else if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "true")
	{
		PUSH_BOOLEAN_CONSTANT();

		cout << "Parsing [ LOGIC -> true ] succeed!" << endl;
		Token tk1;
		next(tk1);
		return true;
	}
	else if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "false")
	{
		PUSH_BOOLEAN_CONSTANT();

		cout << "Parsing [ LOGIC -> false ] succeed!" << endl;
		Token tk2;
		next(tk2);
		return true;
	}
	else
	{
		cerr << "Parsing failed at [ LOGIC -> ID ], [ LOGIC -> true ] and [ LOGIC -> false ]!" << endl;
		return false;
	}
}

bool Parser::TYPE()
{
	if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "integer")
	{
		cout << "Parsing [ TYPE -> integer ] succeed!" << endl;
		Token tk1;
		next(tk1);
		return true;
	}
	else if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "real")
	{
		cout << "Parsing [ TYPE -> real ] succeed!" << endl;
		Token tk2;
		next(tk2);
		return true;
	}
	else if(tokenSequence.back().lexeme == KEYWORD &&
		keywordList[tokenSequence.back().attribute] == "boolean")
	{
		cout << "Parsing [ TYPE -> boolean ] succeed!" << endl;
		Token tk3;
		next(tk3);
		return true;
	}
	else
	{
		cerr << "Parsing failed at [ TYPE -> integer ], [ TYPE -> real ] and [ TYPE -> boolean ]!" << endl;
		return false;
	}
}

bool Parser::ID(bool r)
{
	if(tokenSequence.back().lexeme == LEXEME::ID)
	{
		string n = symbolList[tokenSequence.back().attribute].name;
		if(!r)
		{
			symbolList.erase(symbolList.end() - 1);
			auto p = find_if(symbolList.begin(), symbolList.end(),
				[n](const SymbolListItem& si)
				{
					return si.name == n;
				});
			tokenSequence.back().attribute = p - symbolList.begin();
		}
		else
		{
			if(keywordList[tokenSequence[tokenSequence.size() - 2].attribute] == "integer")
			{
				symbolList[tokenSequence.back().attribute].type = 0;
			}
			else if(keywordList[tokenSequence[tokenSequence.size() - 2].attribute] == "real")
			{
				symbolList[tokenSequence.back().attribute].type = 1;
			}
			else if(keywordList[tokenSequence[tokenSequence.size() - 2].attribute] == "boolean")
			{
				symbolList[tokenSequence.back().attribute].type = 2;
			}
		}

		cout << "Parsing succeed, current morpheme is an ID!" << endl;
		Token tk;
		next(tk);
		return true;
	}
	else
	{
		cerr << "Parsing failed, current morpheme is not an ID!" << endl;
		return false;
	}
}

bool Parser::NUMBER()
{
	if(tokenSequence.back().lexeme == LEXEME::INTEGER_CONSTANT ||
		tokenSequence.back().lexeme == LEXEME::REAL_CONSTANT)
	{
		cout << "Parsing succeed, current morpheme is a NUMBER!" << endl;
		Token tk;
		next(tk);
		return true;
	}
	else
	{
		cerr << "Parsing failed, current morpheme is not a NUMBER!" << endl;
		return false;
	}
}

bool Parser::parse()
{
	if(PROGRAM())
	{
		cout << "Parsing source code succeed!" << endl;
		return true;
	}
	else
	{
		cerr << "Parsing source code failed!" << endl;
		return false;
	}
}

// determine index of temporary parameters

void Parser::GENERATE_ADD()
{
	cout << "GENERATE_ADD: ";
	Quaternion q;
	q.operation = OPERATION::ADD;
	q.operand2 = semantics.back().semanticsIndex;
	q.attribute2 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);

	SymbolListItem si;
	si.name = "$t" + to_string(temporaryIndex++);
	si.type = 1; // real
	si.symbolCategory = VARIABLE;
	// si.address ...
	symbolList.push_back(si);
	q.result = symbolList.size() - 1;

	semantics.push_back(SemanticsItem(symbolList.size() - 1, ATTRIBUTE::ID));

	quaternionSequence.push_back(q);

	showQuaternion(q);
}

void Parser::GENERATE_SUBTRACT()
{
	cout << "GENERATE_SUBTRACT: ";
	Quaternion q;
	q.operation = OPERATION::SUBTRACT;
	q.operand2 = semantics.back().semanticsIndex;
	q.attribute2 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);

	SymbolListItem si;
	si.name = "$t" + to_string(temporaryIndex++);
	si.type = 1; // real
	si.symbolCategory = VARIABLE;
	// si.address ...
	symbolList.push_back(si);
	q.result = symbolList.size() - 1;

	semantics.push_back(SemanticsItem(symbolList.size() - 1, ATTRIBUTE::ID));

	quaternionSequence.push_back(q);

	showQuaternion(q);
}

void Parser::GENERATE_MULTIPLY()
{
	cout << "GENERATE_MULTIPLY: ";
	Quaternion q;
	q.operation = OPERATION::MULTIPLY;
	q.operand2 = semantics.back().semanticsIndex;
	q.attribute2 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);

	SymbolListItem si;
	si.name = "$t" + to_string(temporaryIndex++);
	si.type = 1; // real
	si.symbolCategory = VARIABLE;
	// si.address ...
	symbolList.push_back(si);
	q.result = symbolList.size() - 1;

	semantics.push_back(SemanticsItem(symbolList.size() - 1, ATTRIBUTE::ID));

	quaternionSequence.push_back(q);

	showQuaternion(q);
}

void Parser::GENERATE_DIVIDE()
{
	cout << "GENERATE_DIVIDE: ";
	Quaternion q;
	q.operation = OPERATION::DIVIDE;
	q.operand2 = semantics.back().semanticsIndex;
	q.attribute2 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);

	SymbolListItem si;
	si.name = "$t" + to_string(temporaryIndex++);
	si.type = 1; // real
	si.symbolCategory = VARIABLE;
	// si.address ...
	symbolList.push_back(si);
	q.result = symbolList.size() - 1;

	semantics.push_back(SemanticsItem(symbolList.size() - 1, ATTRIBUTE::ID));

	quaternionSequence.push_back(q);

	showQuaternion(q);
}

void Parser::PUSH_ID()
{
	cout << "PUSH_ID: ";
	semantics.push_back(SemanticsItem(tokenSequence[tokenSequence.size() - 2].attribute, ATTRIBUTE::ID));
	cout << "[ " << symbolList[tokenSequence[tokenSequence.size() - 2].attribute].name << " ]" << endl;
}

void Parser::PUSH_INTEGER_NUMBER()
{
	cout << "PUSH_INTEGER_NUMBER: ";
	semantics.push_back(SemanticsItem(tokenSequence[tokenSequence.size() - 2].attribute, ATTRIBUTE::INTEGER_NUMBER));
	cout << "[ " << integerConstantList[tokenSequence[tokenSequence.size() - 2].attribute] << " ]" << endl;
}

void Parser::PUSH_REAL_NUMBER()
{
	cout << "PUSH_REAL_NUMBER: ";
	semantics.push_back(SemanticsItem(tokenSequence[tokenSequence.size() - 2].attribute, ATTRIBUTE::REAL_NUMBER));
	cout << "[ " << realConstantList[tokenSequence[tokenSequence.size() - 2].attribute] << " ]" << endl;
}

void Parser::PUSH_BOOLEAN_CONSTANT()
{
	cout << "PUSH_BOOLEAN_CONSTANT: ";
	semantics.push_back(SemanticsItem(tokenSequence[tokenSequence.size() - 1].attribute, ATTRIBUTE::BOOLEAN));
	cout << "[ " << keywordList[tokenSequence[tokenSequence.size() - 1].attribute] << " ]" << endl;
}

void Parser::GENERATE_THEN()
{
	cout << "GENERATE_THEN: ";
	Quaternion q;
	q.operation = OPERATION::THEN;
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);

	// q.result

	quaternionSequence.push_back(q);

	showQuaternion(q);
}

void Parser::GENERATE_ELSE()
{
	cout << "GENERATE_ELSE: ";
	Quaternion q;
	q.operation = OPERATION::ELSE;
	// q.result
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::GENERATE_IF_END()
{
	cout << "GENERATE_IF_END: ";
	Quaternion q;
	q.operation = OPERATION::IF_END;
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::GENERATE_WHILE()
{
	cout << "GENERATE_WHILE: ";
	Quaternion q;
	q.operation = OPERATION::WHILE;
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::GENERATE_WHILE_BEGIN()
{
	cout << "GENERATE_WHILE_BEGIN: ";
	Quaternion q;
	q.operation = OPERATION::WHILE_BEGIN;
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	// q.result
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::GENERATE_WHILE_END()
{
	cout << "GENERATE_WHILE_END: ";
	Quaternion q;
	q.operation = OPERATION::WHILE_END;
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::GENERATE_ASSIGN()
{
	cout << "GENERATE_ASSIGN: ";
	Quaternion q;
	q.operation = OPERATION::ASSIGN;
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	q.result = tokenSequence[tokenSequence.size() - 2].attribute;
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::GENERATE_SET()
{
	cout << "GENERATE_SET: ";
	Quaternion q;
	q.operation = OPERATION::SET;
	q.operand1 = semantics.back().semanticsIndex;
	q.attribute1 = semantics.back().attribute;
	semantics.erase(semantics.end() - 1);
	q.result = semantics.back().semanticsIndex;
	semantics.erase(semantics.end() - 1);
	quaternionSequence.push_back(q);
	showQuaternion(q);
}

void Parser::showQuaternion(Quaternion q)
{
	bool isExpression = false;
	bool isIf = false;
	bool isWhile = false;
	bool isAssign = false;
	bool isSet = false;

	switch(q.operation)
	{
		case OPERATION::ADD:         cout << "[ ADD "; isExpression = true;      break;
		case OPERATION::SUBTRACT:    cout << "[ SUBTRACT "; isExpression = true; break;
		case OPERATION::MULTIPLY:    cout << "[ MULTIPLY "; isExpression = true; break;
		case OPERATION::DIVIDE:      cout << "[ DIVIDE "; isExpression = true;   break;
		case OPERATION::ASSIGN:      cout << "[ ASSIGN "; isAssign = true;       break;
		case OPERATION::SET:         cout << "[ SET "; isSet = true;             break;
		case OPERATION::WHILE:       cout << "[ WHILE "; isWhile = true;         break;
		case OPERATION::WHILE_BEGIN: cout << "[ WHILE_BEGIN "; isWhile = true;   break;
		case OPERATION::WHILE_END:   cout << "[ WHILE_END "; isWhile = true;     break;
		case OPERATION::THEN:        cout << "[ THEN "; isIf = true;             break;
		case OPERATION::ELSE:        cout << "[ ELSE "; isIf = true;             break;
		case OPERATION::IF_END:      cout << "[ IF_END "; isIf = true;           break;
		default: break;
	}

	if(isExpression)
	{
		switch(q.attribute1)
		{
			case ATTRIBUTE::INTEGER_NUMBER: cout << integerConstantList[q.operand1] << " "; break;
			case ATTRIBUTE::REAL_NUMBER:    cout << realConstantList[q.operand1] << " ";    break;
			case ATTRIBUTE::ID:             cout << symbolList[q.operand1].name << " ";     break;
			default: break;
		}

		switch(q.attribute2)
		{
			case ATTRIBUTE::INTEGER_NUMBER: cout << integerConstantList[q.operand2] << " "; break;
			case ATTRIBUTE::REAL_NUMBER:    cout << realConstantList[q.operand2] << " ";    break;
			case ATTRIBUTE::ID:             cout << symbolList[q.operand2].name << " ";     break;
			default: break;
		}

		cout << symbolList[q.result].name << " ";

		cout << "]\n";
	}
	else if(isIf)
	{
		cout << ((q.operation == OPERATION::THEN)? symbolList[q.operand1].name + " ": "__ ");
		cout << "__ ";
		cout << ((q.operation == OPERATION::IF_END)? "__ ]\n": "? ]\n");
	}
	else if(isWhile)
	{
		cout << ((q.operation == OPERATION::WHILE_BEGIN)? symbolList[q.operand1].name + " ": "__ ");
		cout << "__ ";
		cout << ((q.operation == OPERATION::WHILE)? "__ ]\n": "? ]\n");
	}
	else if(isAssign)
	{
		switch(q.attribute1)
		{
			case ATTRIBUTE::INTEGER_NUMBER: cout << integerConstantList[q.operand1]; break;
			case ATTRIBUTE::REAL_NUMBER:    cout << realConstantList[q.operand1];    break;
			case ATTRIBUTE::ID:             cout << symbolList[q.operand1].name;     break;
			default: break;
		}
		cout << " __ " << symbolList[q.result].name << " ]\n";
	}
	else if(isSet)
	{
		switch(q.attribute1)
		{
			case ATTRIBUTE::BOOLEAN: cout << keywordList[q.operand1];     break;
			case ATTRIBUTE::ID:      cout << symbolList[q.operand1].name; break;
			default: break;
		}
		cout << " __ " << symbolList[q.result].name << " ]\n";
	}
}

void Parser::showQuaternionSequence()
{
	cout << "quaternionSequence:\n";
	for(int i = 0; i < quaternionSequence.size(); ++i)
	{
		showQuaternion(quaternionSequence[i]);
	}
}