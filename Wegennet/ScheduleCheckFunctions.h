#pragma once
#include <vector>
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"


//function if all demands still possible (all ODs connected)

// function from schedule to capacities
void binaryToCapacities(size_t T, size_t M, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, std::vector<std::vector<double>> &standardCap, std::vector< std::vector< std::vector<double>>>& resultingCapacities);

//function that updates available routes:
bool adjustAvailableRoutes(size_t T, size_t M, size_t ODpairs, std::vector<size_t>& numberODpaths, std::vector<std::vector<std::vector<size_t>>>& ODpaths, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, std::vector<std::vector<std::vector<size_t>>>& maintenanceToRouteInterruption, std::vector<std::vector<std::vector<size_t>>>& availableRoutes, std::vector<std::vector<size_t>>& numAvailableRoutes);

void findInterruptedRoutes(size_t M, const std::vector<std::vector<size_t>>& locations, size_t ODpairs, const std::vector<size_t>& numberODpaths, const std::vector<std::vector<std::vector<size_t>>>& ODpaths, std::vector<std::vector<std::vector<size_t>>>& interruptedRoutes);//set the routes interrupted for each maintenance activity);//find which routes are interrupted by the maintenance action.


bool bruteForceSchedule(MaintenanceActivities& Maint, size_t schedule, size_t runOutTime, std::vector<std::vector<size_t>>& binarySchedule);
void wholeScheduleToMaintenance(size_t T, size_t M, size_t scheduleValue, vector<size_t>& startTimes);