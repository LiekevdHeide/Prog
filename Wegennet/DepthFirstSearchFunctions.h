#pragma once
#include "RoadNetwork.h"

void depthFirstSearch(RoadNetwork);

void dfsRecursive(size_t v, vector<bool> visited, size_t vertices, vector<vector<double>> capacities);

void findAllPaths(size_t o, size_t d);