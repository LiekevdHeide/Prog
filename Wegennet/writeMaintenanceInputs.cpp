#include <fstream>
#include <cstddef> //necessery for size_t!!
#include <string>

#include "ScheduleCheckFunctions.h"

using namespace std;
void writeMaintenanceInputs() {

	//write maintenance input
	string whichComputer{ "/home/p279495/Wegennet/sourceFiles/" };

	size_t runoutTime = 30;
	size_t timeHorizon = 31 + runoutTime;
	size_t numVertices = 4;
	vector<vector<size_t>> maintenanceLocations{ {0,1}, {1, 2, 2, 1}, {0, 2}, {1,3}, {2,3} };

	//maintenanceInput
	for (size_t m = 3; m < 6; ++m) {
		for (size_t wholeDurtation = 1; wholeDurtation < pow(5, 3); ++wholeDurtation) { //not if none planned, or only 1 planned
			string nameFile = whichComputer + to_string(m) + '_' + to_string(wholeDurtation) + ".txt";
			ofstream write(nameFile);
			write << nameFile << '\n';

			write << m << ' ' << timeHorizon << ' ' << runoutTime << '\n';

			vector<size_t> durations(5, 0);
			wholeScheduleToMaintenance(2, m, wholeDurtation, durations);
			for (size_t d = 0; d < durations.size(); ++d) {
				durations[d] *= 5;
				if (durations[d] > 0) {
					write << durations[d] << ' ';
				}
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
	vector<string> mu{ "0.5", "1.0" };
	vector<size_t> demands{ 10, 20 };

	size_t cap = 5;
	size_t freeflow = 5;

	for (size_t touristPerc = 0; touristPerc < 2; ++touristPerc) {
		for(size_t numMu = 0; numMu < 2; ++numMu)
			for (size_t dem = 0; dem < 2; ++dem) {
				for (size_t freeF12 = 0; freeF12 < 3; ++freeF12) 
					for(size_t freeF23 = 0; freeF23 < 2; ++freeF23){
					string nameNetfile = whichComputer + touristPercentages[touristPerc] + '_' + mu[numMu] + '_' + to_string(demands[dem]) + ".txt";
					ofstream writeNet(nameNetfile);

					writeNet << nameNetfile << '\n';
					writeNet << "4 1 " << touristPercentages[touristPerc] << ' ' << mu[numMu] << '\n';

					writeNet << "0 3\n";

					writeNet << demands[dem] << '\n';


					for (size_t v = 0; v < numVertices; ++v) {
						for (size_t w = 0; w < numVertices; ++w) {
							//print capacities
							if (v != w && v != w + 3 && w != v + 3) {
								writeNet << cap << ' ';
							}
							else {
								writeNet << 0 << ' ';
							}
						}
						writeNet << '\n';
					}
					//print freeflow times:
					writeNet << 0 << ' ' << freeflow << ' ' << freeflow << ' ' << 0 << '\n';
					writeNet << freeflow << ' ' << 0 << ' ';
					if (freeF12 == 0) {
						writeNet << 1 << ' ';
					}
					if (freeF12 == 1) {
						writeNet << 3 << ' '; 
					}
					else {
						writeNet << 5 << ' '; 
					}
					writeNet << freeflow << '\n';

					writeNet << freeflow << ' ';
					if (freeF12 == 0) {
						writeNet << 1 << ' ';
					}
					if (freeF12 == 1) {
						writeNet << 3 << ' ';
					}
					else {
						writeNet << 5 << ' ';
					}
					writeNet << 0 << ' ' << freeflow << '\n';

					writeNet << 0 << ' ' << freeflow << ' ';
					
					if (freeF23 == 0) {
						writeNet << 1 << ' ';
					}
					else {
						writeNet << 3 << ' ';
					}
					
					writeNet << 0 << '\n';
					}
			}
	}

	//create batch file using it
	string mainLocation{ "/home/p279495/Wegennet/" };
	ofstream batchFile(mainLocation + "batch.txt");
	for (size_t m = 3; m < 6; ++m) 
		for (size_t wholeDurtation = 1; wholeDurtation < pow(5, 3); ++wholeDurtation) //not if none planned, or only 1 planned
			for (size_t touristPerc = 0; touristPerc < 2; ++touristPerc) 
				for(size_t numMu = 0; numMu < 2; ++numMu)
				for (size_t dem = 0; dem < 2; ++dem) {
					string nameFile = mainLocation + "script" + to_string(m) + '_' + to_string(wholeDurtation) + touristPercentages[touristPerc]+ '_' + mu[numMu] + '_' + to_string(demands[dem]) + ".txt";
					ofstream writeScript(nameFile);

					writeScript << "#!/bin/bash\n";
					writeScript << "#SBATCH --job-name=" << to_string(m) + '_' + to_string(wholeDurtation) + touristPercentages[touristPerc] + '_' + mu[numMu] + '_' + to_string(demands[dem]) << '\n';
					if (m < 5) {
						writeScript << "#SBATCH --time=00:20:00\n";
					}
					else {
						writeScript << "#SBATCH --time=5:00:00\n";
					}
					writeScript << "#SBATCH --mem=1GB\n";
					writeScript << '\n';

					writeScript << "module load CPLEX/12.9.0 \n";
					writeScript << '\n';
					writeScript << "srun ./mainExperiments13920.exe " << whichComputer + touristPercentages[touristPerc] + '_' + mu[numMu] + '_' + to_string(demands[dem]) + ".txt " << whichComputer + to_string(m) + '_' + to_string(wholeDurtation) + ".txt " << whichComputer + "VNSinputTrial.txt \n" ;

					//write all scriptnames in .txt
					batchFile << "sbatch " <<  nameFile << '\n';

					writeScript.close();
				}


	batchFile.close();

	return;
}