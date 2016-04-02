#include "lexer.h"
#include <cstdio>

Token::Token(int myType, int myVal): type(myType), val(myVal)
{

}

Lexer::Lexer(const string& keywordFilePath, const string& symbolFilePath)
{
	ifstream keywordFile(keywordFilePath);
	assert(keywordFile);

	string keyword;
	int keywordNum;
	keywordFile >> keywordNum;
	for(int i = 0; i < keywordNum; ++i)
	{
		keywordFile >> keyword;
		keywordTab.push_back(keyword);
		keywordTrie.insert(keyword);
	}

	keywordFile.close();

	ifstream symbolFile(symbolFilePath);
	assert(symbolFile);

	string symbol;
	int symbolNum;
	symbolFile >> symbolNum;
	for(int i = 0; i < symbolNum; ++i)
	{
		symbolFile >> symbol;
		symbolTab.push_back(symbol);
		symbolTrie.insert(symbol);
	}

	symbolFile.close();
}

void Lexer::showKeywordAndSymbol()
{
	cout << "[keyword trie] "; keywordTrie.preorderTraverse();
	cout << "[symbol trie] "; symbolTrie.preorderTraverse();
	keywordTrie.display();
	symbolTrie.display();
}

void Lexer::readID(FILE* fp, char& ch)
// automaton using if-else sentence
{
	string IDOrKeyword;
	while(isdigit(ch) || ch == '_' || isalpha(ch))
	{
		IDOrKeyword.push_back(ch);
		ch = fgetc(fp);
	}
	// cout << IDOrKeyword << endl;
	auto keywordTabPtr = find(keywordTab.begin(), keywordTab.end(), IDOrKeyword);
	auto idTabPtr = find(idTab.begin(), idTab.end(), IDOrKeyword);
	if(keywordTabPtr != keywordTab.end()) // is keyword
	{
		tokenSeq.push_back(Token(keywordTabPtr - keywordTab.begin() + 4, 0));
	}
	else // is ID
	{
		if(idTabPtr != idTab.end()) // exists in idTab
		{
			tokenSeq.push_back(Token(0, idTabPtr - idTab.begin()));
		}
		else // needs to be added to idTab
		{
			idTab.push_back(IDOrKeyword);
			tokenSeq.push_back(Token(0, idTab.size() - 1));
		}
	}
}

void Lexer::readCh(FILE* fp, char& ch)
{
	Token token;
	token.type = 1;
	ch = fgetc(fp);
	if(ch == '\\')
	{
		char __ch__;
		__ch__ = fgetc(fp);
		switch(__ch__)
		{
			case 'n': ch = '\n'; break;
			case 't': ch = '\t'; break;
			case '\\': ch = '\\'; break;
			case '\'': ch = '\''; break;
			case '\"': ch = '\"'; break;
		}
	}

	// auto chTabPtr = find(chTab.begin(), chTab.end(), ch);
	// if(chTabPtr != chTab.end())
	// {
	// 	token.val = chTabPtr - chTab.begin();
	// 	tokenSeq.push_back(token);
	// }
	// else
	// {
	// 	chTab.push_back(ch);
	// 	token.val = chTab.size() - 1;
	// 	tokenSeq.push_back(token);
	// }

	chTab.push_back(ch);
	token.val = chTab.size() - 1;
	tokenSeq.push_back(token);

	// switch(ch)
	// {
	// 	case '\n': cout << "\\n" << endl; break;
	// 	case '\t': cout << "\\t" << endl; break;
	// 	case '\\': cout << "\\\\" << endl; break;
	// 	case '\'': cout << "\\\'" << endl; break;
	// 	case '\"': cout << "\\\"" << endl; break;
	// 	default: cout << ch << endl;
	// }

	ch = fgetc(fp);
	ch = fgetc(fp);
}

void Lexer::readStr(FILE* fp, char& ch)
{
	string str;
	ch = fgetc(fp);
	while(ch != '\"')
	{
		if(ch == '\\')
		{
			ch = fgetc(fp);
			switch(ch)
			{
				case 'n': ch = '\n'; break;
				case 't': ch = '\t'; break;
				case '\\': ch = '\\'; break;
				case '\'': ch = '\''; break;
				case '\"': ch = '\"'; break; 
			}
		}
		str.push_back(ch);
		ch = fgetc(fp);
	}
	strTab.push_back(str);
	tokenSeq.push_back(Token(2, strTab.size() - 1));
	// cout << str << endl;
	ch = fgetc(fp);
}

void Lexer::readConst(FILE* fp, char& ch)
// automaton whose shift function is embedded in code
{
	int state = 0;
	double d = 0.0;
	int n = 0;
	int p = 0;
	int m = 0;
	int e = 1;

	while(1)
	{
		switch(state)
		{
			// else default condition err

			case 0:
			n = 0;
			p = 0;
			m = 0;
			e = 1;
			if(isdigit(ch))
			{
				state = 1;
				n = 10 * n + (ch - '0');
			}
			else state = 9;
			break;

			case 1:
			if(isdigit(ch)) n = 10 * n + (ch - '0');
			else if(ch == 'e') state = 4;
			else if(ch == '.') state = 2;
			else state = 8;
			break;

			case 2:
			if(isdigit(ch))
			{
				state = 3;
				n = 10 * n + (ch - '0');
				++m;
			}
			else state = 9;
			break;

			case 3:
			if(isdigit(ch))
			{
				n = 10 * n + (ch - '0');
				++m;
			}
			else if(ch == 'e') state = 4;
			else state = 7;
			break;

			case 4: 
			if(ch == '-') e = -1;
			if(ch == '+' || ch == '-') state = 5;
			else if(isdigit(ch))
			{
				state = 6;
				p = 10 * p + (ch - '0');
			}
			else state = 9;
			break;

			case 5:
			if(isdigit(ch)) 
			{
				state = 6;
				p = 10 * p + (ch - '0');
			}
			else state = 9;
			break;

			case 6:
			if(isdigit(ch)) p = 10 * p + (ch - '0');
			else state = 7;
			break;

			default: break;
		}
		if(state != 7 && state != 8 && state != 9)
		{
			ch = fgetc(fp);
		}
		else
		{
			break;
		}
	}

	if(state == 9)
	{
		cerr << "Error when parsing constant!" << endl;
	}
	else
	{
		d = n * pow(10, e * p - m);
		// cout << d << endl;
		tokenSeq.push_back(Token(3, constTab.size()));
		constTab.push_back(d);
	}
}

void Lexer::readSymbol(FILE* fp, char& ch)
// use trie to replace lookahead strategy which might cause uncertainty
{
	// cout << "current character: " << ch << endl;
	symbolTrie.resetCurPtr();
	string symbol;
	while(symbolTrie.match(ch))
	{
		symbol.push_back(ch);
		ch = fgetc(fp);
	}
	tokenSeq.push_back(
		Token(find(symbolTab.begin(), symbolTab.end(), symbol) - symbolTab.begin() + keywordTab.size() + 4, 0)
		);
	// cout << "symbol identified: " << symbol << endl << endl;
}

void Lexer::readSpaceTabNewline(FILE* fp, char& ch)
{
	while(ch == ' ' || ch == '\t' || ch == '\n')
	{
		ch = fgetc(fp);
	}
}

void Lexer::lexicalAnalyze(const string& srcCodeFilePath)
// an integrated automaton using if-else sentence
// according to current input symbol, a sub-automaton would be activated
{
	FILE* fp = fopen(srcCodeFilePath.c_str(), "r");
	char ch;
	ch = fgetc(fp);

	while(ch != EOF)
	{
		// cout << ch;
		if(isalpha(ch) || ch == '_')
		{
			// cout << "will read id" << endl;
			readID(fp, ch);
			// cout << "read id ok" << endl;
		}
		else if(ch == '\'')
		{
			// cout << "will read character" << endl;
			readCh(fp, ch);
			// cout << "read character ok" << endl;
		}
		else if(ch == '\"')
		{
			// cout << "will read string" << endl;
			readStr(fp, ch);
			// cout << "read string ok" << endl;
		}
		else if(isdigit(ch))
		{
			// cout << "will read constant" << endl;
			readConst(fp, ch);
			// cout << "read constant ok" << endl;
		}
		else if(ch == ' ' || ch == '\t' || ch == '\n')
		{
			// cout << "will read space tab and newline" << endl;
			readSpaceTabNewline(fp, ch);
			// cout << "read space tab and newline ok" << endl;
		}
		else
		{
			// cout << "will read symbol" << endl;
			readSymbol(fp, ch);
			// cout << "read symbol ok" << endl;
		}
	}

	// cout << endl;

	fclose(fp);
}

void Lexer::showTokenSeq()
{
	cout << "token sequence: ";
	for_each(tokenSeq.begin(), tokenSeq.end(),
		[](const Token& token)
		{
			cout << "(" << token.type << "," << token.val << ")";
		});
	cout << endl;
}

void Lexer::showIDTab()
{
	cout << "id table: ";
	for_each(idTab.begin(), idTab.end(),
		[](const string& str)
		{
			cout << str << " ";
		});
	cout << endl;
}

void Lexer::showChTab()
{
	cout << "character table: ";
	for_each(chTab.begin(), chTab.end(),
		[](const char& ch)
		{
			cout << ch << " ";
		});
	cout << endl;
}

void Lexer::showStrTab()
{
	cout << "string table: ";
	for_each(strTab.begin(), strTab.end(),
		[](const string& str)
		{
			cout << str << " ";
		});
	cout << endl;
}

void Lexer::shiowConstTab()
{
	cout << "constant table: ";
	for_each(constTab.begin(), constTab.end(),
		[](const double& d)
		{
			cout << d << " ";
		});
	cout << endl;
}

void Lexer::showKeywordTab()
{
	cout << "keyword table: ";
	for_each(keywordTab.begin(), keywordTab.end(),
		[](const string& str)
		{
			cout << str << " ";
		});
	cout << endl;
}

void Lexer::showSymbolTab()
{
	cout << "symbol table: ";
	for_each(symbolTab.begin(), symbolTab.end(),
		[](const string& str)
		{
			cout << str << " ";
		});
	cout << endl;
}