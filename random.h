#pragma once
#include <random>
#include <sstream>
#include <iomanip>

using namespace std;

float randomGenerator(int a)
{
	float randomNumber = ((float)rand() / (float)RAND_MAX) *(float)a;

	stringstream ss;

	ss << fixed << setfill('0') << setprecision(2) << randomNumber; // set the precision of random number generated

	ss >> randomNumber; // pass it back to float

	return randomNumber;
}

int randomInt(int a)
{
	int randomNumber = (rand() % a) + 1;

	return randomNumber;
}