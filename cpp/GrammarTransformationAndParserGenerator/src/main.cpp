// #include "simplifyCFG.h"
#include "CFG.h"
#include "CYK.h"

// #ifdef WINDOWS
// #include <direct.h>
// #define GetCurrentDir _getcwd
// #else
// #include <unistd.h>
// #define GetCurrentDir getcwd
// #endif

using namespace std;

int main(int argc, char** argv)
{
	// char cCurrentPath[FILENAME_MAX];
	// if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	// {
	// 	return errno;
	// }
	// cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	// cout << cCurrentPath << endl;

	// ContextFreeGrammar cfg("../src/cfg");
	// cfg.simplifyCFG();

	CFG g;
	g.displayGrammar();
	g.putInCNF();

	return 0;
}
