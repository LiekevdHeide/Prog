#include "generalPrintingFunctions.h"

#include <iostream>
#include <vector>

using namespace std;
void print2Dim(vector<vector<double>> toPrint, size_t dim1, size_t dim2) {

	for (size_t a = 0; a < dim1; ++a) {
		for (size_t b = 0; b < dim2; ++b) {
			cout << toPrint[a][b] << ' ';
		}
		cout << '\n';
	}

	return;
}