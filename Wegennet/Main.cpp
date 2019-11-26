#include "performanceTimer.h"
#include "RoadNetwork.h"

#include <vector>
//#include <functional> //if want to make a standard function that can be different functions (such as policies etc)
//#include <random>
#include <iostream>
#include <fstream>

using namespace std;

int main() 
{
	string whichComputer{ "X:/My Documents/Wegennetwerk/Prog" };
	//string whichComputer{ "C:/Users/lieke/Documents/Wegennetwerk/Prog" };

	Timer time;

	//read the data file with road network    //should be inputs: road network configurations (set of vertices, arcs (directed), OD-pairs, constant for traffic time per arc)
	string roadInput = whichComputer + "/roadInput.txt";

	RoadNetwork Network(roadInput);

	for (size_t n = 0; n < Network.numberODpairs; ++n) {
		for (size_t r = 0; r < Network.numberODpaths[n]; ++r) {
			for (size_t o = 0; o < Network.ODpaths[n][r].size(); ++o) {
				cout << Network.ODpaths[n][r][o] << ' ';
			}
			cout << '\n';
		}
		cout << '\n';
	}
	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = whichComputer + "/maintenanceInput.txt";

	
	//write results to:
	ofstream write(whichComputer + "/Results.txt");  //, std::ios::app for adding to end of file

	//initialize class for both inputs + read inputs into them

	//class for all things with maintenance (all location sets, all durations, ..)
	//class for all things with road network (matrix link basic times, ?)
	//class? with schedule / capacities per link per time unit?

	cout << "Reading time: " << time.elapsed() << " seconds \n";
	time.reset();

	//calculate equilibrium with no maintenance scheduled.
	//look at what the Honours boys did..


	//Make initial schedule solution 

	//implement a heuristic GA/ALNS?

	cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
}