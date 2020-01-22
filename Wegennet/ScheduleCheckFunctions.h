#pragma once
#include <vector>

//function if all demands still possible (all ODs connected)

// function from schedule to capacities
void binaryToCapacities(size_t T, size_t M, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, std::vector< std::vector< std::vector<double>>>& resultingCapacities);

//function that updates available routes:
void adjustAvailableRoutes(size_t T, size_t M, size_t ODpairs, std::vector<size_t>& numberODpaths, std::vector<std::vector<std::vector<size_t>>>& ODpaths, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, std::vector<std::vector<std::vector<size_t>>> &availableRoutes, std::vector<std::vector<size_t>> &numAvailableRoutes);