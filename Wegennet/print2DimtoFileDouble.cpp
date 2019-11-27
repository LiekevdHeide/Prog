#include "generalPrintingFunctions.h"

#include <vector>
#include <fstream>
void print2Dim(std::vector<std::vector<double>> toPrint, size_t dim1, size_t dim2, std::ofstream& write) {
	for (size_t a = 0; a < dim1; ++a) {
		for (size_t b = 0; b < dim2; ++b) {
			write << toPrint[a][b] << ' ';
		}
		write << '\n';
	}

	return;
}