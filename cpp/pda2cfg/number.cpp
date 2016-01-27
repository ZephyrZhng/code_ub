#include "number.h"

Number::Number()
{
	
}

Number::Number(int _r): r(_r)
{
	num.push_back(0);
}

void operator ++(Number& n)
{
	int carry = 0;
	n.num[n.num.size() - 1] += 1;
	if(n.num[n.num.size() - 1] >= n.r)
	{
		n.num[n.num.size() - 1] -= n.r;
		carry = 1;
	}
	for(int i = n.num.size() - 2; i >= 0 && carry != 0; --i)
	{
		n.num[i] += carry;
		carry = 0;
		if(n.num[i] >= n.r)
		{
			n.num[i] -= n.r;
			carry = 1;
		}
	}
	if(carry != 0)
	{
		n.num.insert(n.num.begin(), 1);
	}
}

void Number::setLen(int len)
{
	while(num.size() < len)
	{
		num.insert(num.begin(), 0);
	}
}

bool Number::isMax()
{
	for(int i = 0; i < num.size(); ++i)
	{
		if(num[i] != r - 1)
		{
			return false;
		}
	}
	return true;
}

void Number::display()
{
	for_each(num.begin(), num.end(), [](const int& i){ cout << i; });
}

