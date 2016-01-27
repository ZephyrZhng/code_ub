#include <iostream>
using namespace std;
int main(int argc, char** argv){
	int n;
	cin >> n;
	int N = 2*n;
	for(int t = 0; t < (2*n-1)*2*n; ++t){
		int i = t/N;
		int j = i > (n-1)? (2*(n-1)-i): i;
		int k = n-1-j;
		if(t >= i*N+k && t <= i*N+k+2*j){
			cout << "*";
		} else if((t+1.0)/N - int((t+1)/N) < 1e-5){
			cout << "\n";
		} else{
			cout << " ";
		} 
	}
	return 0;
}