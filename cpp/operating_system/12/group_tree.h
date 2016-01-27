#ifndef GROUP_TREE_H
#define GROUP_TREE_H

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

#include "pcb.h"

using namespace std;

struct Node{
	Node(){}
	Node(int n, Node* pp, vector<Node*> cpl):
	pcb_id(n), parent_ptr(pp), child_ptr_list(cpl){}

	int pcb_id;
	Node* parent_ptr;
	vector<Node*> child_ptr_list;
};

class GroupTree{
public:
	GroupTree(){
		root = new Node();
		root->pcb_id = -1;
	}

	void destruct(Node* p){
		if(p != nullptr){
			for(size_t i = 0; i < p->child_ptr_list.size(); ++i){
				destruct(p->child_ptr_list[i]);
			}
			delete p;
		}
	}

	~GroupTree(){
		destruct(root);
		// cout << "destruct\n";
	}

	void find(int i, Node* p, bool& found, Node*& pp){
		if(p != nullptr){
			if(p->pcb_id == i){
				found = true;
				pp = p;
				return;
			}else{
				for(size_t j = 0; j < p->child_ptr_list.size(); ++j){
					find(i, p->child_ptr_list[j], found, pp);
				}
			}
		}
	}

	bool create(int i, int pcb_id){
		Node* pp;
		bool found = false;
		find(i, root, found, pp);
		// if(pp != nullptr){
		// 	cout << "find " << i << " " << pp->pcb_id << endl;
		// }else{
		// 	cout << "find null\n";
		// }
		if(found){
			Node* np = new Node();
			np->pcb_id = pcb_id;
			np->parent_ptr = pp;
			pp->child_ptr_list.push_back(np);
		}
	}

	bool destroy(int i){
		Node* p;
		bool found = false;
		find(i, root, found, p);
		if(found){
			p->parent_ptr->child_ptr_list.erase(
				find_if(
					p->parent_ptr->child_ptr_list.begin(), p->parent_ptr->child_ptr_list.end(),
					[i](Node* cp){
						return cp->pcb_id == i;
					}));
			destruct(p);
		}
	}

	void predisp(vector<int>& stk, Node* p, bool b1){
		if(p != nullptr){
			for(int i = 0; i < stk.size(); ++i){
				cout << (stk[i] == 0? " ": "|") << "\t";
			}
			cout << "\\____ ";
			// display node
			cout << "id: " << p->pcb_id;
			cout << "\n";
			for(size_t i = 0; i < p->child_ptr_list.size(); ++i){
				if(b1){
					stk.push_back(0);
				}else{
					stk.push_back(1);
				}
				predisp(stk, p->child_ptr_list[i], i == p->child_ptr_list.size() - 1);
				stk.erase(stk.end() - 1);
			}
		}
	}

	void preorder_traverse_display(){
		vector<int> stk;
		bool b = true; // show whether my parent is the last child of my grand parent
		predisp(stk, root, b);
	}

	Node* root;
private:
};

#endif // GROUP_TREE_H
