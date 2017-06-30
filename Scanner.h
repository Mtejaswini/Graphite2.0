#ifndef __SCANNER_H
#define __SCANNER_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>

#include "TraversalConfiguration.h"

class Scanner
{
public:
	Scanner();
	~Scanner();

	//scan the file
	//Divide Vs and Vt into n logical partitions.
	void inputScan(int nSize, const char* szSkitterFile);
	//LS-traversal
	void LSScan(TraversalConfig config, vector<int>* resultVertexSet);

private:

};

#endif
