#include "printingResults.h"

#include "RoadNetwork.h"
#include "MaintenanceActivities.h"

#include <vector>
#include <fstream>
#include <cstddef> //necessery for size_t!!

using namespace std;

void printInput(ofstream &write, RoadNetwork &Network, MaintenanceActivities &Maintenance) {
	//print all standard input:
	write << "T M OD V\n";
	write << Maintenance.T << ' ' << Maintenance.M << ' ' << Network.numberODpairs << ' ' << Network.vertices << '\n';
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		write << Network.numberODpaths[od] << ' ';
	}
	write << '\n';


	return;
}

