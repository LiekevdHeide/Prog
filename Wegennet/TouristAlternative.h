#pragma once
#include "RoadNetwork.h"
#include <vector>
#include <cstddef> 

//std::vector<size_t> findTouristAlternativeOld(size_t N, double Mu, std::vector<std::vector<double>>& travelTimes, std::vector<std::vector<double>>& standardCapacities, std::vector<size_t> interrupted);
std::vector<size_t> findTouristAlternative1(size_t N, double Mu, std::vector<std::vector<double>>& travelTimes, std::vector<std::vector<double>>& standardCapacities, std::vector<size_t> interrupted);

//diff those 2?


std::vector<std::vector<std::vector<double>>> touristAlternative(RoadNetwork Net, double Mu, size_t M, std::vector<std::vector<size_t>>& locations, std::vector<std::vector<double>>& eqPathFlows, std::vector<std::vector<double>>& eqTravelTimeArcs);

void adjustTouristArcFLows(size_t T, size_t M, const vector<vector<size_t>>& binarySchedule, const vector<vector<vector<double>>>& touristAlternativeFlowsPerwholeState, vector<vector<vector<double>>>& arcFlowTourist);

//unused:  findAlternativeArcFlowsTourists in PSAP.h