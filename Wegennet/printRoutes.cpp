#include "printingResults.h"

#include <vector>
#include <fstream>
using namespace std;

void printRoutes(ofstream& write, size_t OD, vector<size_t> &numRoutes, vector<vector<vector<size_t>>> &Routes) {
	write << "Routes for this network:\n";

	for (size_t od = 0; od < OD; ++od) {
		for (size_t p = 0; p < numRoutes[od]; ++p) {
			for (size_t i = 0; i < Routes[od][p].size(); ++i) {
				write << Routes[od][p][i] << ' ';
			}
			write << '\n';
		}
	}
	return;
}