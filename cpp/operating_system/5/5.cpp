#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <wait.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

const int total_instruction = 10;
const int mem_frame_num = 4;
const int access_frame_num = 5;

template<typename T>
void disp(const vector<T>& vec){
	for_each(vec.begin(), vec.end(),
		[](const T& t){
			cout << t << " ";
		});
}

int main(int argc, char** argv){
	vector<int> access_series(total_instruction);
	srand(time(0));
	for(int i = 0; i < access_series.size(); ++i){
		access_series[i] = rand() % access_frame_num + 1;
	}
	cout << "access_series: ";
	disp(access_series);
	cout << endl;
	vector<int> fifo, lru;
	int p1, p2;
	while((p1 = fork()) == -1);
	if(p1 == 0){
		int disaffect = 0;
		double hit = 0;
		for(int i = 0; i < access_series.size(); ++i){
			cout << "lru\taccess frame " << access_series[i] << ";\t";
			auto pos = find(lru.begin(), lru.end(), access_series[i]);
			if(pos == lru.end()){
				if(lru.size() >= mem_frame_num){
					lru.erase(lru.begin());
				}
				lru.push_back(access_series[i]);
				++disaffect;
				cout << "disaffect = " << disaffect << "\t";
				disp(lru); cout << endl;
			}else{
				lru.erase(pos);
				lru.push_back(access_series[i]);
				cout << "\t\t";
				disp(lru);
				cout << endl;
			}
		}
		hit = 1 - disaffect * 1.0 / total_instruction;
		cout << "lru\thit = " << hit << endl;
		exit(0);
	}
	while((p2 = fork()) == -1);
	if(p2 == 0){
		int disaffect = 0;
		double hit = 0;
		for(int i = 0; i < access_series.size(); ++i){
			cout << "fifo\taccess frame " << access_series[i] << ";\t";
			auto pos = find(fifo.begin(), fifo.end(), access_series[i]);
			if(pos == fifo.end()){
				if(fifo.size() >= mem_frame_num){
					fifo.erase(fifo.begin());
				}
				fifo.push_back(access_series[i]);
				++disaffect;
				cout << "disaffect = " << disaffect << "\t";
				disp(fifo); cout << endl;
			}else{
				cout << "\t\t";
				disp(fifo);
				cout << endl;
			}
		}
		hit = 1 - disaffect * 1.0 / total_instruction;
		cout << "fifo\thit = " << hit << endl;
		exit(0);
	}
	return 0;
}