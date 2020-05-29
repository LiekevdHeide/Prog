#include "ScheduleCheckFunctions.h"
#include "generalPrintingFunctions.h"
#include "TouristAlternative.h"
#include "RoadNetwork.h"

#include "generalPrintingFunctions.h"

#include <algorithm>
#include <vector>

using namespace std;

//main function for touristAlternatives: input network, output list for all maintenance combi's: which tourist flows (total tourist flows, not just the rerouted)
vector<vector<vector<double>>> touristAlternative(RoadNetwork Net, double Mu, size_t M, vector<vector<size_t>> &locations, vector<vector<double>> &eqPathFlows, vector<vector<double>> &eqTravelTimeArcs) {//assumes maintenance => zero capacity
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
	
	vector<vector<vector<double>>> resultingArcFlows(pow(2, M), vector<vector<double>>(Net.vertices, vector<double>(Net.vertices, 0.0)));
	vector<vector<vector<size_t>>> actualPaths;
	//for each binary / wholeschedule:
	cout << "schedules:\n";
	for (size_t s = 0; s < pow(2, M); ++s) {
		cout << s << '\n';

		actualPaths = Net.ODpaths;

		//use wholeScheduleToMaintenance to get the binary for it
		vector<size_t> schedule(M, 0);
		vector<size_t> scheduledMaintenance;
		wholeScheduleToMaintenance(2, M, s, schedule);
		//find which maintenance (in number) is scheduled
		cout << "scheduled maintenance:";
		for (size_t m = 0; m < M; ++m) {
			if (schedule[m] > 0) {
				scheduledMaintenance.push_back(m);
				cout << m;
			}
		}
		cout << '\n';
		//update capacities for this schedule!
		vector<vector<double>> actualCapacities = Net.standardCapacities;
		for (size_t m = 0; m < scheduledMaintenance.size(); ++m) {
			for (size_t l = 0; l < locations[scheduledMaintenance[m]].size(); l+=2) {
				actualCapacities[locations[scheduledMaintenance[m]][l]][locations[scheduledMaintenance[m]][l + 1]] = 0.0;
			}
		}
		
		//use interruptedRoutes to find which routes are interrupted for the scheduled maintenance
		for (size_t od = 0; od < Net.numberODpairs; ++od) {
			vector<size_t> whichInterrupted;

			for (size_t m = 0; m < scheduledMaintenance.size(); ++m) {
				for (size_t p = 0; p < Net.numberODpaths[od]; ++p) {
					if (interruptedRoutes[scheduledMaintenance[m]][od][p] == 1) {
						//if not yet added, add this route
						if (find(whichInterrupted.begin(), whichInterrupted.end(), p) == whichInterrupted.end()) {
							whichInterrupted.push_back(p);
						}
					}
				}
			}
			if (whichInterrupted.size() < Net.numberODpaths[od]) {//if all routes are blocked: this schedule is not feasible!
				//cout << "sizes:" << whichInterrupted.size() << '-' << Net.numberODpaths[od] << ' ';
				for (size_t p = 0; p < whichInterrupted.size(); ++p) {
					cout << "od" << od << "route" << whichInterrupted[p] << ' ';
					if (eqPathFlows[od][whichInterrupted[p]] > 0.0) {
						actualPaths[od][whichInterrupted[p]] = findTouristAlternative1(Net.vertices, Mu, eqTravelTimeArcs, actualCapacities, Net.ODpaths[od][whichInterrupted[p]]);//should be actualtimes!
					}
				}
			}
			else {
				cout << "Infeasible s" << s << '\n';
				break;
			}
		}
		
		//use actualPaths (i.e. standard paths, but replaced with newalternative paths for 1) used in eq 2) blocked paths)
		for (size_t od = 0; od < Net.numberODpairs; ++od) {
			for (size_t p = 0; p < Net.numberODpaths[od]; ++p) {
				//if the path is used in equilibrium
				if (eqPathFlows[od][p] > 0) {
					//add the eqPathFlow to the arcs in resultingArcFlow
					for (size_t j = 0; j < actualPaths[od][p].size() - 1; ++j) {
						size_t vertice1 = actualPaths[od][p][j];
						size_t vertice2 = actualPaths[od][p][j + 1];
						resultingArcFlows[s][vertice1][vertice2] += (Net.touristPercentage *  eqPathFlows[od][p]);
					}
				}
			}
		}
	}

	return resultingArcFlows;
}