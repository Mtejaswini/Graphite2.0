#include "Block.h"
#include <iostream>

Block::Block()
{
}

Block::~Block()
{

}

//Block range Scans every block range of a vector and
//returns a block vector p that meets the condition {type}.
void Block::scanBlock(vector<int> tg, vector<string> type, vector<int>* p)
{
	int k = 0;

	for (vector<int>::const_iterator it = blocks.begin(); it != blocks.end(); ++it)
	{
		for (vector<int>::const_iterator ittg = tg.begin(); ittg != tg.end(); ++ittg)
		{
			if (*it == *ittg)
			{
				//The current code does not compare {type}.
				//You can add code here to compare.

				p->push_back(k);
			}
		}
		k++;
	}
}

//The target points corresponding to the detected points are searched at Vt.
void Block::materialize(vector<int> v, vector<int>* ret)
{
	for(vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		int r = blocks.at(*it);
		ret->push_back(r);
	}
}

void Block::setvalue(int val)
{
	blocks.push_back(val);
}

void Block::init()
{
	blocks.clear();
}
