#include "trie.h"

Node::Node()
{

}

Node::Node(const char& ch): val(ch), terminal(false)
{

}

Trie::Trie()
{
	root = new Node('$');
	curPtr = root;
}

void Trie::destruct(Node* p)
{
	if(p != nullptr)
	{
		for(size_t i = 0; i < p->sons.size(); ++i)
		{
			destruct(p->sons[i]);
		}

		delete p;
	}
}

Trie::~Trie()
{
	destruct(root);
}

void Trie::insert(const string& str)
{
	bool existed = false;
	Node* ptr = root;
	auto itr = str.begin();
	bool found = true;
	int mid = 0;

	while(found)
	{
		found = false;
		mid = 0;
		
		int low = 0;
		int high = ptr->sons.size() - 1;
		while(low <= high)
		{
			mid = (low + high) / 2;
			if(ptr->sons[mid]->val == *itr)
			{
				found = true;
				ptr = ptr->sons[mid];
				++itr;

				if(itr == str.end())
				{
					existed = true;
					goto next;
				}

				goto result;
			}
			else if(ptr->sons[mid]->val < *itr) low = mid + 1;
			else high = mid - 1;
		}

		result:
		if(!found)
		{
			break;
		}
	}

	next:
	if(!existed)
	{
		if(ptr->sons.empty())
		{
			mid = 0;
		}
		else if(ptr->sons[mid]->val < *itr)
		{
			++mid;
		}

		ptr->sons.insert(ptr->sons.begin() + mid, new Node(*itr));
		if(itr == str.end() - 1)
		{
			ptr->sons[mid]->terminal = true;
		}
		ptr = ptr->sons[mid];
		++itr;

		while(itr != str.end())
		{
			ptr->sons.push_back(new Node(*itr));
			if(itr == str.end() - 1)
			{
				ptr->sons[ptr->sons.size() - 1]->terminal = true;
			}
			ptr = ptr->sons[ptr->sons.size() - 1];
			++itr;
		}
	}
	else
	{
		ptr->terminal = true;
	}
}

bool Trie::match(char ch)
{
	for(size_t i = 0; i < curPtr->sons.size(); ++i)
	{
		if(curPtr->sons[i]->val == ch)
		{
			curPtr = curPtr->sons[i];
			return true;
		}
	}
	return false;
}

void Trie::resetCurPtr()
{
	curPtr = root;
}

void Trie::preorder_traverse(Node* p)
{
	if(p != nullptr)
	{
		cout << p->val;
		if(p->terminal)
		{
			cout << "(terminal)";
		}
		cout << " ";

		for(size_t i = 0; i < p->sons.size(); ++i)
		{
			preorder_traverse(p->sons[i]);
		}
	}
}

void Trie::postorder_traverse(Node* p)
{
	if(p != nullptr)
	{
		for(size_t i = 0; i < p->sons.size(); ++i)
		{
			postorder_traverse(p->sons[i]);
		}

		cout << p->val;
		if(p->terminal)
		{
			cout << "(terminal)";
		}
		cout << " ";
	}
}

void Trie::preorderTraverse()
{
	cout << "preorder traverse: ";
	preorder_traverse(root);
	cout << endl;
}

void Trie::postorderTraverse()
{
	cout << "postorder traverse: ";
	postorder_traverse(root);
	cout << endl;
}

void Trie::preorder_traverse_display(vector<int>& stk, Node* p, bool b1)
{
	if(p != nullptr)
	{
		for(size_t i = 0; i < stk.size(); ++i)
		{
			cout << ((stk[i] == 0)? " ": "|") << "\t";
		}
		cout << "|------ " << p->val << endl;

		for(size_t i = 0; i < p->sons.size(); ++i)
		{
			if(b1)
			{
				stk.push_back(0);
			}
			else
			{
				stk.push_back(1);
			}
			preorder_traverse_display(stk, p->sons[i], i == p->sons.size() - 1);
			stk.erase(stk.end() - 1);
		}
	}
}

void Trie::display()
{
	vector<int> stk;
	bool b = true; // show whether my parent is the last child of my grand parent
	preorder_traverse_display(stk, root, b);
}