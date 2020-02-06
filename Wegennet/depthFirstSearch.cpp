#include "RoadNetwork.h"
#include "DepthFirstSearchFunctions.h"

#include <vector>

using namespace std;

void depthFirstSearch(RoadNetwork Network) {

	for (size_t od = 0; od < Network.numberODpairs; ++od) {

		
		findAllPaths(Network.ODpairs[2 * od], Network.ODpairs[2 * od + 1]);

	}


	return;
}