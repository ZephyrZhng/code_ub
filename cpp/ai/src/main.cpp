#include "8DigitsPuzzle.h"

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

	// vector<int> v{1,2,3,4,5};
	// v.insert(v.begin() + 1, 10);
	// for(size_t i = 0; i < v.size(); ++i)
	// {
	// 	cout << v[i];
	// }
	// cout << endl;

	Puzzle p(vector<int>{2, 8, 3, 1, 6, 4, 7, 0, 5});
	p.solve();

	return 0;
}
