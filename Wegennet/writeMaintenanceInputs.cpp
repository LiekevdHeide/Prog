#include <fstream>
#include <cstddef> //necessery for size_t!!
#include <string>

#include "ScheduleCheckFunctions.h"

using namespace std;
void writeMaintenanceInputs() {

	//write maintenance input
	string whichComputer{ "C:/Users/Gebruiker/Documents/Wegennetwerk/Experiments/finding err in costs/" };

	size_t runoutTime = 30;
	size_t timeHorizon = 31 + runoutTime;
	size_t numVertices = 4;
	vector<vector<size_t>> maintenanceLocations{ {0,1}, {1,2, 2, 1}, {0, 2}, {1,3}, {2,3} };

	//maintenanceInput
	for (size_t m = 3; m < 6; ++m) {
		for (size_t wholeDurtation = 1; wholeDurtation < pow(5, 3); ++wholeDurtation) { //not if none planned, or only 1 planned
			string nameFile = whichComputer + to_string(m) + '_' + to_string(wholeDurtation);
			ofstream write(nameFile);
			write << nameFile << '\n';

			write << m << ' ' << timeHorizon << ' ' << runoutTime << '\n';

			vector<size_t> durations(5, 0);
			wholeScheduleToMaintenance(2, m, wholeDurtation, durations);
			for (size_t d = 0; d < durations.size(); ++d) {
				durations[d] *= 5;
				write << durations[d] << ' ';
			}
			write << '\n';

			for (size_t d = 0; d < durations.size(); ++d) {
				if (durations[d] > 0) {
					for (size_t l = 0; l < maintenanceLocations[d].size(); ++l) {
						write << maintenanceLocations[d][l] << ' ';
					}
					write << numVertices + 10 << '\n';
				}

			}
		
			write.close();
		}
	}

	//write network input
	vector<string> touristPercentages{ "0.4", "0.0" };
	string mu = "0.5";
	vector<size_t> demands{ 10, 20 };

	for (size_t touristPerc = 0; touristPerc = 2; ++touristPerc) {

		for (size_t dem = 0; dem < 2; ++dem) {
			string nameNetfile = whichComputer + touristPercentages[touristPerc] + '_' + to_string(demands[dem]);
			ofstream writeNet(nameNetfile);

			writeNet << nameNetfile << '\n';
			writeNet << "4 1 " << touristPercentages[touristPerc] << ' ' << mu << '\n';

			writeNet << "0 3\n";

			writeNet << demands[dem] << '\n';

			for(size_t v = 0; v < numVertices; ++v)
				for (size_t w = 0; w < numVertices; ++w) {
					//print capacities

				}
			for (size_t v = 0; v < numVertices; ++v)
				for (size_t w = 0; w < numVertices; ++w) {
					//print freeflowtimes

				}
		}
	}
	



	//write script using <<\n or \newline

	//write all scriptnames in .txt

	return;
}