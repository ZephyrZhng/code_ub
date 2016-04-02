#include <algorithm>
#include <iostream>

using namespace std;

int f(int i)
{
	/*
	an interesting function
	*/
	++i;
	return i;
}

void g(int& i)
{
	--i;
}

int main()
{
	int x_1_x,__y22,z__123_;
	x_1_x=2*(3+(5/2)-(4<<2));
	__y22 = ~3 + (3 | 5) + (2 ^ 4) + (0 & 1);
	z__123_+=x_1_x;
	z__123_-=__y22;
	z__123_*=z__123_;
	z__123_/=z__123_;

	cout << "x_1_x\t\t= " << x_1_x << endl;
	cout << "__y22\t\t= " << __y22 << endl;
	cout << "z__123_\t\t= " << z__123_ << endl << endl;

	char _a1_       ='a';
	char b__22      = '\n';
	char __3_333___ = '\t';
	char _a_        = '\\';
	char aa         = '\'';
	char bbb        = '\"';
	char s[] = "abc\n\t\\\'\"-=~!@#$%^&*()_+";
	double d=-3.14e-10;

	cout << "_a1_\t\t= " << _a1_ << endl;
	cout << "b__22\t\t= " << b__22 << endl;
	cout << "__3_333___\t= " << __3_333___ << endl;
	cout << "_a_\t\t= " << _a_ << endl;
	cout << "aa\t\t= " << aa << endl;
	cout << "bbb\t\t= " << bbb << endl;
	cout << "s\t\t= " << s << endl;
	cout << "d\t\t= " << d << endl << endl;

	bool b1=true;
	bool b2=false;
	int j=2;
	int a[10];

	cout << "b1\t\t= " << b1 << endl;
	cout << "b2\t\t= " << b2 << endl;
	cout << "j\t\t= " << j << endl;
	cout << "a\t\t= ";
	for_each(begin(a), end(a),
		[](int i)
		{
			cout << i << " ";
		});
	cout << endl << endl;

	// what the fuck

	cout << "loop begins" << endl;
	for(int i=0;i<10;++i,b1=~b1,b2=b2&2)
	{
		a[i]=i;

		switch(i)
		{
			case 0:i*=2;break;
			case 1:j <<= 3; break;
			case 2:j=f(i);break;
			case 3: g(j); break;
			default:--j;break;
		}

		if(i<=5&&j!=2)
		{
			continue;
		}
		else
		{
			if(j >= 0)
			{
				b1 = false;
			}
		}

		cout << "b1\t\t= " << b1 << endl;
		cout << "b2\t\t= " << b2 << endl;
		cout << "j\t\t= " << j << endl;
		cout << "a\t\t= ";
		for_each(begin(a), end(a),
			[](int i)
			{
				cout << i << " ";
			});
		cout << endl << endl;
	}
	cout << "loop ends" << endl;

	return 0;
}
