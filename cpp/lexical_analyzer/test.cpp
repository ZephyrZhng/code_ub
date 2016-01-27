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

	char _a1_       ='a';
	char b__22      = '\n';
	char __3_333___ = '\t';
	char _a_        = '\\';
	char aa         = '\'';
	char bbb        = '\"';
	char s[] = "abc\n\t\\\'\"-=~!@#$%^&*()_+";
	double d=-3.14e-10;

	bool b1=true;
	bool b2=false;
	int j=2;
	int a[10];

	// what the fuck

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
	}

	return 0;
}
