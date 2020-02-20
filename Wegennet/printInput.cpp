#include "printingResults.h"

#include "RoadNetwork.h"
#include "MaintenanceActivities.h"

#include <vector>
#include <fstream>

using namespace std;

void printInput(ofstream &write, RoadNetwork &Network, MaintenanceActivities &Maintenance) {
	//print all standard input:

	write << Maintenance.T << ' ' << Maintenance.M << ' ' << Network.numberODpairs;


	return;
}

