#include "generalPrintingFunctions.h"

#include <iostream>
#include <vector>

using namespace std;
void print2Dim(vector<vector<double>> toPrint) {

	for (size_t a = 0; a < toPrint.size(); ++a) {
		for (size_t b = 0; b < toPrint[a].size(); ++b) {
			cout << toPrint[a][b] << ' ';
		}
		cout << '\n';
	}

	return;
}