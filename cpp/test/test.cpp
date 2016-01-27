#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////
typedef struct my_struct{
	int i;
	string s;
	void display(){
		cout << i << " " << s;
	}
}my_struct;

tuple<int, string> my_tuple;

/////////////////////////////////////////////////////////////
template<typename T>
void display(vector<T> v){
	// for(auto& i: v){
	// 	i.display();
	// 	cout << endl;
	// }
	for_each(v.begin(), v.end(),
		[](T t){
			t.display();
			cout << endl;
		});
}

void display_tuple(vector< tuple<int, string> > v_t){
	// for(auto& i: v_t){
	// 	cout << get<0>(i) << " " 
	// 		 << get<1>(i) << endl;
	// }
	for_each(v_t.begin(), v_t.end(),
		[](const tuple<int, string>& t){
			cout << get<0>(t) << " "
				 << get<1>(t) << endl;
		});
}

/////////////////////////////////////////////////////////////
class reload_operator{
public:
	bool operator ()(const my_struct& a, const my_struct& b){
		return (a.i <= b.i);
	}
};

class reload_operator_tuple{
public:
	bool operator ()(const tuple<int, string>& a, const tuple<int, string>& b){
		return (get<0>(a) <= get<0>(b));
	}
};

/////////////////////////////////////////////////////////////
struct functor: public binary_function<my_struct, string, bool>{
public:
	bool operator ()(const my_struct a, const string str) const {
		return (a.s == str);
	}
};

struct functor_tuple: public binary_function< tuple<int, string>, string, bool >{
public:
	bool operator ()(const tuple<int, string> a, const string str) const {
		return (get<1>(a) == str);
	}
};

/////////////////////////////////////////////////////////////
vector<string> split(const string& s, char delim)
{
	stringstream ss(s);
	vector<string> elems;
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

/*
	template <typename T>
	string to_string(T value)
	{
		ostringstream os;
		os << value;
		return os.str();
	}
*/

void out(string str, initializer_list<string> s, initializer_list<int> i)
{
	cout << str << endl;
	auto p = s.begin();
	auto q = i.begin();
	for(; p != s.end(), q != i.end(); ++p, ++q)
	{
		cout << *q << ": " << *p << endl;
	}
}

template<typename T>
void displayVec(const vector<T>& vec)
{
	cout << "{";
	for(size_t i = 0; i < vec.size(); ++i)
	{
		cout << vec[i];
		if(i != vec.size() - 1)
		{
			cout << ",";
		}
	}
	cout << "}";
}

template<typename T>
int BinSearch(vector<T> v, int key) // v is sorted from small to big
{
	int low = 0;
	int high = v.size() - 1;
	int mid;
	while(low <= high)
	{
		mid = (low + high) / 2;
		if(v[mid] == key) return mid;
		else if(v[mid] < key) low = mid + 1;
		else high = mid - 1;
	}
	return -1; // key should be inserted before mid
			   // however, if key > vmax, key needs to be inserted after mid
}

/////////////////////////////////////////////////////////////
void test_initializer_list(initializer_list<int> list)
{
	cout << "test_initializer_list: ";
	for(auto& itr: list)
	{
		cout << itr << " ";
	}
	cout << endl;
}

/////////////////////////////////////////////////////////////
template<class a, class b>
auto add(a mya, b myb)->decltype(mya + myb)
{
	return mya + myb;
}

/////////////////////////////////////////////////////////////
template <typename First, typename Second, int Third>
class SomeType;

template <typename Second>
using TypedefName = SomeType<float, Second, 5>;

/////////////////////////////////////////////////////////////
template<class Integral>
Integral foo(Integral x, Integral y) {
	static_assert(std::is_integral<Integral>::value, "foo() parameter must be an integral type.");
}

/////////////////////////////////////////////////////////////
// This function will obtain a reference to the parameter 'r' and increment it.
void myfunc (int &r)  
{ 
	r++; 
}

// Template function.
template<class F, class P> 
void gfunc (F f, P t)  
{ 
	f(t); 
}

int main()
{
	vector<my_struct> v = {
		{3, "i"},
		{2, "fuck"},
		{1, "you"}
	};

	vector< tuple<int, string> > v_t = {
		make_tuple(3, "i"),
		make_tuple(2, "fuck"),
		make_tuple(1, "you")
	};

	/////////////////////////////////////////////////////////////
	vector<my_struct> v1(v);
	sort(v1.begin(), v1.end(), reload_operator());
	cout << "sorted v1:" << endl; display(v1);

	vector< tuple<int, string> > v1_t(v_t);
	sort(v1_t.begin(), v1_t.end(), reload_operator_tuple());
	cout << "sorted v1_t:" << endl; display_tuple(v1_t);

	/////////////////////////////////////////////////////////////
	vector<my_struct> v2(v);
	sort(v2.begin(), v2.end(), [](const my_struct& a, const my_struct& b){
		return (a.i <= b.i);
	});
	cout << "sorted v2:" << endl; display(v2);

	vector< tuple<int, string> > v2_t(v_t);
	sort(v2_t.begin(), v2_t.end(), [](const tuple<int, string>& a, const tuple<int, string>& b){
		return (get<0>(a) <= get<0>(b));
	});
	cout << "sorted v2_t:" << endl; display_tuple(v2_t);

	/////////////////////////////////////////////////////////////
	vector<my_struct> v3(v);
	function<bool(const my_struct& a, const my_struct& b)> func = [](const my_struct& a, const my_struct& b){
		return (a.i <= b.i);
	};
	sort(v3.begin(), v3.end(), func);
	cout << "sorted v3:" << endl; display(v3);

	vector< tuple<int, string> > v3_t(v_t);
	function<bool(const tuple<int, string>& a, const tuple<int, string>& b)> func_t = [](const tuple<int, string>& a, const tuple<int, string>& b){
		return (get<0>(a) <= get<0>(b));
	};
	sort(v3_t.begin(), v3_t.end(), func_t);
	cout << "sorted v3_t" << endl; display_tuple(v3_t);

	/////////////////////////////////////////////////////////////
	functor my_functor;
	cout << "find by functor: "; (*(find_if(v.begin(), v.end(), bind2nd(my_functor, "fuck")))).display(); cout << endl;

	functor_tuple my_functor_tuple;
	tuple<int, string> res = *(find_if(v_t.begin(), v_t.end(), bind2nd(my_functor_tuple, "fuck")));
	cout << "find by functor_tuple: "
		 << get<0>(res) << " "
		 << get<1>(res) << endl;

	/////////////////////////////////////////////////////////////
	vector<string> split_v = split("i fuck your mother", ' ');
	cout << "split:" << endl;
	for(auto& p: split_v){
		cout << p << endl;
	}

	cout << "to_string: " << to_string(233) << endl;

	/////////////////////////////////////////////////////////////
	out("i wan to fuck:", {"you", "your mother", "your father"}, {1, 2, 3});

	vector<int> vint = {1,2,3,4,3,2,1};
	displayVec(vint); cout << endl;

	vector<int> vbs = {2,4,6,8,10};
	cout << BinSearch(vbs, 0) << BinSearch(vbs, 3) << BinSearch(vbs, 11) << BinSearch(vbs, 6) << endl;
	vbs = {1};
	vbs.insert(vbs.begin() + 1, 11);
	displayVec(vbs); cout << endl;

	int i = 5;
	while(--i)
	{
		cout << "loop" << i << endl;
		switch(i)
		{
			case 0: cout << "0\n";
			case 1: cout << "1\n";
			case 2: cout << "2\n";
			case 3: cout << "3\n";
			case 4: cout << "4\n";
			case 5: cout << "5\n";
		}
	}

	/////////////////////////////////////////////////////////////
	test_initializer_list({1, 2});
	test_initializer_list({1, 2, 3});

	/////////////////////////////////////////////////////////////
	auto type1 = 1;
	decltype(type1) type2 = type1 + 2;
	cout << "type1 = " << type1 << " "
		 << "type2 = " << type2 << endl; 

	/////////////////////////////////////////////////////////////
	int intadder1 = 1;
	int intadder2 = 2;
	float floatadder1 = 1.0;
	double doubleadder2 = 2.0;
	cout << "add: (int) " << intadder1 << " + (int) " << intadder2 << " = " << add(intadder1, intadder2) << endl
		 << "add: (float) " << floatadder1 << " + (double) " << doubleadder2 << " = " << add(floatadder1, doubleadder2) << endl;


	/////////////////////////////////////////////////////////////
	std::string s ("this subject has a submarine as a subsequence");
	std::smatch m;
	std::regex e ("\\b(sub)([^ ]*)");   // matches words beginning by "sub"

	std::cout << "Target sequence: " << s << std::endl;
	std::cout << "Regular expression: /\\b(sub)([^ ]*)/" << std::endl;
	std::cout << "The following matches and submatches were found:" << std::endl;

	while (std::regex_search (s,m,e)) {
		for (auto x:m) std::cout << x << " ";
		std::cout << std::endl;
		s = m.suffix().str();
	}

	/////////////////////////////////////////////////////////////
	std::uniform_int_distribution<int> distribution(0, 99);
	std::mt19937 engine; // Mersenne twister MT19937
	int randomnum;
	for (int i = 0; i < 10; ++i) {
		randomnum = distribution(engine); // pseudo-random number, the same every time executed
		cout << randomnum << " ";
	}
	cout << endl;

	/////////////////////////////////////////////////////////////
	int myi = 0;
	gfunc (myfunc, myi); // 'g<void (int &r), int>' is instantiated
				 // then 'i' will not be modified.
	std::cout << myi << std::endl; // Output -> 0

	gfunc (myfunc, std::ref(myi)); // 'g<void(int &r),reference_wrapper<int>>' is instantiated
						   // then 'i' will be modified.
	std::cout << myi << std::endl; // Output -> 1

	return 0;
}
