#pragma once
#include "Network.h"
#include "random.h"
#include <ctime>
#include <iostream>
#include <queue>
#include <cmath>

Network::Network(int field, int nodes, float range)
{
	this->field = field;
	this->noOfNodes = nodes;
	this->range = range;

	// resize all vectors to appropriate sizes
	this->nodes.resize(nodes + 1); // +1's because one indexing
	nodeCoordinates.resize(nodes + 1);
	graph.resize(nodes + 1);
	status.resize(nodes + 1);
	connectionExists.resize(nodes + 1, false);

	// resize the vector containing the status 
	for (int i = 1; i < status.size(); i++)
	{
		status[i].resize(nodes + 1, false);
	}

	//vector<vector<int>> grid(nodes + 1, vector<int>(nodes)); //grid F x F
	srand(time(NULL));

	// call the random coordinates deployment
	randomDeployment();

	// call method to create graph
	createGraph();

	// print nodes & coordinates
	printNodesCoor();

	// print graph structure
	printGraphStruct();

	// area for path search
	source = randomInt(nodes); // get random source

	destination = randomInt(nodes); // get random destination

	while (destination == source) // make sure source isn't same as destination
	{
		destination = randomInt(nodes);
	}

	broadcastId = 1;

	DSR(source, destination, broadcastId); // call DSR method with random values
}

Network::~Network()
{

}

// randomly generates  x and y coordinates for 
// each node
void Network::randomDeployment()
{
	float xCoor, yCoor;
	pairsFloat coordinates;

	// loop through all the nodes
	for (int i = 1; i <= noOfNodes; i++)
	{
		// generate random x and y coordinates
		xCoor = randomGenerator(field);
		yCoor = randomGenerator(field);

		// make a pair using the numbers generated
		coordinates = make_pair(xCoor, yCoor);

		// insert it into vector containing coordinates for each node
		nodeCoordinates[i] = coordinates;
	}
}

// function to calculate Euclidean distance
// accepts two nodes, uses them to get their coordinate pairs
// calculates the answer then returns it.
float Network::EucDist(int node1, int node2)
{
	int x1, x2, y1, y2, xCoor, yCoor, distance;
	x1 = nodeCoordinates[node1].first;
	y1 = nodeCoordinates[node1].second;

	x2 = nodeCoordinates[node2].first;
	y2 = nodeCoordinates[node2].second;

	xCoor = pow((x1 - x2), 2);
	yCoor = pow((y1 - y2), 2);

	distance = sqrt(xCoor + yCoor);

	return distance;
}

// function to make edges
// accepts the two nodes and creates an edge
void Network::edge(int node1, int node2)
{
	edgePair edge;

	edge = make_pair(node2, nodeCoordinates[node2]);
	graph[node1].push_back(edge);

	status[node1][node2] = true;
}

// creates the graph 
void Network::createGraph()
{
	float xCoor, yCoor;
	pairsFloat coordinates;

	for (int i = 1; i <= noOfNodes; ) // loop through all nodes
	{
		for (int j = 1; j <= noOfNodes; j++)
		{
			// if same as current node skip
			if (i == j)
			{
				continue;
			}

			else
			{
				if (EucDist(i, j) <= (2 * range)) // compare distance
				{
					if (!status[i][j]) // if connection doesnt exist already
					{
						edge(i, j);
						connectionExists[i] = true;
					}

					if (!status[j][i]) // if connection doesnt exist already
					{
						edge(j, i);
						connectionExists[j] = true;
					}
				}
			}
		}

		// ensures each node meets the at least one connection rule
		// regenerates new coordinates for the node until it is connection
		// range to another node
		if (connectionExists[i] == false)
		{
			// generate random x and y coordinates
			xCoor = randomGenerator(field);
			yCoor = randomGenerator(field);

			// make a pair using the numbers generated
			coordinates = make_pair(xCoor, yCoor);

			// insert it into vector containing coordinates for each node
			nodeCoordinates[i] = coordinates;
		}

		else
		{
			i++;
		}
	}
}

void Network::DSR(int source, int dest, int broadcastID)
{
	printSearchHead();

	this->nodeDistance.resize(noOfNodes + 1, -1); // initialize all distances to zero
	this->nodeParent.resize(noOfNodes + 1, 0); // initialize parents to zero
	bool found = false;

	queue<int> q;
	int currentNode, adjacentNode;

	q.push(source); // insert source as root
	nodeDistance[source] = 0; // set distance of root to zero

	// print RREQ header
	printRREQhead();

	//set message
	message = "RREQ";

	while (!q.empty())
	{
		vector<int> currentPath;

		currentNode = q.front();

		for (int i = 0; i < graph[currentNode].size(); i++) //loop through all adjacent nodes
		{
			adjacentNode = graph[currentNode][i].first;

			if (nodeDistance[adjacentNode] == -1)
			{
				nodeDistance[adjacentNode] = nodeDistance[currentNode] + 1; // set the node distance from root
				nodeParent[adjacentNode] = currentNode; // set the parent
				q.push(adjacentNode); // add node to queue
			}
		}

		if (currentNode != source) // block to print out current path
		{
			int present = currentNode;
			while (true)
			{
				currentPath.push_back(present);
				present = nodeParent[present];

				if (nodeDistance[present] == 0)
				{
					currentPath.push_back(present);
					break;
				}
			}

			printRREQ(message, currentPath, broadcastID); // print current request path
		}

		if (currentNode == dest) // if we get to destination break
		{
			found = true;
			break;
		}
		q.pop();
	}

	if (!found) // if there's no path from source to destination quit program
	{
		cout << "PATH NOT FOUND" << endl;
		exit(0);
	}

	// print hops for each node
	printHops();


	vector<int> path;
	// tracepack path to source
	while (true)
	{
		path.push_back(dest);
		dest = nodeParent[dest];

		if (nodeDistance[dest] == 0)
		{
			path.push_back(dest);
			break;
		}
	}
	message = "RREP";

	printRREP(message, path);

	message = "DATA";
	// print data path
	printData(message, path, broadcastID);

}

void Network::printNodesCoor()
{
	cout << "===================================================" << endl;
	cout << "NODES AND COORDINATES" << endl;
	cout << "===================================================" << endl << endl;

	// print out nodes and their coordinates
	for (int i = 1; i <= noOfNodes; i++)
	{
		cout << "Node " << i << " (" << nodeCoordinates[i].first << ", " << nodeCoordinates[i].second << ")" << endl;
	}
}

void Network::printGraphStruct()
{
	cout << endl << "===================================================" << endl;
	cout << "GRAPH STRUCTURE(ADJACENCY LIST)" << endl;
	cout << "===================================================" << endl << endl;

	// print out nodes and their connections
	for (int i = 1; i <= noOfNodes; i++)
	{
		cout << i;
		for (int j = 0; j < graph[i].size(); j++)
		{
			cout << " -> " << graph[i][j].first;
		}

		cout << endl;
	}

}

void Network::printHops()
{
	cout << endl << "===================================================" << endl;
	cout << "Hop Counts" << endl;
	cout << "===================================================" << endl << endl;

	//print out node distances and parents
	for (int i = 1; i <= noOfNodes; i++)
	{
		cout << "Node " << i << " Parent-> " << nodeParent[i] << " Hops -> " << nodeDistance[i] << endl;
	}
}


void Network::printRREQhead()
{
	cout << endl << "===================================================" << endl;
	cout << "RREQ TRACES" << endl;
	cout << "===================================================" << endl << endl;
}

void Network::printRREQ(string mess, vector<int> pathPassed, int id)
{
	cout << "Message: " << message << " || Current Path: ";

	for (int i = pathPassed.size() - 1; i >= 0; i--)
	{
		cout << pathPassed[i];

		if (i >= 1)
		{
			cout << " ,";
		}
	}

	cout << " || Broadcast ID: " << " " << id;

	cout << endl;
}

void Network::printRREP(string mess, vector<int> pathPassed)
{
	cout << endl << "===================================================" << endl;
	cout << "RREP TRACE BACK NODES" << endl;
	cout << "===================================================" << endl << endl;

	for (int i = 0; i < pathPassed.size(); i++)
	{
		cout << "MESSAGE:" << message << " || Node:" << pathPassed[i] << endl;

		if (i < pathPassed.size() - 1)
		{
			cout << "                 |                    " << endl;
			cout << "                 V                    " << endl;
		}
	}
}

void Network::printData(string mess, vector<int> pathPassed, int Bid)
{
	cout << endl << "===================================================" << endl;
	cout << "DATA FROM SOURCE TO DESTINATION" << endl;
	cout << "===================================================" << endl << endl;

	for (int i = pathPassed.size() - 1; i >= 0; i--)
	{
		cout << "MESSAGE:" << message << " || Node:" << pathPassed[i] << " || Broadcast ID: " << Bid << endl;

		if (i > 0)
		{
			cout << "                 |                    " << endl;
			cout << "                 V                    " << endl;
		}
	}
}

void Network::printSearchHead()
{
	cout << endl << "===================================================" << endl;
	cout << "SOURCE (" << source << ") ---> DESTINATION (" << destination << ")" << endl;
	cout << "===================================================" << endl << endl;
}