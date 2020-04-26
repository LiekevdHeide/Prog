#include "ScheduleCheckFunctions.h"
#include "generalPrintingFunctions.h"
#include "TouristAlternative.h"
#include "RoadNetwork.h"

#include <algorithm>
#include <vector>

using namespace std;

//main function for touristAlternatives: input network, output list for all maintenance combi's: which tourist flows.
void touristAlternative(RoadNetwork Net, size_t M, vector<vector<size_t>> &locations) {//assumes maintenance => zero capacity
	//Find alternative tourist flows for all combinations of maintenances

	//find which paths are interrupted
	vector<vector<vector<size_t>>> interruptedRoutes(M, vector<vector<size_t>>(Net.numberODpairs, vector<size_t>()));
	for(size_t m = 0; m < M; ++m)
		for(size_t od = 0; od < Net.numberODpairs; ++od)
			for (size_t r = 0; r < Net.numberODpaths[od]; ++r) {
				interruptedRoutes[m][od] = vector<size_t>(Net.numberODpaths[od], 0);
			}
	
	findInterruptedRoutes(M, locations, Net.numberODpairs, Net.numberODpaths, Net.ODpaths, interruptedRoutes);// input: locations is , returns interruptedRoutes (binary):m, od, r.

	//use binary schedule and binary interruptedRoutes to get interrupted routes for this schedule

	//for each binary / wholeschedule:
	cout << "schedules:\n";
	for (size_t s = 1; s < pow(2, M); ++s) {
		cout << s << '\n';

		//use wholeScheduleToMaintenance to get the binary for it
		vector<size_t> schedule(M, 0);
		vector<size_t> scheduledMaintenance;
		wholeScheduleToMaintenance(2, M, s, schedule);
		//find which maintenance (in number) is scheduled
		for (size_t m = 0; m < M; ++m) {
			if (schedule[m] > 0) {
				scheduledMaintenance.push_back(m);
			}
		}

		//update capacities for this schedule!
		vector<vector<double>> actualCapacities = Net.standardCapacities;
		for (size_t m = 0; m < scheduledMaintenance.size(); ++m) {
			for (size_t l = 0; l < locations[scheduledMaintenance[m]].size(); l+=2) {
				actualCapacities[locations[scheduledMaintenance[m]][l]][locations[scheduledMaintenance[m]][l + 1]] = 0.0;
			}
		}
		
		//use interruptedRoutes to find which routes are interrupted for the scheduled maintenance
		for (size_t od = 0; od < Net.numberODpairs; ++od) {
			cout << "od" << od << ' ';
			vector<size_t> whichInterrupted;

			for (size_t m = 0; m < scheduledMaintenance.size(); ++m) {
				if (s == 3) {
					cout << "m" << scheduledMaintenance[m] << ' ';
				}
				for (size_t p = 0; p < Net.numberODpaths[od]; ++p) {
					if (interruptedRoutes[m][od][p] == 1) {
						//if not yet added, add this route
						if (s == 3) {
							cout << "p" << p << ' ';
						}
						if (find(whichInterrupted.begin(), whichInterrupted.end(), p) == whichInterrupted.end()) {
							whichInterrupted.push_back(p);
							if (s == 3) {
								cout << "int" << p << " ";
							}
						}
					}
				}
			}
			if (whichInterrupted.size() < Net.numberODpaths[od]) {//if all routes are blocked: this schedule is not feasible!
				cout << "sizes:" << whichInterrupted.size() << '-' << Net.numberODpaths[od] << ' ';
				for (size_t p = 0; p < whichInterrupted.size(); ++p) {
					cout << "route" << whichInterrupted[p] << ' ';
					findTouristAlternative(Net.vertices, Net.freeFlowTimes, actualCapacities, Net.ODpaths[od][whichInterrupted[p]]);//should be actualtimes!
				}
			}
			else {
				cout << "Infeasible s \n";
				break;
			}
		}
		

	}

	return;
}