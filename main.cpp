#include "Scanner.h"
#include "TraversalConfiguration.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << endl << "Usage: " << argv[0] << " SKITTER-FILE" << endl;
		exit(1);
	}

	// create scanner instance
	Scanner scanner;
	// do LS traversal and save the result set of vertices
	vector<int> resultVertices;

	// scan the file
	// Read data from file to create vertex, Vs and Vt.
	// Define block size
	//Divide into logical partitions of the same size.
	int nBlockSize = 10000;
	scanner.inputScan(nBlockSize, argv[1]);

	// create a test traversal configuration
	TraversalConfig config;
	config.startVertices.push_back(3);
	config.types.push_back("belongs");
	config.startLevel = 1;
	config.maxLevels = 2;
	config.direction = 1; //1:forward 0:backward

	cout << "--Traversal configuration--" << endl;
	cout << config << endl;

	scanner.LSScan(config, &resultVertices);

	return 0;
}
