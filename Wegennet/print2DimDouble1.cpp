#include "generalPrintingFunctions.h"

#include <iostream>
#include <vector>
#include <cstddef> //necessery for size_t!!


using namespace std;
void print2Dim(vector<vector<double>> toPrint, size_t dim1) {

	for (size_t a = 0; a < dim1; ++a) {
		for (size_t b = 0; b < toPrint[a].size(); ++b) {
			cout << toPrint[a][b] << ' ';
		}
		cout << '\n';
	}

	return;
}