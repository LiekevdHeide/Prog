#include <vector>
#include <iostream>

using namespace std;

void dfsRecursive(size_t v, size_t destination, size_t vertices, const vector<vector<double>> &capacities, vector<vector<size_t>> &finalPaths, size_t pathIndex, vector<bool> visited, vector<size_t> path) {
	visited[v] = true;
	path[pathIndex] = v;
	pathIndex++;

	if (v == destination) {
		//end and save path
		for (size_t i = 0; i < pathIndex; ++i) {
			//cout << path[i] << ' ';
		}
		finalPaths.push_back(path);
		for (size_t i = pathIndex; i < path.size(); ++i) {
			finalPaths[finalPaths.size() - 1].pop_back();
		}
		//cout << '\n';
	}
	else {

		for (size_t i = 0; i < vertices; ++i) {
			if (capacities[v][i] > 0) {
				if (!visited[i]) {
					dfsRecursive(i, destination, vertices, capacities, finalPaths, pathIndex, visited, path);
				}
			}
		}
	}

	//remove from list and mark as not visited
	--pathIndex;
	visited[v] = false;
	

	return;
}