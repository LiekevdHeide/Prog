#pragma once
#include <string>
#include <cstddef> 

using namespace std;
class VNSparameters
{
private:


public:
	//constructor
	VNSparameters(string inputDoc);

	size_t maxItSameNBH;
	size_t itsBeforeRestart;
	size_t maxNoNewBest;

};

