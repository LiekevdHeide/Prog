#include "printingResults.h"

#include <fstream>
#include <vector>
#include <cstddef> //necessery for size_t!!
using namespace std;

void printRecurringTraffic(ofstream& write, size_t T, size_t OD, vector<size_t>& numPaths, vector<vector<vector<double>>>& pathFlow) {//t,od,path,t
	write << "RecurrentPathTraffic_t,od,path\n";
	for (size_t t = 0; t < T; ++t) {
		for (size_t od = 0; od < OD; ++od) {
			for (size_t p = 0; p < numPaths[od]; ++p) {
				write << pathFlow[t][od][p] << ' ';
			}
			//write << '\n';
		}
		write << '\n';
	}
	
	return;
}