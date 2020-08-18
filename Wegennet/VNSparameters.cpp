#include "VNSparameters.h"
#include <fstream> //read input file
#include <cassert> //assertion file works
#include <iostream> //cout

VNSparameters::VNSparameters(string inputDoc) {

	ifstream readVNSinput(inputDoc);
	assert(readVNSinput && "Wrong VNS input doc");
	string trash;
	readVNSinput >> trash;//trash the first line

	readVNSinput >> maxItSameNBH;
	readVNSinput >> itsBeforeRestart;
	readVNSinput >> maxNoNewBest;
	cout << "VNS input read\n";
}
