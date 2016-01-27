#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <vector>

#include "pcb.h"
#include "group_tree.h" 

using namespace std;

const int concurrency = 256;

vector<Pcb> pcb_table;
vector<int> ready_queue;
vector<int> blocked_queue;
int id = 0;
int exe_p = -1;
bool schedule = false;
GroupTree group_tree;

template<typename T>
void display(vector<T> v){
	for_each(v.begin(), v.end(),
		[](const T& t){
			cout << t << " ";
		});
}

void stop(){
	exe_p = -1;
}

bool get_pcb(string name, int& i){
	// return false: exists; true: not exists
	// i: index (-1, allocate memory failed)
	auto p = find_if(pcb_table.begin(), pcb_table.end(), 
		[name](const Pcb& pcb){
			return pcb.name == name;
		});
	if(p == pcb_table.end()){
		if(pcb_table.size() + 1 > concurrency){
			i = -1;
			cerr << "Error: utstrip capacity of concurrency, apply for pcb failed!\n";
			return false;
		}else{
			pcb_table.push_back(Pcb());
			i = pcb_table.size() - 1;
			cerr << "Process named \"" << name << "\" not found, create successfully!\n";
			return false;
		}
	}else{
		i = p - pcb_table.begin();
		cerr << "Procdss named \"" << name << "\" already exists.\n";
		return true;
	}
}

bool create(string name, CpuState cpu_state, int priority, MainStore main_store, Resources resources){
	int i = id++;
	if(!get_pcb(name, i) && i != -1){
		pcb_table[i].id = i;
		pcb_table[i].name = name;
		pcb_table[i].cpu_state = cpu_state;
		pcb_table[i].status = ready;
		pcb_table[i].priority = priority;
		pcb_table[i].main_store = main_store;
		pcb_table[i].resources = resources;
		ready_queue.push_back(i);
		group_tree.create(-1, i);
		// group_tree
		cerr << "Apply pcb successfully.\n";
		return true;
	}else{
		cerr << "Error: create process named \"" << name << "\" failed!\n";
		return false;
	}
}

bool remove(int i){
	if(pcb_table[i].status == ready){
		ready_queue.erase(find_if(ready_queue.begin(), ready_queue.end(),
			[i](const int& idx){
				return i == idx;
			}));
		cerr << "Remove pcb[" << i << "] from ready_queue successfully.\n";
		return true;
	}else if(pcb_table[i].status == blocked){
		blocked_queue.erase(find_if(blocked_queue.begin(), blocked_queue.end(),
			[i](const int& idx){
				return i == idx;
			}));
		cerr << "Remove pcb[" << i << "] from blocked_queue successfully.\n";
		return true;
	}else{
		return false;
	}
}

bool kill(int i){
	if(i >= 0){
		if(pcb_table[i].status == running){
			stop();
			schedule = true;
		}
		remove(i);
		// kill subtree in group_tree
		// release main_store and resources
		pcb_table.erase(pcb_table.begin() + i);
		cerr << "Kill pcb[" << i << "] successfully.\n";
		return true;
	}else{
		cerr << "Error: kill pcb[" << i << "] failed!\n";
		return false;
	}
}

bool destroy(string name){
	schedule = false;
	int i;
	if(get_pcb(name, i)){
		kill(i);
		if(schedule){
			// scheduler dispatches
		}
		cerr << "Destroy process named \"" << name << "\" successfully.\n";
		return true;
	}else{
		cerr << "Error: destroy process named \"" << name << "\" failed!\n";
		return false;
	}
}

bool block(){
	int i = exe_p;
	if(exe_p >= 0)
	{
		stop();
		pcb_table[i].status = blocked;
		blocked_queue.push_back(i);
		// scheduler dispatches
		cerr << "Block successfully.\n";
		return true;
	}else{
		cerr << "Error: block failed!\n";
		return false;
	}
}

bool wake_up(string name){
	int i;
	if(get_pcb(name, i)){
		remove(i);
		pcb_table[i].status = ready;
		ready_queue.push_back(i);
		// scheduler dispatches
		cerr << "Wake up process named \"" << name << "\" successfully.\n";
		return true;
	}else{
		cerr << "Error: wake up process named \"" << name << "\" failed!\n";
		return false;
	}
}

bool suspend(string name){
	int i;
	if(get_pcb(name, i)){
		Status s = pcb_table[i].status;
		if(s == running){
			stop();
		}
		// main_store <- pcb_table[i]
		if(s == blocked){
			pcb_table[i].status == blocked_suspend;
		}else{// ready / running
			pcb_table[i].status = ready_suspend;
		}
		if(s == running){
			// scheduler dispatches
		}
		cerr << "Suspend process named \"" << name << "\" successfully.\n";
		return true;
	}else{
		cerr << "Error: suspend process named \"" << name << "\" failed!\n";
		return false;
	}
}

bool activate(string name){
	int i;
	if(get_pcb(name, i)){
		pcb_table[i].status = pcb_table[i].status == ready_suspend? ready: blocked;
		if(pcb_table[i].status == ready){
			// scheduler dispatches
		}
		cerr << "Activate process named \"" << name << "\" successfully.\n";
		return true;
	}else{
		cerr << "Error: activate process named \"" << name << "\" failed!\n";
		return false;
	}
}

bool fork(string parent_name, string child_name){
	int i = id++;
	if(!get_pcb(child_name, i) && i != -1){
		int parent_id;
		get_pcb(parent_name, parent_id);
		pcb_table[i].id = i;
		pcb_table[i].name = child_name;
		pcb_table[i].cpu_state = pcb_table[parent_id].cpu_state;
		pcb_table[i].status = ready;
		pcb_table[i].priority = pcb_table[parent_id].priority;
		pcb_table[i].main_store = pcb_table[parent_id].main_store;
		pcb_table[i].resources = pcb_table[parent_id].resources;
		ready_queue.push_back(i);
		group_tree.create(parent_id, i);
		cerr << "Fork process \"" << parent_name << "\" successfully.\n";
		return true;
	}else{
		cerr << "Error: fork process \"" << parent_name << "\" failed!\n";
		return false;
	}
}

bool scheduler(string cmd, string name){
	if(cmd == "admit"){
		// new -> ready_suspend
		string _cpu_state;
		int priority;
		CpuState cpu_state;

		cout << ">> cpu_state(usr/kernel): ";
		cin >> _cpu_state;
		if(_cpu_state == "usr"){
			cpu_state = usr_state;
		}else if(_cpu_state == "kernel"){
			cpu_state = kernel_state;
		}
		cout << ">> priority(int): ";
		cin >> priority;
		cout << ">> main_store\n";
		cout << ">>> start_addr(int): ";
		int start_addr;
		cin >> start_addr;
		cout << ">>> mem_size(int): ";
		int mem_size;
		cin >> mem_size;
		// resources
		create(name, cpu_state, priority, MainStore(start_addr, mem_size, false), Resources());
		return true;
	}else if(cmd == "rebase"){
		// all -> exit
		int i;
		get_pcb(name, i);
		group_tree.destroy(i);
		destroy(name);
		return true;
	}else if(cmd == "activate"){
		// ready_suspend -> ready
		// blocked_suspend -> blocked
		activate(name);
		return true;
	}else if(cmd == "suspend"){
		// ready -> ready_suspend
		// blocked -> blocked_suspend
		suspend(name);
		return true;
	}else if(cmd == "eventoccurs"){
		int i;
		if(get_pcb(name, i)){
			if(pcb_table[i].status == blocked_suspend){// blocked_suspend -> ready_suspend
				pcb_table[i].status = ready_suspend;
				cerr << "Event occurs, switch process \"" << name 
				     << "\" from blocked_suspend to ready_suspend successfully.\n";
			}else if(pcb_table[i].status == blocked){// blocked -> ready
				wake_up(name);
			}
			return true;
		}else{
			cerr << "Error: schedule process \"" << name << "\" failed when event occurs!\n";
			return true;
		}
	}else if(cmd == "eventwaits"){
		// running -> blocked
		block();
		return true;
	}else if(cmd == "dispatch"){
		// ready -> running
		int i;
		get_pcb(name, i);
		if(get_pcb(name, i)){
			ready_queue.erase(find_if(ready_queue.begin(), ready_queue.end(),
				[i](const int& idx){
					return i == idx;
				}));
			exe_p = i;
			pcb_table[i].status = running;
			cerr << "Running process \"" << name << "\" successfully.\n";
			return true;
		}else{
			cerr << "Error: dispatch process \"" << name << "\" failed!\n";
			return true;
		}
	}else if(cmd == "timeout"){
		// running -> ready
		int i;
		if(get_pcb(name, i)){
			pcb_table[i].status = ready;
			ready_queue.push_back(i);
			stop();
			cerr << "Schedule process \"" << name << "\" successfully when time out.\n";
			return true;
		}else{
			cerr << "Error: schedule process \"" << name << "\" failed when time out!\n";
			return true;
		}
	}else if(cmd == "quit"){
		return false;
	}else if(cmd == "fork"){
		if(exe_p >= 0){
			fork(pcb_table[find_if(pcb_table.begin(), pcb_table.end(),
				[](const Pcb& pcb){
					return pcb.id == exe_p;
				}) - pcb_table.begin()].name, name);
		}
	}else{
		cerr << "Error: undefined command!\n";
		return true;
	}
}

int main(int argc, char** argv){
	// group_tree.create(-1, 0);
	// group_tree.create(-1, 1);
	// group_tree.create(-1, 2);
	// group_tree.create(0, 4);
	// group_tree.create(4, 5);
	// group_tree.create(4, 6);
	// group_tree.create(6, 7);
	// group_tree.create(1, 3);
	// group_tree.preorder_traverse_display();
	// group_tree.destroy(4);
	// group_tree.preorder_traverse_display();
	cout << "command list" << endl
	     << "\tadmit + process name" << endl
         << "\trebase + process name" << endl
         << "\tactivate + process name" << endl
         << "\tsuspend + process name" << endl
         << "\teventoccurs + process name" << endl
         << "\teventwaits + process name" << endl
         << "\tdispatch + process name" << endl
         << "\ttimeout + process name" << endl
         << "\tquit + process name" << endl
         << "\tfork + process name" << endl
         << "> ";
	string cmd, name;
	cin >> cmd >> name;
	while(scheduler(cmd, name)){
		cout << "group_tree:\n"; group_tree.preorder_traverse_display();
		cout << "ready_queue: "; display(ready_queue); cout << endl;
		cout << "blocked_queue: "; display(blocked_queue); cout << endl;
		cout << "exe_p: "; cout << exe_p << endl;
		cout << "> ";
		cin >> cmd >> name;
	}
	return 0;
}
