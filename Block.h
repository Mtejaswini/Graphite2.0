#ifndef __BLOCK_H
#define __BLOCK_H

#include <string>
#include <ostream>
#include <vector>

using namespace std;

//This class represent each block unit of a block range vector.(Bi)
class Block
{
public:
	Block();
	~Block();

	//Block range Scans every block range of a vector and
	//returns a block vector p that meets the condition.
	void scanBlock(vector<int> tg, vector<string> type, vector<int>* p );
	void materialize(vector<int> v, vector<int>* ret);

	//friend ostream& operator<<(ostream& out, const Block& b);

	void setvalue(int val);
	int getsize(){return blocks.size();};
	void init();

private:
	vector<int> blocks;
};

#endif
#pragma once
