#pragma once
#include <fstream>
#include <vector>

#include "RoadNetwork.h"
#include "MaintenanceActivities.h"

void printTraffic(std::ofstream& write, size_t T, size_t vertices, std::vector<std::vector<std::vector<double>>>& arcFlowsAll);//print arc flows for: t, v, w

void printSchedule(std::ofstream& write, size_t T, size_t M, std::vector<std::vector<size_t>>& binarySchedule);//print as maintenance time
void printSchedule(std::ofstream& write, size_t T, size_t M, std::vector<size_t>& startTimes);//print as maintenance time


void printInput(std::ofstream& write, RoadNetwork& Network, MaintenanceActivities& Maintenance);//print all maintenance / road input
void printRoutes(std::ofstream& write, size_t OD, std::vector<size_t>& numRoutes, std::vector<std::vector<std::vector<size_t>>>& Routes);

void printRecurringTraffic(std::ofstream& write, size_t T, size_t OD, std::vector<size_t>& numPaths, std::vector<std::vector<std::vector<double>>>& pathFlow);//od,path,t

void printCostsPerTime(std::ofstream& write, RoadNetwork& Network, size_t T, vector<vector<vector<double>>>& scheduledCapacities, vector<vector<vector<double>>>& arcFlows);