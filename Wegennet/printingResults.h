#pragma once
#include <fstream>
#include <vector>

void printTraffic(std::ofstream& write, size_t T, size_t vertices, std::vector<std::vector<std::vector<double>>>& arcFlowsAll);//print arc flows for: t, v, w

void printSchedule(std::ofstream& write, size_t T, size_t M, std::vector<std::vector<size_t>>& binarySchedule);//print as maintenance time