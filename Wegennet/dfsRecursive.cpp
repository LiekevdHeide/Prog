#include <vector>

using namespace std;

void dfsRecursive(size_t v, vector<bool> visited, size_t vertices, vector<vector<double>> capacities) {
	visited[v] = true;

	for (size_t i = 0; i < vertices; ++i) {
		if (capacities[v][i] > 0) {
			if (!visited[i]) {
				dfsRecursive(i, visited, vertices, capacities);
			}
		}
	}

	return;
}