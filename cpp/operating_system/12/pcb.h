#ifndef PCB_H
#define PCB_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <fstream>
#include <functional>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

enum CpuState{usr_state, kernel_state};
enum Status{creat, ready, ready_suspend, blocked, blocked_suspend, running, ex};
struct MainStore{
	MainStore(){}
	MainStore(int _start_addr, int _mem_size, bool _usable):
	start_addr(_start_addr), mem_size(_mem_size), usable(_usable){}

	int start_addr;
	int mem_size;
	bool usable;
};
struct Resources{
	Resources(){}
	vector<string> opened_files;
};

class Pcb{
public:
	Pcb(){}

// description
	int id;
	string name;
	string usr_id;
	// group_tree

// ctrl_infl
	CpuState cpu_state;
	Status status;
	int priority;
	// int entry_addr;
	// int disk_addr;
	// StatInfo stat_info;

// resources
	// main_store
	MainStore main_store;
	Resources resources;

// cpu_scene

private:
};

#endif // PCB_H
