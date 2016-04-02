#include "lexer.h"
#include "parser.h"
#include <unistd.h>

#define getcwd getcwd

int main(int argc, char** argv)
{
	// char cCurrentPath[FILENAME_MAX];
	// if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
	// {
	// 	return errno;
	// }
	// cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	// printf ("The current working directory is %s\n", cCurrentPath);

	Lexer lexer("../src/keyword.txt", "../src/symbol.txt");
	lexer.showKeywordAndSymbol();

	lexer.lexicalAnalyze("../test/test.cpp");

	lexer.showTokenSeq();
	lexer.showIDTab();
	lexer.showChTab();
	lexer.showStrTab();
	lexer.showKeywordTab();
	lexer.showSymbolTab();

	return 0;
}