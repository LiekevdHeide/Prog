#pragma once
#include "RoadNetwork.h"
#include <vector>

std::vector<size_t> findTouristAlternative(size_t N, std::vector<std::vector<double>>& travelTimes, std::vector<std::vector<double>>& standardCapacities, std::vector<size_t> interrupted);

std::vector<std::vector<std::vector<double>>> touristAlternative(RoadNetwork Net, size_t M, std::vector<std::vector<size_t>>& locations, std::vector<std::vector<double>>& eqPathFlows);


