#include "CFG.h"
#include "CYK.h"
#include "LL1.h"
#include "SLR.h"
#include "CLR.h"
#include "utility.h"

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

	// CYKParser p = CYKParser(CFG());
	// p.parse({"b", "a", "a", "b", "a"});
	// p.displayCYKTable();

	// CYKParser p = CYKParser(CFG());
	// p.parse({"she", "eats", "a", "fish", "with", "a", "fork"});
	// p.displayCYKTable();

	// LL1Parser ll1 = LL1Parser(CFG());
	// ll1.constructLL1Table();
	// ll1.displayLL1Table();
	// ll1.parse(vector<string>{"0", "+", "0", "*", "0"});

	return 0;
}
