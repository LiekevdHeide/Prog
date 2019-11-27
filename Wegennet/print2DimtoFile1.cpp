#include "generalPrintingFunctions.h"

#include <vector>
#include <fstream>
void print2Dim(std::vector<std::vector<size_t>> toPrint, size_t dim1, std::ofstream& write) {
	for (size_t a = 0; a < dim1; ++a) {
		for (size_t b = 0; b < toPrint[a].size(); ++b) {
			write << toPrint[a][b] << ' ';
		}
		write << '\n';
	}

	return;
}