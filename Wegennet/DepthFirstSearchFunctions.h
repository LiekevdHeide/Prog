#pragma once
#include "RoadNetwork.h"

void depthFirstSearch(RoadNetwork &Network);

void dfsRecursive(size_t v, size_t destination, size_t vertices, const vector<vector<double>>& capacities, vector<vector<size_t>>& finalPaths, size_t pathIndex, vector<bool> visited, vector<size_t> path);

void findAllPaths(size_t od, RoadNetwork &Network);