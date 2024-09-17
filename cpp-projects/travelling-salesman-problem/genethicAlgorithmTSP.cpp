#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#define K 2
#include "genethic.h"
using namespace std;


int main()
{
    int populationSize = 10;
    int numCities = 6;
	int gen = 3;
	int startGen = 1;
    
    vector<vector<unsigned int>> costMatrix = mat(numCities);
    vector<string> cityNames = nameGenerator(numCities);
    vector<vertex> graph = graphGenerator(costMatrix, cityNames);
    vector<vector<string>>popString = popPathMatrix(populationSize, numCities, cityNames);
    vector<int>popCost = costInd(populationSize, numCities, popString, graph, costMatrix);
    vector<Population> generatedPopulation = populationArray(graph, numCities, populationSize, popString, popCost);
    for (const auto &v : graph)
    {
        cout << "Vertex " << v.name << ": ";
        for (const auto cost : v.cost)
        {
            cout << cost << " ";
        }
        cout << endl;
    }
    while(gen != 0)
    {
	cout << "GEN :" << startGen << endl;

	for (const auto individual : generatedPopulation)
	{
	    cout << "PathName: " << individual.pathName << " | ";
	    for (const string &city : individual.path)
	    {
	        cout << city << " ";
	    }
	    cout << "| Cost: " << individual.cost; 
	    cout << endl;
	}
	vector<Population> matingPool = selectionOperator(generatedPopulation, populationSize);
	cout << "MATING POOL BY SELECTION OPERATOR : " << endl;
	for (const auto pool : matingPool)
	{
	    cout << "PathName: " << pool.pathName << " | ";
	    for (const string path : pool.path)
	    {
	        cout << path << " ";
	    }
	    cout << " Cost " << pool.cost << " |  " << endl;
	}
    vector<vector<string>> offString = crossOver(matingPool, populationSize, numCities);
    vector<int>offCost = costInd(offString.size(), numCities, offString, graph, costMatrix);
    vector<Population> springOff = offSpring(offString, graph, numCities, populationSize, costMatrix, offCost);
    cout << "OFFSPRING DETAILS:" << endl;
    for (const auto individual : springOff)
    {
        cout << "PathName: " << individual.pathName << " | ";
        for (const string city : individual.path)
        {
            cout << city << " ";
        }
        cout << "| Cost: " << individual.cost;
        cout << endl;
    }
    cout << " ELITIST INDIVIDUALS OF" << " GEN : " << startGen << endl;
    elitistInd(generatedPopulation, springOff);
    gen--;
    startGen++;
}
    return 0;
}
