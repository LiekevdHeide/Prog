#pragma once


//function if all demands still possible (all ODs connected)

// function from schedule to capacities
void binaryToCapacities(size_t T, size_t M, const std::vector< std::vector<size_t>>& binarySchedule, const vector<vector<size_t>>& locations, std::vector< std::vector< std::vector<double>>>& resultingCapacities);
//function from