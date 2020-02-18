#include "printingResults.h"

#include <vector>
#include <fstream>

using namespace std;

void printSchedule(ofstream &write, size_t T, size_t M, vector<vector<size_t>> &binarySchedule) {//print schedule: maintenance, time

	for (size_t m = 0; m < M; ++m) {
		for (size_t t = 0; t < T; ++t) {
			if (binarySchedule[m][t] == 1) {
				write << m << ' ' << t << '\n';
			}
		}
	}

	return;
}