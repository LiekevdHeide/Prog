#include "printingResults.h"

#include <vector>
#include <fstream>
using namespace std;

void printTraffic(ofstream &write, size_t T, size_t vertices, vector<vector<vector<double>>> &arcFlowsAll) {
	//print arcFlows
	for (size_t t = 0; t < T; ++t) {
		for (size_t v = 0; v < vertices; ++v) {
			for (size_t w = 0; w < vertices; ++w) {
				write << arcFlowsAll[t][v][w] << ' ';
			}
			write << '\n';
		}
		write << '\n';
	}
	return;
}