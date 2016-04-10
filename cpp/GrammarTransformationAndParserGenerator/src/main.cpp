// #include "simplifyCFG.h"
#include "CFG.h"
#include "CYK.h"
#include "LL1Parser.h"
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

	// ContextFreeGrammar cfg("../src/cfg");
	// cfg.simplifyCFG();

	// CYKParser p = CYKParser(CFG());
	// p.parse({"b", "a", "a", "b", "a"});
	// p.displayCYKTable();

	// CYKParser p = CYKParser(CFG());
	// p.parse({"she", "eats", "a", "fish", "with", "a", "fork"});
	// p.displayCYKTable();

	LL1Parser ll1 = LL1Parser(CFG());
	ll1.constructLL1Table();
	ll1.displayLL1Table();
	ll1.parse(vector<string>{"0", "+", "0", "*", "0"});

	// vector<int> v = {1,2,3,4,5}, v2;
	// for(auto it = v.rbegin(); it != v.rend(); ++it)
	// {
	// 	v2.insert(v2.begin(), *it);
	// }
	// display(v2);
	// cout << endl;

	return 0;
}
