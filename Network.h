#pragma once
#include <vector>
#include <string>
using namespace std;

typedef pair<int, int> pairsInt;
typedef pair<float, float> pairsFloat;
typedef pair<int, pairsFloat> edgePair;

class Network
{
private:
	int field;
	int noOfNodes;
	int source;
	int destination;
	int broadcastId;
	float range;
	string message = "";
	vector<int> nodes;
	vector<pairsFloat> nodeCoordinates;
	vector<vector<pair<int,pairsFloat>>> graph; // adjacency list to hold nodes and it's connections
	vector<vector<bool>> status; // shows if there is a connection between two nodes or not
	vector<int> nodeDistance; // stores distance to source
	vector<int> nodeParent; // stores parent of each node
	vector<bool> connectionExists; // helps in checking that all nodes satisfy one connection rule

public:
	Network(int, int, float);
	~Network();
	void randomDeployment();
	float EucDist(int, int);
	void edge(int, int);
	void createGraph();
	void DSR(int, int,int); // source, destination, broadcastID
	void printNodesCoor();
	void printGraphStruct();
	void printHops();
	void printTrace();
	void printRREQhead();
	void printSearchHead();
	void printRREQ(string, vector<int>, int);
	void printRREP(string, vector<int>);
	void printData(string, vector<int>, int);
};