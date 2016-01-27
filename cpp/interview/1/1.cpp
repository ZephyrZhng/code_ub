#include <iostream>
using namespace std;

int f(int j)
{
	int res = 0;
	for (int i = j; i > 0; i /= 10)
	{
		res += i % 10;
	}
	return res;
}

int main(int argc, char const *argv[])
{
	int cnt = 0;
	for (int i = 1; i <= 100; ++i)
	{
		
		if (f(i) % 10 == 0)
		{
			++cnt;
			cout << cnt << "\t" << i;
			cout << "\t\%9 = " << i % 9 << endl;
		}
	}
	return 0;
}