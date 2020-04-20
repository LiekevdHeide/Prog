#include "TouristAlternative.h"


#include <ilcplex/ilocplex.h>
#include <vector>
#include <string>

using namespace std;

void findTouristAlternative(size_t N, vector<vector<double>> &travelTimes, vector<vector<double>> &standardCapacities){

	IloEnv env;
	IloModel model(env);
	

	//create variables:
	IloArray<IloNumVarArray> arcs(env, N);
	for (size_t i = 0; i < N; ++i) {
		arcs[i] = IloNumVarArray(env, N, 0, 1, IloNumVar::Int);

		string name = "arc";
		name += to_string(i);
		arcs[i].setNames(name.c_str());
		model.add(arcs[i]);
	}

	
	//objective function:
	IloExpr objFunction(env);
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j)
			objFunction += travelTimes[i][j] * arcs[i][j]; // *travelTimes[i][j];
	}

	model.add(IloMinimize(env, objFunction));
	objFunction.end();


	//add constraints:
	//in=out
	for (size_t i = 1; i < N - 1; ++i) {
		IloExpr allIn(env);
		IloExpr allOut(env);
		for (size_t j = 0; j < N; ++j) {
			allIn += arcs[i][j];
			allOut += arcs[j][i];
		}
		model.add(allIn == allOut);
		allIn.end();
		allOut.end();
	}

	//leave o = 1, arrive d = 1
	IloExpr leaveOrigin(env);
	IloExpr arriveDestination(env);
	for (size_t j = 0; j < N; ++j) {
		leaveOrigin += arcs[0][j];
		arriveDestination += arcs[j][N - 1];
	}
	model.add(leaveOrigin == 1);
	model.add(arriveDestination == 1);
	leaveOrigin.end();
	arriveDestination.end();

	//only use available roads
	for(size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j) {
			model.add(arcs[i][j] <= standardCapacities[i][j]);
		}


	// make + solve the cplex
	IloCplex cplex(model);
	cplex.exportModel("ObjectiveAndConstraints.lp");
	cplex.solve();

	//get results from IP
	IloNum objVal = cplex.getObjValue();
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			cout << cplex.getValue(arcs[i][j]) << ' ';
		}
		cout << '\n';
	}


	model.end();
	env.end();
	return;
}