#include "tokenizer.h"

Tokenizer::Tokenizer()
{
	keywordList.assign({
		"program",
		"declaration", "statement", 
		"begin", "end", 
		"integer", "real", "boolean",
		"assign", "to", 
		"set", "as",
		"if", "then", "else", 
		"while", 
		"add", "subtract", "multiply", "divide",
		"true", "false"
	});

	typeList.assign({
		TypeListItem(INTEGER, 0),
		TypeListItem(REAL, 0),
		TypeListItem(BOOLEAN, 0)
	});

	f.open("test2.en", ios::in);
}

Tokenizer::~Tokenizer()
{
	f.close();
}

bool Tokenizer::isID(string s)
{
	return regex_match(s, regex("[a-zA-Z_][0-9a-zA-Z_]*"));
}

bool Tokenizer::isRealConstant(string s)
{
	return regex_match(s, regex("(0|(-?[1-9]([0-9])*))\\.([0-9])+"));
}

bool Tokenizer::isIntegerConstant(string s)
{
	return regex_match(s, regex("0|(-?[1-9][0-9]*)"));
}

bool Tokenizer::isKeyword(string s)
{
	return find(keywordList.begin(), keywordList.end(), s) != keywordList.end();
}

bool Tokenizer::next(Token& tk)
{
	if(!f.eof())
	{
		string morpheme;
		f >> morpheme;

		// string sentence = "And I feel fine...";
		// istringstream iss(sentence);

		// vector<string> tokens;
		// copy(istream_iterator<string>(iss),
		// 	 istream_iterator<string>(),
		// 	 back_inserter(tokens));

		// vector<string> tokens{istream_iterator<string>{iss},
		// 					  istream_iterator<string>{}};

		if(isKeyword(morpheme))
		{
			tk.lexeme = KEYWORD;
			tk.attribute = find(keywordList.begin(), keywordList.end(), morpheme) - keywordList.begin();

			tokenSequence.push_back(tk);

			cout << "Morpheme [ " << keywordList[tokenSequence.back().attribute] << " ] is a keyword with index in keywordList "
			     << tokenSequence.back().attribute << "." << endl;
		}
		else if(isID(morpheme))
		{
			tk.lexeme = ID;

			SymbolListItem si;
			si.name = morpheme;
			// si.type
			si.symbolCategory = VARIABLE;
			// si.address ...
			symbolList.push_back(si);
			tk.attribute = symbolList.size() - 1;

			tokenSequence.push_back(tk);

			cout << "Morpheme [ " << symbolList[tokenSequence.back().attribute].name << " ] is an ID." << endl;
		}
		else if(isRealConstant(morpheme))
		{
			tk.lexeme = REAL_CONSTANT;

			double d = stod(morpheme);
			realConstantList.push_back(d);
			tk.attribute = realConstantList.size() - 1;

			tokenSequence.push_back(tk);

			cout << "Morpheme [ " << morpheme << " ] is a real with value of " << realConstantList[tokenSequence.back().attribute] << "." << endl;
		}
		else if(isIntegerConstant(morpheme))
		{
			tk.lexeme = INTEGER_CONSTANT;

			int i = stoi(morpheme);
			integerConstantList.push_back(i);
			tk.attribute = integerConstantList.size() - 1;

			tokenSequence.push_back(tk);

			cout << "Morpheme [ " << morpheme << " ] is an integer with value of " << integerConstantList[tokenSequence.back().attribute] << "." << endl;
		}
		else
		{
			cerr << "Morpheme [ " << morpheme << " ] is not an ID, an integer constant, a real constant or a keyword!" << endl;
			return false;
		}


		return true;
	}
	else
	{
		return false;
	}
}

void Tokenizer::showIntegerConstantList()
{
	cout << "integerConstantList: [ ";
	for_each(integerConstantList.begin(), integerConstantList.end(),
		[](const int& i)
		{
			cout << i << " ";
		});
	cout << "]\n";
}

void Tokenizer::showRealConstantList()
{
	cout << "realConstantList: [ ";
	for_each(realConstantList.begin(), realConstantList.end(),
		[](const double& i)
		{
			cout << i << " ";
		});
	cout << "]\n";
}

void Tokenizer::showSymbolList()
{
	cout << "symbolList:\n";
	for_each(symbolList.begin(), symbolList.end(),
		[this](const SymbolListItem& si)
		{
			cout << "[ " << si.name << " ";
			switch(typeList[si.type].type)
			{
				case TYPE::INTEGER: cout << "integer "; break;
				case TYPE::REAL:    cout << "real ";    break;
				case TYPE::BOOLEAN: cout << "boolean "; break;
				default:                                break;
			}
			cout << "variable ";
			cout << "? ]" << endl;
		});
}