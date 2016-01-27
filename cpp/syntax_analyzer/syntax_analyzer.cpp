#include "syntax_analyzer.h"

#include "../grammar_ch/grammar.cpp"

string ch2str(char ch)
{
	string str;
	str.insert(str.begin(), ch);
	return str;
}

LL1Item::LL1Item(unsigned int myI, unsigned int myJ): i(myI), j(myJ)
{

}

bool operator <(const LR0Item& i1, const LR0Item& i2)
{
	return i1.lProductionRuleIdx < i2.lProductionRuleIdx?
		true: (i1.lProductionRuleIdx > i2.lProductionRuleIdx?
			false: (i1.rProductionRuleIdx < i2.rProductionRuleIdx?
				true: (i1.rProductionRuleIdx > i2.rProductionRuleIdx?
					false: (i1.dotPos < i2.dotPos))));
}

bool operator ==(const LR0Item& i1, const LR0Item& i2)
{
	return i1.lProductionRuleIdx == i2.lProductionRuleIdx
		&& i1.rProductionRuleIdx == i2.rProductionRuleIdx
		&& i1.dotPos == i2.dotPos;
}

bool operator >(const LR0Item& i1, const LR0Item& i2)
{
	return !(i1 < i2) && !(i1 == i2);
}

bool operator !=(const LR0Item& i1, const LR0Item& i2)
{
	return !(i1 == i2);
}


LR0Item::LR0Item(unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myDotPos):
	lProductionRuleIdx(myLProductionRuleIdx), rProductionRuleIdx(myRProductionRuleIdx), dotPos(myDotPos)
{

}

LR0ActionTabItem::LR0ActionTabItem(const Action& myAction, unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myItemSetIdx):
	action(myAction), lProductionRuleIdx(myLProductionRuleIdx), rProductionRuleIdx(myRProductionRuleIdx), itemSetIdx(myItemSetIdx)
{

}

bool operator <(const LR1Item& i1, const LR1Item& i2)
{
	return i1.lProductionRuleIdx < i2.lProductionRuleIdx?
		true: (i1.lProductionRuleIdx > i2.lProductionRuleIdx?
			false: (i1.rProductionRuleIdx < i2.rProductionRuleIdx?
				true: (i1.rProductionRuleIdx > i2.rProductionRuleIdx?
					false: (i1.dotPos < i2.dotPos?
						true: (i1.lookaheadSymbol < i2.lookaheadSymbol)))));
}

bool operator ==(const LR1Item& i1, const LR1Item& i2)
{
	return i1.lProductionRuleIdx == i2.lProductionRuleIdx
		&& i1.rProductionRuleIdx == i2.rProductionRuleIdx
		&& i1.dotPos == i2.dotPos
		&& i1.lookaheadSymbol == i2.lookaheadSymbol;
}

bool operator >(const LR1Item& i1, const LR1Item& i2)
{
	return !(i1 < i2) && !(i1 == i2);
}

bool operator !=(const LR1Item& i1, const LR1Item& i2)
{
	return !(i1 == i2);
}

LR1Item::LR1Item(const LR0Item& myLR0Item, char myLookaheadSymbol):
	lProductionRuleIdx(myLR0Item.lProductionRuleIdx), rProductionRuleIdx(myLR0Item.rProductionRuleIdx), dotPos(myLR0Item.dotPos),
	lookaheadSymbol(myLookaheadSymbol)
{

}

LR1Item::LR1Item(unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myDotPos, char myLookaheadSymbol):
	lProductionRuleIdx(myLProductionRuleIdx), rProductionRuleIdx(myRProductionRuleIdx), dotPos(myDotPos), lookaheadSymbol(myLookaheadSymbol)
{

}

LR1ActionTabItem::LR1ActionTabItem(const Action& myAction, unsigned int myLProductionRuleIdx, unsigned int myRProductionRuleIdx, unsigned int myItemSetIdx):
	action(myAction), lProductionRuleIdx(myLProductionRuleIdx), rProductionRuleIdx(myRProductionRuleIdx), itemSetIdx(myItemSetIdx)
{

}

SyntaxAnalyzer::SyntaxAnalyzer(const string& filePath): G(Grammar(filePath)), expanded(false)
{

}

SyntaxAnalyzer::~SyntaxAnalyzer()
{

}

void SyntaxAnalyzer::display(const LR0Item& item)
{
	cout << G.P[item.lProductionRuleIdx].lPart;
	cout << "->";
	for(unsigned int i = 0; i < item.dotPos; ++i)
	{
		cout << G.P[item.lProductionRuleIdx].rPartSet[item.rProductionRuleIdx][i];
	}
	cout << ".";
	for(unsigned int i = item.dotPos; i < G.P[item.lProductionRuleIdx].rPartSet[item.rProductionRuleIdx].size(); ++i)
	{
		cout << G.P[item.lProductionRuleIdx].rPartSet[item.rProductionRuleIdx][i];
	}
}

void SyntaxAnalyzer::display(const LR1Item& item)
{
	cout << "[";
	display(LR0Item(item.lProductionRuleIdx, item.rProductionRuleIdx, item.dotPos));
	cout << "," << item.lookaheadSymbol << "]";
}

void SyntaxAnalyzer::display(const set<LR0Item>& I)
{
	for_each(I.begin(), I.end(),
		[this](const LR0Item& item)
		{
			display(item);
			cout << endl;
		});
}

void SyntaxAnalyzer::display(const set<LR1Item>& I)
{
	for_each(I.begin(), I.end(),
		[this](const LR1Item& item)
		{
			display(item);
			cout << endl;
		});
}

void SyntaxAnalyzer::expandGrammar()
{
	if(!expanded)
	{
		G.P.push_back(ProductionRule('S', vector<string>({ch2str(G.S)})));
		sort(G.P.begin(), G.P.end(),
			[](const ProductionRule& pr1, const ProductionRule& pr2)
			{
				return pr1.lPart < pr2.lPart?
					true: (pr1.lPart > pr2.lPart?
						false: pr1.rPartSet[0] < pr2.rPartSet[0]);
			});

		G.VN.push_back('S');
		sort(G.VN.begin(), G.VN.end());

		G.S = 'S';

		G.nullable.clear();
		G.first.clear();
		G.follow.clear();
		G.nullable.assign(G.VN.size(), false);
		G.first.assign(G.VN.size() + G.VT.size(), set<char>());
		G.follow.assign(G.VN.size(), set<char>());

		G.display();

		G.calNullableFirstFollow();

		expanded = true;
	}
}

set<LR1Item> SyntaxAnalyzer::LR1Closure(const set<LR1Item>& I)
{
	set<LR1Item> ret;
	for(auto ptr = I.begin(); ptr != I.end(); ++ptr)
	{
		ret.insert(*ptr);
	}

	bool updated = true;
	while(updated)
	{
		updated = false;

		for(auto ptr = ret.begin(); ptr != ret.end(); ++ptr)
		{
			if(ptr->dotPos < G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx].size()
				&& G.isNonTerminal(G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos]))
			{
				ProductionRule pr;
				int prIdx = G.getProductionRuleByLeft(pr, G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos]);
				for(size_t i = 0; i < pr.rPartSet.size(); ++i)
				{
					string str;
					if(ptr->dotPos + 1 < G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx].size())
					{
						str = G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos + 1] + ch2str(ptr->lookaheadSymbol);
					}
					else
					{
						str = ch2str(ptr->lookaheadSymbol);
					}
					set<char> s = G.getFirst(str);
					int oldSize = ret.size();
					for(auto& sPtr: s)
					{
						if(sPtr != 'e')
						{
							ret.insert(LR1Item(prIdx, i, 0, sPtr));
						}
					}
					int newSize = ret.size();
					if(oldSize != newSize)
					{
						updated = true;
					}
				}
			}
		}
	}

	// cout << "LR1Closure:\n"; display(ret); cout << endl;

	return ret;
}

set<LR1Item> SyntaxAnalyzer::LR1GoTo(const set<LR1Item>& I, char X)
{
	if(find(G.VN.begin(), G.VT.begin(), X) != G.VN.end()
		|| find(G.VT.begin(), G.VT.end(), X) != G.VT.end())
	{
		set<LR1Item> ret;

		for(auto ptr = I.begin(); ptr != I.end(); ++ptr)
		{
			if(ptr->dotPos < G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx].size()
				&& G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos] == X)
			{
				ret.insert(LR1Item(ptr->lProductionRuleIdx, ptr->rProductionRuleIdx, ptr->dotPos + 1, ptr->lookaheadSymbol));
			}
		}

		// cout << "LR1GoTo:\n"; display(LR1Closure(ret)); cout << endl;
		
		return LR1Closure(ret);
	}
	else
	{
		return set<LR1Item>();
	}
}

void SyntaxAnalyzer::calLR1ItemSetFamily()
{
	expandGrammar();

	lr1ItemSetFamily.push_back(
		LR1Closure(
			set<LR1Item>({
				LR1Item(
					find_if(
						G.P.begin(), G.P.end(),
						[](const ProductionRule& pr)
						{
							return pr.lPart == 'S';
						}) - G.P.begin(),
					0, 0, '$'
					)
				})
			)
		);

	bool updated = true;
	while(updated)
	{
		updated = false;

		for(auto& itemSetPtr: lr1ItemSetFamily)
		{
			for(auto& symbolPtr: G.VN)
			{
				set<LR1Item> itemSet = LR1GoTo(itemSetPtr, symbolPtr);
				if(!itemSet.empty()
					&& find_if(lr1ItemSetFamily.begin(), lr1ItemSetFamily.end(),
						[itemSet](const set<LR1Item>& s)
						{
							if(s.size() != itemSet.size())
							{
								return false;
							}
							else
							{
								auto ptr1 = itemSet.begin();
								auto ptr2 = s.begin();
								for(; ptr1 != itemSet.end(); ++ptr1, ++ptr2)
								{
									if(!(*ptr1 == *ptr2))
									{
										return false;
									}
								}
								return true;
							}
						}) == lr1ItemSetFamily.end())
				{
					lr1ItemSetFamily.push_back(itemSet);
					updated = true;
				}
			}

			for(auto& symbolPtr: G.VT)
			{
				set<LR1Item> itemSet = LR1GoTo(itemSetPtr, symbolPtr);
				if(!itemSet.empty()
					&& find_if(lr1ItemSetFamily.begin(), lr1ItemSetFamily.end(),
						[itemSet](const set<LR1Item>& s)
						{
							if(s.size() != itemSet.size())
							{
								return false;
							}
							else
							{
								auto ptr1 = itemSet.begin();
								auto ptr2 = s.begin();
								for(; ptr1 != itemSet.end(); ++ptr1, ++ptr2)
								{
									if(!(*ptr1 == *ptr2))
									{
										return false;
									}
								}
								return true;
							}
						}) == lr1ItemSetFamily.end())
				{
					lr1ItemSetFamily.push_back(itemSet);
					updated = true;
				}
			}
		}
	}

	// sort lr1ItemSetFamily
	sort(lr1ItemSetFamily.begin(), lr1ItemSetFamily.end(),
		[](const set<LR1Item>& itemSet1, const set<LR1Item>& itemSet2)
		{
			auto ptr1 = itemSet1.begin();
			auto ptr2 = itemSet2.begin();
			for(; ptr1 != itemSet1.end() && ptr2 != itemSet2.end(); ++ptr1, ++ptr2)
			{
				if(*ptr1 > *ptr2)
				{
					return false;
				}
				else if(*ptr1 < *ptr2)
				{
					return true;
				}
			}
			if(ptr1 != itemSet1.end() && ptr2 == itemSet2.end())
			{
				return false;
			}
			else
			{
				return true;
			}
		});

	/*cout << "item set family" << endl;
	for(size_t i = 0; i < lr1ItemSetFamily.size(); ++i)
	{
		cout << "I" << i << endl;
		display(lr1ItemSetFamily[i]);
		cout << endl;
	}

	cout << "Goto graph" << endl;
	for(size_t i = 0; i < lr1ItemSetFamily.size(); ++i)
	{
		for(size_t j = 0; j < G.VT.size(); ++j)
		{
			set<LR1Item> Ij = LR1GoTo(lr1ItemSetFamily[i], G.VT[j]);
			if(!Ij.empty())
			{
				cout << "I" << i << endl;
				display(lr1ItemSetFamily[i]);
				cout << "receives " << G.VT[j] << ", goes to" << endl;
				display(Ij);
				cout << endl;
			}
		}

		for(size_t j = 0; j < G.VN.size(); ++j)
		{
			set<LR1Item> Ij = LR1GoTo(lr1ItemSetFamily[i], G.VN[j]);
			if(!Ij.empty())
			{
				cout << "I" << i << endl;
				display(lr1ItemSetFamily[i]);
				cout << "receives " << G.VN[j] << ", goes to" << endl;
				display(Ij);
				cout << endl;
			}
		}
	}*/
}

void SyntaxAnalyzer::buildLR1ParseTab()
// notice that the lookahead symbol may be e!!!
{
	lr1ActionTab.assign(lr1ItemSetFamily.size(), vector<LR1ActionTabItem>(G.VT.size() + 1, LR1ActionTabItem(ERR, 0, 0, 0)));
	lr1GoToTab.assign(lr1ItemSetFamily.size(), vector<unsigned int>(G.VN.size(), lr1ItemSetFamily.size()));
	// if an item in lr1GoToTab equals to the value of lr1ItemSetFamily.size(), it means this item is invalid

	for(size_t i = 0; i < lr1ItemSetFamily.size(); ++i)
	{
		for(auto itemSetPtr = lr1ItemSetFamily[i].begin(); itemSetPtr != lr1ItemSetFamily[i].end(); ++itemSetPtr)
		{
			auto VTPtr = find(G.VT.begin(), G.VT.end(), itemSetPtr->lookaheadSymbol);
			auto VNPtr = find(G.VN.begin(), G.VN.end(), itemSetPtr->lookaheadSymbol);
			if(itemSetPtr->dotPos < G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx].size())
			{
				if(G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx] == "e")
				{
					lr1ActionTab[i][(VTPtr == G.VT.end())? G.VT.size(): (VTPtr - G.VT.begin())] =
						LR1ActionTabItem(REDUCE, itemSetPtr->lProductionRuleIdx, itemSetPtr->rProductionRuleIdx, 0);
				}

				auto ptr = find(G.VT.begin(), G.VT.end(),
					G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx][itemSetPtr->dotPos]);
				if(ptr != G.VT.end())
				{
					set<LR1Item> Ij = LR1GoTo(lr1ItemSetFamily[i],
						G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx][itemSetPtr->dotPos]);
					int lr1ActionTabCol = ptr - G.VT.begin();
					lr1ActionTab[i][lr1ActionTabCol] = LR1ActionTabItem(
						SHIFT, 0, 0,
						find_if(lr1ItemSetFamily.begin(), lr1ItemSetFamily.end(),
							[Ij](const set<LR1Item>& s)
							{
								if(s.size() != Ij.size())
								{
									return false;
								}
								else
								{
									auto ptr1 = Ij.begin();
									auto ptr2 = s.begin();
									for(; ptr1 != Ij.end(); ++ptr1, ++ptr2)
									{
										if(!(*ptr1 == *ptr2))
										{
											return false;
										}
									}
									return true;
								}
							}) - lr1ItemSetFamily.begin()
						);
				}
			}
			else
			{
				if(G.P[itemSetPtr->lProductionRuleIdx].lPart != 'S')
				{
					int lr1ActionTabCol = (VTPtr == G.VT.end())? G.VT.size(): (VTPtr - G.VT.begin());
					lr1ActionTab[i][lr1ActionTabCol] = LR1ActionTabItem(REDUCE, itemSetPtr->lProductionRuleIdx, itemSetPtr->rProductionRuleIdx, 0);
				}
				else
				{
					lr1ActionTab[i][G.VT.size()] = LR1ActionTabItem(ACCEPT, 0, 0, 0);
				}
			}
		}
	}

	for(size_t i = 0; i < lr1ItemSetFamily.size(); ++i)
	{
		for(size_t A = 0; A < G.VN.size(); ++A)
		{
			set<LR1Item> Ij = LR1GoTo(lr1ItemSetFamily[i], G.VN[A]);
			lr1GoToTab[i][A] = find_if(lr1ItemSetFamily.begin(), lr1ItemSetFamily.end(),
				[Ij](const set<LR1Item>& s)
				{
					if(s.size() != Ij.size())
					{
						return false;
					}
					else
					{
						auto ptr1 = Ij.begin();
						auto ptr2 = s.begin();
						for(; ptr1 != Ij.end(); ++ptr1, ++ptr2)
						{
							if(!(*ptr1 == *ptr2))
							{
								return false;
							}
						}
						return true;
					}
				}) - lr1ItemSetFamily.begin();
		}
	}

	cout << "lr1ActionTab" << endl;
	for(size_t i = 0; i < lr1ActionTab.size(); ++i)
	{
		for(size_t j = 0; j < lr1ActionTab[i].size(); ++j)
		{
			cout << "lr1ActionTab[" << i << "," << ((j == G.VT.size())? '$': G.VT[j]) << "] = ";
			switch(lr1ActionTab[i][j].action)
			{
				case SHIFT: cout << "SHIFT " << lr1ActionTab[i][j].itemSetIdx << endl; break;
				case REDUCE: cout << "REDUCE "; G.P[lr1ActionTab[i][j].lProductionRuleIdx].display(); cout << endl; break;
				case ACCEPT: cout << "ACCEPT" << endl; break;
				default: cout << "ERR" << endl;
			}
		}
	}
	cout << endl;

	cout << "lr1GoToTab" << endl;
	for(size_t i = 0; i < lr1GoToTab.size(); ++i)
	{
		for(size_t j = 0; j < lr1GoToTab[i].size(); ++j)
		{
			cout << "lr1GoToTab[" << i << "," << G.VN[j] << "] = "
			     << ((lr1GoToTab[i][j] == lr1ItemSetFamily.size())? " ": to_string(lr1GoToTab[i][j])) << endl;
		}
	}
}

bool SyntaxAnalyzer::lr1SyntaxAnalyze(const string& myLR1Input)
{
	if(myLR1Input.size() == 0)
	{
		cout << "Input is empty!" << endl;
		return false;
	}

	cout << "Parsing " << myLR1Input << "..." << endl;

	lr1Stack.push_back(
		find_if(
			lr1ItemSetFamily.begin(), lr1ItemSetFamily.end(),
			[this](const set<LR1Item>& s)
			{
				return find_if(s.begin(), s.end(),
					[this](const LR1Item& i)
					{
						return G.P[i.lProductionRuleIdx].lPart == 'S';
					}) != s.end();
			}
			) - lr1ItemSetFamily.begin()
		);

	string str = myLR1Input;
	reverse(str.begin(), str.end());
	lr1Input = "$" + str;

	int idx = lr1Input.size() - 1;
	char a = lr1Input[idx];
	for(;;)
	{
		unsigned int s = lr1Stack[lr1Stack.size() - 1];
		auto ptr = find(G.VT.begin(), G.VT.end(), a);
		int aIdx = (ptr == G.VT.end())? G.VT.size(): ptr - G.VT.begin();
		LR1ActionTabItem curAction = lr1ActionTab[s][aIdx];
		cout << "Begin a new loop..." << endl;
		cout << "Current state(top of the stack): "
		     << lr1Stack[lr1Stack.size() - 1] << endl;
		display(lr1ItemSetFamily[lr1Stack[lr1Stack.size() - 1]]);
		cout << "Current input symbol: " << a << endl << endl;
		if(curAction.action == SHIFT)
		{
			lr1Stack.push_back(curAction.itemSetIdx);
			cout << "Push back state " << curAction.itemSetIdx << endl;
			display(lr1ItemSetFamily[curAction.itemSetIdx]);
			--idx;
			a = lr1Input[idx];
			cout << "Shift and now the input symbol is " << a << endl << endl;
		}
		else if(curAction.action == REDUCE)
		{
			cout << "Reduce using production rule "
			     << G.P[curAction.lProductionRuleIdx].lPart << "->"
			     << G.P[curAction.lProductionRuleIdx].rPartSet[curAction.rProductionRuleIdx] << endl;
			int num = (G.P[curAction.lProductionRuleIdx].rPartSet[curAction.rProductionRuleIdx] == "e")?
				0: G.P[curAction.lProductionRuleIdx].rPartSet[curAction.rProductionRuleIdx].size();
			for(int i = 0; i < num; ++i)
			{
				lr1Stack.erase(lr1Stack.end() - 1);
			}
			cout << "Pop " << num << " state(s) out of the stack" << endl;
			cout << "Now the stack top is state " << lr1Stack[lr1Stack.size() - 1] << endl;
			display(lr1ItemSetFamily[lr1Stack[lr1Stack.size() - 1]]);
			int t = lr1Stack[lr1Stack.size() - 1];
			lr1Stack.push_back(lr1GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()]);
			cout << "lr1GoToTab[" << t << "," << G.P[curAction.lProductionRuleIdx].lPart << "] = "
			     << ((lr1GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()] == lr1ItemSetFamily.size())?
			        	" ": to_string(lr1GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()]));
			cout << ", so we push back state "
			     << lr1GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()] << endl;
			display(lr1ItemSetFamily[lr1GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()]]);
			cout << endl;
		}
		else if(curAction.action == ACCEPT)
		{
			cout << "Parsing succeed!" << endl << endl;
			return true;
		}
		else
		{
			cout << "Parsing error!" << endl;
			cout << "In state " << lr1Stack[lr1Stack.size() - 1] << endl;
			display(lr1ItemSetFamily[lr1Stack[lr1Stack.size() - 1]]); cout << endl;
			return false;
		}
	}
}

void SyntaxAnalyzer::buildLL1ParseTab()
{
	ll1ParseTab.assign(G.VN.size(), vector<LL1Item>(G.VT.size() + 1, LL1Item(G.P.size(), 0)));

	for(size_t i = 0; i < G.P.size(); ++i)
	{
		unsigned int AIdx = find(G.VN.begin(), G.VN.end(), G.P[i].lPart) - G.VN.begin();

		for(size_t j = 0; j < G.P[i].rPartSet.size(); ++j)
		{
			set<char> firstAlpha = G.getFirst(G.P[i].rPartSet[j]);

			for(auto& p: firstAlpha)
			{
				unsigned int aIdx = find(G.VT.begin(), G.VT.end(), p) - G.VT.begin();
				if(aIdx < G.VT.size())
				{
					ll1ParseTab[AIdx][aIdx] = LL1Item(i, j);					
				}
			}

			if(find(firstAlpha.begin(), firstAlpha.end(), 'e') != firstAlpha.end())
			{
				set<char> followA = G.getFollow(G.P[i].lPart);
				
				for(auto& p: followA)
				{
					unsigned int bIdx = find(G.VT.begin(), G.VT.end(), p) - G.VT.begin();
					ll1ParseTab[AIdx][bIdx] = LL1Item(i, j);
				}

				if(find(followA.begin(), followA.end(), '$') != followA.end())
				{
					ll1ParseTab[AIdx][G.VT.size()] = LL1Item(i, j);
				}
			}
		}
	}

	for(size_t i = 0; i < ll1ParseTab.size(); ++i)
	{
		for(size_t j = 0; j < ll1ParseTab[i].size(); ++j)
		{
			cout << "ll1ParseTab[" << G.VN[i] << ", " << ((j == G.VT.size())? '$': G.VT[j]) << "] = ";
			if(ll1ParseTab[i][j].i < G.P.size())
			{
				cout << G.P[ll1ParseTab[i][j].i].lPart << "->" << G.P[ll1ParseTab[i][j].i].rPartSet[ll1ParseTab[i][j].j];
			}
			cout << endl;
		}
	}
}

bool SyntaxAnalyzer::ll1SyntaxAnalyze(const string& myLL1Input)
{
	if(myLL1Input.size() == 0)
	{
		cout << "Input is empty!" << endl;
		return false;
	}

	cout << "Parsing " << myLL1Input << "..." << endl;

	string str = myLL1Input;
	reverse(str.begin(), str.end());
	ll1Input = '$' + str;
	ll1Stack.push_back('$');
	ll1Stack.push_back(G.S);
	int ip = ll1Input.size() - 1;
	char x = ll1Stack[ll1Stack.size() - 1];

	while(x != '$')
	{
		if(x == ll1Input[ip])
		{
			ll1Stack.erase(ll1Stack.end() - 1);
			--ip;
		}
		else if(find(G.VT.begin(), G.VT.end(), x) != G.VT.end())
		{
			cout << "Parsing error!" << endl;
			return false;
		}
		else
		{
			unsigned int row = find(G.VN.begin(), G.VN.end(), x) - G.VN.begin();
			unsigned int col = (ll1Input[ip] == '$')? G.VT.size(): (find(G.VT.begin(), G.VT.end(), ll1Input[ip]) - G.VT.begin());

			if(ll1ParseTab[row][col].i >= G.P.size())
			{
				cout << "Parsing error!" << endl;
				return false;
			}
			else
			{
				cout << G.P[ll1ParseTab[row][col].i].lPart << "->" << G.P[ll1ParseTab[row][col].i].rPartSet[ll1ParseTab[row][col].j] << endl;
				ll1Stack.erase(ll1Stack.end() - 1);
				if(!(G.P[ll1ParseTab[row][col].i].rPartSet[ll1ParseTab[row][col].j].size() == 1 &&
					G.P[ll1ParseTab[row][col].i].rPartSet[ll1ParseTab[row][col].j][0] == 'e'))
				{				
					for(int cnt = G.P[ll1ParseTab[row][col].i].rPartSet[ll1ParseTab[row][col].j].size() - 1; cnt >= 0; --cnt)
					{
						ll1Stack.push_back(G.P[ll1ParseTab[row][col].i].rPartSet[ll1ParseTab[row][col].j][cnt]);
					}
				}
			}
		}
		x = ll1Stack[ll1Stack.size() - 1];
	}

	cout << "Parsing succeed!" << endl;
	return true;
}

set<LR0Item> SyntaxAnalyzer::LR0Closure(const set<LR0Item>& I)
{
	set<LR0Item> ret;
	for(auto ptr = I.begin(); ptr != I.end(); ++ptr)
	{
		ret.insert(*ptr);
	}

	bool updated = true;
	while(updated)
	{
		updated = false;

		for(auto ptr = ret.begin(); ptr != ret.end(); ++ptr)
		{
			if(ptr->dotPos < G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx].size()
				&& G.isNonTerminal(G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos]))
			{
				ProductionRule pr;
				int prIdx = G.getProductionRuleByLeft(pr, G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos]);
				int oldSize = ret.size();
				for(size_t i = 0; i < pr.rPartSet.size(); ++i)
				{
					ret.emplace(LR0Item(prIdx, i, 0));
				}
				int newSize = ret.size();
				if(oldSize != newSize)
				{
					updated = true;
				}
			}
		}
	}

	// cout << "LR0Closure:\n"; display(ret); cout << endl;

	return ret;
}

set<LR0Item> SyntaxAnalyzer::LR0GoTo(const set<LR0Item>& I, char X)
{
	if(find(G.VN.begin(), G.VT.begin(), X) != G.VN.end()
		|| find(G.VT.begin(), G.VT.end(), X) != G.VT.end())
	{
		set<LR0Item> ret;

		for(auto ptr = I.begin(); ptr != I.end(); ++ptr)
		{
			if(ptr->dotPos < G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx].size()
				&& G.P[ptr->lProductionRuleIdx].rPartSet[ptr->rProductionRuleIdx][ptr->dotPos] == X)
			{
				ret.insert(LR0Item(ptr->lProductionRuleIdx, ptr->rProductionRuleIdx, ptr->dotPos + 1));
			}
		}

		// cout << "LR0GoTo:\n"; display(LR0Closure(ret)); cout << endl;
		
		return LR0Closure(ret);
	}
	else
	{
		return set<LR0Item>();
	}
}

void SyntaxAnalyzer::calLR0ItemSetFamily()
{
	expandGrammar();

	lr0ItemSetFamily.push_back(
		LR0Closure(
			set<LR0Item>({
				LR0Item(
					find_if(
						G.P.begin(), G.P.end(),
						[](const ProductionRule& pr)
						{
							return pr.lPart == 'S';
						}) - G.P.begin(),
					0, 0
					)
				})
			)
		);

	bool updated = true;
	while(updated)
	{
		updated = false;

		for(auto& itemSetPtr: lr0ItemSetFamily)
		{
			for(auto& symbolPtr: G.VN)
			{
				set<LR0Item> itemSet = LR0GoTo(itemSetPtr, symbolPtr);
				if(!itemSet.empty()
					&& find_if(lr0ItemSetFamily.begin(), lr0ItemSetFamily.end(),
						[itemSet](const set<LR0Item>& s)
						{
							if(s.size() != itemSet.size())
							{
								return false;
							}
							else
							{
								auto ptr1 = itemSet.begin();
								auto ptr2 = s.begin();
								for(; ptr1 != itemSet.end(); ++ptr1, ++ptr2)
								{
									if(!(*ptr1 == *ptr2))
									{
										return false;
									}
								}
								return true;
							}
						}) == lr0ItemSetFamily.end())
				{
					lr0ItemSetFamily.push_back(itemSet);
					updated = true;
				}
			}

			for(auto& symbolPtr: G.VT)
			{
				set<LR0Item> itemSet = LR0GoTo(itemSetPtr, symbolPtr);
				if(!itemSet.empty()
					&& find_if(lr0ItemSetFamily.begin(), lr0ItemSetFamily.end(),
						[itemSet](const set<LR0Item>& s)
						{
							if(s.size() != itemSet.size())
							{
								return false;
							}
							else
							{
								auto ptr1 = itemSet.begin();
								auto ptr2 = s.begin();
								for(; ptr1 != itemSet.end(); ++ptr1, ++ptr2)
								{
									if(!(*ptr1 == *ptr2))
									{
										return false;
									}
								}
								return true;
							}
						}) == lr0ItemSetFamily.end())
				{
					lr0ItemSetFamily.push_back(itemSet);
					updated = true;
				}
			}
		}
	}

	// sort lr0ItemSetFamily
	sort(lr0ItemSetFamily.begin(), lr0ItemSetFamily.end(),
		[](const set<LR0Item>& itemSet1, const set<LR0Item>& itemSet2)
		{
			auto ptr1 = itemSet1.begin();
			auto ptr2 = itemSet2.begin();
			for(; ptr1 != itemSet1.end() && ptr2 != itemSet2.end(); ++ptr1, ++ptr2)
			{
				if(*ptr1 > *ptr2)
				{
					return false;
				}
				else if(*ptr1 < *ptr2)
				{
					return true;
				}
			}
			if(ptr1 != itemSet1.end() && ptr2 == itemSet2.end())
			{
				return false;
			}
			else
			{
				return true;
			}
		});

	/*cout << "item set family" << endl;
	for(size_t i = 0; i < lr0ItemSetFamily.size(); ++i)
	{
		cout << "I" << i << endl;
		display(lr0ItemSetFamily[i]);
		cout << endl;
	}

	cout << "Goto graph" << endl;
	for(size_t i = 0; i < lr0ItemSetFamily.size(); ++i)
	{
		for(size_t j = 0; j < G.VT.size(); ++j)
		{
			set<LR0Item> Ij = LR0GoTo(lr0ItemSetFamily[i], G.VT[j]);
			if(!Ij.empty())
			{
				cout << "I" << i << endl;
				display(lr0ItemSetFamily[i]);
				cout << "receives " << G.VT[j] << ", goes to" << endl;
				display(Ij);
				cout << endl;
			}
		}

		for(size_t j = 0; j < G.VN.size(); ++j)
		{
			set<LR0Item> Ij = LR0GoTo(lr0ItemSetFamily[i], G.VN[j]);
			if(!Ij.empty())
			{
				cout << "I" << i << endl;
				display(lr0ItemSetFamily[i]);
				cout << "receives " << G.VN[j] << ", goes to" << endl;
				display(Ij);
				cout << endl;
			}
		}
	}*/
}

void SyntaxAnalyzer::buildLR0ParseTab()
{
	lr0ActionTab.assign(lr0ItemSetFamily.size(), vector<LR0ActionTabItem>(G.VT.size() + 1, LR0ActionTabItem(ERR, 0, 0, 0)));
	lr0GoToTab.assign(lr0ItemSetFamily.size(), vector<unsigned int>(G.VN.size(), lr0ItemSetFamily.size()));
	// if an item in lr1GoToTab equals to the value of lr0ItemSetFamily.size(), it means this item is invalid

	for(size_t i = 0; i < lr0ItemSetFamily.size(); ++i)
	{
		for(auto itemSetPtr = lr0ItemSetFamily[i].begin(); itemSetPtr != lr0ItemSetFamily[i].end(); ++itemSetPtr)
		{
			if(itemSetPtr->dotPos < G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx].size())
			{
				if(G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx] == "e")
				{
					set<char> followA = G.getFollow(G.P[itemSetPtr->lProductionRuleIdx].lPart);
					for(auto& a: followA)
					{
						lr0ActionTab[i][find(G.VT.begin(), G.VT.end(), a) - G.VT.begin()] =
							LR0ActionTabItem(REDUCE, itemSetPtr->lProductionRuleIdx, itemSetPtr->rProductionRuleIdx, 0);
					}
				}

				auto ptr = find(G.VT.begin(), G.VT.end(),
					G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx][itemSetPtr->dotPos]);
				if(ptr != G.VT.end())
				{
					set<LR0Item> Ij = LR0GoTo(lr0ItemSetFamily[i],
						G.P[itemSetPtr->lProductionRuleIdx].rPartSet[itemSetPtr->rProductionRuleIdx][itemSetPtr->dotPos]);
					int lr0ActionTabCol = ptr - G.VT.begin();
					lr0ActionTab[i][lr0ActionTabCol] = LR0ActionTabItem(
						SHIFT, 0, 0,
						find_if(lr0ItemSetFamily.begin(), lr0ItemSetFamily.end(),
							[Ij](const set<LR0Item>& s)
							{
								if(s.size() != Ij.size())
								{
									return false;
								}
								else
								{
									auto ptr1 = Ij.begin();
									auto ptr2 = s.begin();
									for(; ptr1 != Ij.end(); ++ptr1, ++ptr2)
									{
										if(!(*ptr1 == *ptr2))
										{
											return false;
										}
									}
									return true;
								}
							}) - lr0ItemSetFamily.begin()
						);
				}
			}
			else
			{
				if(G.P[itemSetPtr->lProductionRuleIdx].lPart != 'S')
				{
					set<char> followA = G.getFollow(G.P[itemSetPtr->lProductionRuleIdx].lPart);
					for(auto& a: followA)
					{
						lr0ActionTab[i][find(G.VT.begin(), G.VT.end(), a) - G.VT.begin()] =
							LR0ActionTabItem(REDUCE, itemSetPtr->lProductionRuleIdx, itemSetPtr->rProductionRuleIdx, 0);
					}
				}
				else
				{
					lr0ActionTab[i][G.VT.size()] = LR0ActionTabItem(ACCEPT, 0, 0, 0);
				}
			}
		}
	}

	for(size_t i = 0; i < lr0ItemSetFamily.size(); ++i)
	{
		for(size_t A = 0; A < G.VN.size(); ++A)
		{
			set<LR0Item> Ij = LR0GoTo(lr0ItemSetFamily[i], G.VN[A]);
			lr0GoToTab[i][A] = find_if(lr0ItemSetFamily.begin(), lr0ItemSetFamily.end(),
				[Ij](const set<LR0Item>& s)
				{
					if(s.size() != Ij.size())
					{
						return false;
					}
					else
					{
						auto ptr1 = Ij.begin();
						auto ptr2 = s.begin();
						for(; ptr1 != Ij.end(); ++ptr1, ++ptr2)
						{
							if(!(*ptr1 == *ptr2))
							{
								return false;
							}
						}
						return true;
					}
				}) - lr0ItemSetFamily.begin();
		}
	}

	cout << "lr0ActionTab" << endl;
	for(size_t i = 0; i < lr0ActionTab.size(); ++i)
	{
		for(size_t j = 0; j < lr0ActionTab[i].size(); ++j)
		{
			cout << "lr0ActionTab[" << i << "," << ((j == G.VT.size())? '$': G.VT[j]) << "] = ";
			switch(lr0ActionTab[i][j].action)
			{
				case SHIFT: cout << "SHIFT " << lr0ActionTab[i][j].itemSetIdx << endl; break;
				case REDUCE: cout << "REDUCE "; G.P[lr0ActionTab[i][j].lProductionRuleIdx].display(); cout << endl; break;
				case ACCEPT: cout << "ACCEPT" << endl; break;
				default: cout << "ERR" << endl;
			}
		}
	}
	cout << endl;

	cout << "lr0GoToTab" << endl;
	for(size_t i = 0; i < lr0GoToTab.size(); ++i)
	{
		for(size_t j = 0; j < lr0GoToTab[i].size(); ++j)
		{
			cout << "lr0GoToTab[" << i << "," << G.VN[j] << "] = "
			     << ((lr0GoToTab[i][j] == lr0ItemSetFamily.size())? " ": to_string(lr0GoToTab[i][j])) << endl;
		}
	}
}

bool SyntaxAnalyzer::lr0SyntaxAnalyze(const string& myLR0Input)
{
	if(myLR0Input.size() == 0)
	{
		cout << "Input is empty!" << endl;
		return false;
	}

	cout << "Parsing " << myLR0Input << "..." << endl;

	lr0Stack.push_back(
		find_if(
			lr0ItemSetFamily.begin(), lr0ItemSetFamily.end(),
			[this](const set<LR0Item>& s)
			{
				return find_if(s.begin(), s.end(),
					[this](const LR0Item& i)
					{
						return G.P[i.lProductionRuleIdx].lPart == 'S';
					}) != s.end();
			}
			) - lr0ItemSetFamily.begin()
		);

	string str = myLR0Input;
	reverse(str.begin(), str.end());
	lr0Input = "$" + str;

	int idx = lr0Input.size() - 1;
	char a = lr0Input[idx];
	for(;;)
	{
		unsigned int s = lr0Stack[lr0Stack.size() - 1];
		auto ptr = find(G.VT.begin(), G.VT.end(), a);
		int aIdx = (ptr == G.VT.end())? G.VT.size(): ptr - G.VT.begin();
		LR0ActionTabItem curAction = lr0ActionTab[s][aIdx];
		cout << "Begin a new loop..." << endl;
		cout << "Current state(top of the stack): "
		     << lr0Stack[lr0Stack.size() - 1] << endl;
		display(lr0ItemSetFamily[lr0Stack[lr0Stack.size() - 1]]);
		cout << "Current input symbol: " << a << endl << endl;
		if(curAction.action == SHIFT)
		{
			lr0Stack.push_back(curAction.itemSetIdx);
			cout << "Push back state " << curAction.itemSetIdx << endl;
			display(lr0ItemSetFamily[curAction.itemSetIdx]);
			--idx;
			a = lr0Input[idx];
			cout << "Shift and now the input symbol is " << a << endl << endl;
		}
		else if(curAction.action == REDUCE)
		{
			cout << "Reduce using production rule "
			     << G.P[curAction.lProductionRuleIdx].lPart << "->"
			     << G.P[curAction.lProductionRuleIdx].rPartSet[curAction.rProductionRuleIdx] << endl;
			int num = (G.P[curAction.lProductionRuleIdx].rPartSet[curAction.rProductionRuleIdx] == "e")?
				0: G.P[curAction.lProductionRuleIdx].rPartSet[curAction.rProductionRuleIdx].size();
			for(int i = 0; i < num; ++i)
			{
				lr0Stack.erase(lr0Stack.end() - 1);
			}
			cout << "Pop " << num << " state(s) out of the stack" << endl;
			cout << "Now the stack top is state " << lr0Stack[lr0Stack.size() - 1] << endl;
			display(lr0ItemSetFamily[lr0Stack[lr0Stack.size() - 1]]);
			int t = lr0Stack[lr0Stack.size() - 1];
			lr0Stack.push_back(lr0GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()]);
			cout << "lr0GoToTab[" << t << "," << G.P[curAction.lProductionRuleIdx].lPart << "] = "
			     << ((lr0GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()] == lr0ItemSetFamily.size())?
			        	" ": to_string(lr0GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()]));
			cout << ", so we push back state "
			     << lr0GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()] << endl;
			display(lr0ItemSetFamily[lr0GoToTab[t][find(G.VN.begin(), G.VN.end(), G.P[curAction.lProductionRuleIdx].lPart) - G.VN.begin()]]);
			cout << endl;
		}
		else if(curAction.action == ACCEPT)
		{
			cout << "Parsing succeed!" << endl << endl;
			return true;
		}
		else
		{
			cout << "Parsing error!" << endl;
			cout << "In state " << lr0Stack[lr0Stack.size() - 1] << endl;
			display(lr0ItemSetFamily[lr0Stack[lr0Stack.size() - 1]]); cout << endl;
			return false;
		}
	}
}
