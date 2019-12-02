#include "ConvexCombinations.h"
#include "RoadNetwork.h"

#include <ilcplex/ilocplex.h>
#include <vector>
#include <iostream>

using namespace std;

//ILOSTLBEGIN

double findOptimalStepsize(std::vector<std::vector<double>>& direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork &Road) {//vector new direction arc flows, old solution arc flows, road network
	double a = 0.4;

	//make cplex + environment
	IloEnv env;
	IloModel model(env);
	IloCplex cplex(model);

	//add variables
	IloNumVar alpha(env, 0, 1, ILOFLOAT);//IloNumVar::Float
	model.add(alpha);
	alpha.setName("alpha");

	try {
		//add objective
		IloExpr objective(env);
		for (size_t a = 0; a < Road.vertices; ++a) {
			for (size_t b = 0; b < Road.vertices; ++b) {
				objective += (direction[a][b] - oldFlow[a][b]) * Road.freeFlowTimes[a][b] * (1 + 0.15 * (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b])
					* (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b]) *(oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b])* (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b]));
			}
		}

		model.add(IloMinimize(env, objective));
		objective.end();

		//add constraints: none?
		IloExpr sum(env);
		for (size_t a = 0; a < Road.vertices; ++a) {
			for (size_t b = 0; b < Road.vertices; ++b) {
				sum += (direction[a][b] - oldFlow[a][b]) * Road.freeFlowTimes[a][b] * (1 + 0.15 * (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b])
					* (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b]) * (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b]) * (oldFlow[a][b] + alpha * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b]));

			}
		}
		model.add(sum >= 0);//error?
		sum.end();
	}
	catch (IloException& ex) {
		cout << "Error: " << ex << '\n';
	}

	//run
	cplex.setParam(IloCplex::TiLim, 100);
	cplex.setParam(IloCplex::EpGap, 0.01);
	cplex.exportModel("Test.lp");

	cplex.solve();
	cout << "Objective value:" << cplex.getObjective() << "   ";
	cout << "Optimal step:"<< cplex.getValue(alpha) << ' ';
	return cplex.getValue(alpha);
}