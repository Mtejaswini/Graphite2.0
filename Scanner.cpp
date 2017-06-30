#include <chrono>
#include <pthread.h>
#include <mutex>
#include <queue>
#include "Scanner.h"
#include "Block.h"
#include "thread_safe_queue.h"

using namespace std;

vector<int> vertices;
vector<Block> vs;
vector<Block> vt;

//pthread_mutex_t d_lock = -3;//PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

Scanner::Scanner()
{

}

Scanner::~Scanner()
{

}

//This function is called by the underlying processor for parallel
//processing by the thread.
void* thread_LSScan(void* arg, Block blockVs, Block blockVt)
{
	cout << "thread_LSScan" << endl;

	TraversalConfig config = *((TraversalConfig*)arg);
	vector<int> startVertices = config.startVertices;
	vector<string> types = config.types;
//	int direction = config.direction;

//	int blockid = config.blockid;
	vector<int>* result = new vector<int>();

	vector<int> Dhwai = startVertices;
	vector<int> P;

	//Block blockVs;
	//Block blockVt;

	P.clear();

	//Set the start block according to the direction.
	//1: forward 0: backward
//	if (direction == 1)
//	{
//		blockVs = vs.at(blockid);
//		blockVt = vt.at(blockid);
//	}
//	else //backward
//	{
//		blockVs = vt.at(blockid);
//		blockVt = vs.at(blockid);
//	}

	//Scan the points on the Si that meet the input conditions.
	blockVs.scanBlock(Dhwai, types, &P);

	Dhwai.clear();
	//The target points corresponding to the detected points are searched at Vt.
	blockVt.materialize(P, &Dhwai);

	//Put the points from the search into the results list.
	result->clear();
	for (vector<int>::iterator it = Dhwai.begin(); it != Dhwai.end(); it++)
	{
		result->push_back(*it);
	}

	pthread_exit(result);
}

//LS-Traversal
void Scanner::LSScan(TraversalConfig config, vector<int>* resultVertexSet)
{
	//pthread_t *threads;
	TraversalConfig* tc;
	int threadCount = 0;
	vector<int> result;
	bool bDup = false;

	threadCount = vs.size();
	//threads = new pthread_t[threadCount];
	tc = new TraversalConfig[threadCount];

	std::mutex cerrMutex;
	std::vector<std::thread> producers;
    ThreadSafeQueue<vector<int>> queue(5);

	cout << "LSScan start..."<< endl;

	if(config.startLevel == 0)
	{
		result = config.startVertices;
	}

	int p = 1;
	vector<int> Dhwai = config.startVertices;
	vector<int> P;

	//pthread_mutex_init(&d_lock, NULL);

	//Repeat the search as many times as the condition.
	while( p <= config.maxLevels)
	{
		if (Dhwai.size() == 0)
			break;

//		P.clear();

		//If the search is performed more than once,
		//the search is repeated with the previous result
		//list as input condition.
		config.startVertices = Dhwai;
		//Initialize the working vector.
		//Dhwai.clear();
		for (int tID = 0; tID < threadCount; tID++)
		{
			tc[tID] = config;
			tc[tID].blockid = tID;

			producers.push_back(std::thread([&, tID](){
				std::lock_guard<std::mutex> lock(cerrMutex);

				vector<int> result2;
				vector<int> Dhwai1 = config.startVertices;
				vector<int> P;

				P.clear();

				//Scan the points on the Si that meet the input conditions.
				vs.at(tID).scanBlock(Dhwai1, config.types, &P);

				Dhwai1.clear();
				//The target points corresponding to the detected points are searched at Vt.
				vt.at(tID).materialize(P, &Dhwai1);

				//Put the points from the search into the results list.
				//result->clear();
				result.clear();
				for (vector<int>::iterator it = Dhwai1.begin(); it != Dhwai1.end(); it++)
				{
					result2.push_back(*it);
				}

				queue.push(result2);

			}));
		}

		for (int tID = 0; tID < threadCount; tID++)
		{
			vector<int> mm;
			ThreadSafeQueue<vector<int>>::QueueResult result3;
			result3 = queue.pop(mm);
			{
				std::lock_guard<std::mutex> lock(cerrMutex);
			}

			for(vector<int>::iterator it = mm.begin(); it != mm.end(); it++)
			{
				int b = *it;
				//Duplicate inspection
				bDup = false;
				for (vector<int>::iterator itres = result.begin(); itres != result.end(); itres++)
				{
					if (b == *itres)
					{
						bDup = true;
						break;
					}
				}

				if (!bDup)
				{
					result.push_back(b);
					//Create a list of starting points for the next search.
					Dhwai.push_back(b);
				}
			}
		}

		p++;
	}

	std::lock_guard<std::mutex> lock(cerrMutex);

	for (auto& t : producers)
	{
		t.join();
	}

	//output the result.
	cout << "Result : " ;
	for(vector<int>::iterator it = result.begin(); it != result.end(); it++)
	{
		cout <<  *it << " " ;
	}
	cout << endl;

	if (tc != NULL)
	{
		delete[] tc;
		tc = NULL;
	}
}

//Divide Vs and Vt into n logical partitions.
//Vs = {S1, S2, S3, ... Sn}
//Vt = {T1, T2, T3, ... Tn}
void Scanner::inputScan(int nSize, const char* szSkitterFile)
{
	ifstream fin(szSkitterFile);
	string* line;
	clock_t start, end;

	if(!fin.is_open())
	{
		cout << "ERROR: failed to open file " << szSkitterFile << " for reading!" << endl;
		exit(1);
	}

	// mark the start time
	start = clock();
	cout << endl << "Starting scan..." << endl << endl;

	Block block_vs;
	Block block_vt;

	int kk = 0;

	// loop to read each line of vertex file
	while (fin.good())
	{
		// read next line
		line = new string();
		//getline(fin1, *line, '\r'); //show adjacency list
		getline(fin, *line);
		std::istringstream ss(*(string*)line);
		string mask;
		int fromid, toid;
		//check '#'
		getline(ss, mask, '\t');
		if (mask.substr(0, 1) == "#")
			continue;

		if (mask.length() == 0)
			continue;

		stringstream(mask) >> fromid;
		ss >> toid;

		//insert fromid into vertex vector
		bool bfound = false;
		for (vector<int>::iterator it = vertices.begin(); it != vertices.end(); it++)
		{
			if (*it == fromid)
			{
				bfound = true;
				break;
			}
			else if (*it > fromid)
			{
				bfound = true;
				vertices.insert(it, fromid);
				break;
			}
		}

		if (!bfound)
			vertices.push_back(fromid);

		int block_size = block_vs.getsize();
		if (block_size == 0)
		{
			block_vs.setvalue(fromid);
			block_vt.setvalue(toid);
		}
		else if (block_size < nSize)
		{
			block_vs.setvalue(fromid);
			block_vt.setvalue(toid);
		}
		else
		{
			kk++;
			cout << "InputScan size# " << kk << ":" << block_vs.getsize() << endl;
			//Divide into logical partitions of the same size.
			vs.push_back(block_vs);
			vt.push_back(block_vt);
			block_vs.init();
			block_vt.init();
			block_vs.setvalue(fromid);
			block_vt.setvalue(toid);
		}
	}

	cout << "InputScan size# " << kk << ":" << block_vs.getsize() << endl;
	vs.push_back(block_vs);
	vt.push_back(block_vt);

	// mark the end time
	end = clock();

	// display time taken
	cout << endl << "Complete." << endl;
	cout << "Time Taken: " << (end - start)/double(CLOCKS_PER_SEC)*1000 << " secs" << endl;
	cout << "count of vertices : " << vertices.size() << endl;

	fin.close();
}


