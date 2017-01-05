#pragma once
#include "Network.h"
#include <ctime>
#include <iostream>

using namespace std;

int main()
{
	cout << "WELCOME TO DSR SIMULATOR" << endl;

	int field, nodes;
	float range;

	// accept values
	cout << "ENTER PARAMETERS FOR DSR" << endl;
	cout << "Field: ";
	cin >> field;
	cout << "Nodes: ";
	cin >> nodes;
	cout << "Range: ";
	cin >> range;

	Network network(field, nodes,range); // initialize object of class using accepted values

	//get source and destination from user
	//int source, destination, broadcastId;

	//cout << "ENTER ROUTE PARAMETERS" << endl;
	//cout << "Source: ";
	//cin >> source;
	//cout << "Destination: ";
	//cin >> destination;
	//cout << "Broadcast Id: ";
	//cin >> broadcastId;

	//network.DSR(source, destination, broadcastId); // call DSR method

	return 0;
}