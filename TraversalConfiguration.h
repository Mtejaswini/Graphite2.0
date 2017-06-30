#ifndef TRAVERSALCONFIGURATION_H_
#define TRAVERSALCONFIGURATION_H_

#include <vector>
#include <string>

using namespace std;

// struct to define the traversal configuration input to BFS traversal
struct TraversalConfig
{
	vector<int> startVertices;   // list of start vertices
	vector<string> types;        // predicates for edge type to filter traversal
	int startLevel;             // start level of traversal (ie collection boundary c)
	int maxLevels;              // maximum levels to traversal (r)
	int direction;
	int blockid;

	friend ostream& operator<<(ostream& out, TraversalConfig& config)
	{
		int i=0;
		vector<int> sourceVertices = config.startVertices;
		vector<string> types = config.types;
		out << "TraversalConfig = [{";
		for(vector<int>::const_iterator it = sourceVertices.begin(); it != sourceVertices.end(); ++it)
		{
			if(i == 0)
				out << *it;
			else
				out << "," << *it;
			i++;
		}
		out << "}, types={";
		for(vector<string>::const_iterator it2 = types.begin(); it2 != types.end(); ++it2)
		{
			out << *it2 << " ";
		}
		out << "}, c=" << config.startLevel << ", r=" << config.maxLevels << ", ";
		out << "d=" << config.direction << "}]";
		return out;
	}
};


#endif /* TRAVERSALCONFIGURATION_H_ */
