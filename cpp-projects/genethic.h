#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#define K 2

using namespace std;

struct vertex
{
    string name;
    vector<unsigned int> cost;
};

struct Population
{
    string pathName;
    vector<string> path;
    unsigned int cost;
};

struct comparePop
{
    inline bool operator()(const Population &ind1, const Population &ind2)
    {
        return (ind1.cost < ind2.cost);
    }
};

vector<vector<unsigned int>> mat(int size)
{
    srand(time(0));

    cout << "Size : " << size << endl;
    vector<vector<unsigned int>> matrix(size, vector<unsigned int>(size, 0));

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            unsigned int value = rand() % 1509 + 1;
            matrix[i][j] = value;
            matrix[j][i] = value;
            if (i == j)
            {
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}

vector<string> nameGenerator(unsigned int numCities)
{
    vector<string> names;
    for (int i = 0; i < numCities; i++)
    {
        for (char ch = 'A'; ch <= 'Z'; ch++)
        {
            string name = ch + to_string(i);
            names.push_back(name);
        }
    }
    return names;
}

vector<vertex> graphGenerator(vector<vector<unsigned int>> mat, vector<string>names)
{
    vector<vertex> graph;
    for (int i = 0; i < mat.size(); i++)
    {
        vertex curr;
        curr.name = names[i];
        curr.cost = mat[i];
        graph.push_back(curr);
    }

    return graph;
}

vector<vector<string>>popPathMatrix(int popSize, int numCity, vector<string>names)
{
	vector<vector<string>> pathPopulation;
    for (int i = 0; i < popSize; i++)
    {
        vector<string> cities;

   
        vector<int> indices(numCity);
        for (int j = 0; j < numCity; j++)
        {
            indices[j] = j;
        }
        random_shuffle(indices.begin(), indices.end());

       
        for (int j = 0; j < numCity; j++)
        {
            cities.push_back(names[indices[j]]);
        }
        cities.push_back(cities[0]);
        pathPopulation.push_back(cities);
    }
    return pathPopulation;
}

vector<int>costInd(int size, int numCity, vector<vector<string>> pathMatrix, vector<vertex>graph, vector<vector<unsigned int>> mat)
{
	vector<int>pathCosts;
	for (int i = 0; i < size; i++)
    {
        int totalCost = 0;

        for (int j = 0; j < numCity; j++)
        {
            string currentCity = pathMatrix[i][j];
            string nextCity = pathMatrix[i][j + 1];

            int currentIndex = -1;
            for (int k = 0; k < graph.size(); k++)
            {
                if (graph[k].name == currentCity)
                {
                    currentIndex = k;
                    break;
                }
            }

            int nextIndex = -1;
            for (int k = 0; k < graph.size(); k++)
            {
                if (graph[k].name == nextCity)
                {
                    nextIndex = k;
                    break;
                }
            }

            totalCost += mat[currentIndex][nextIndex];
        }

        pathCosts.push_back(totalCost);
    }
	return pathCosts;
}

vector<Population> populationArray(vector<vertex> graph, int numCity, int popSize, vector<vector<string>> pathPopulation, vector<int>pathCosts)
{
    vector<Population> populationArr;
	//Evaluate Population
    for (int i = 0; i < popSize; i++)
    {
        Population pop;
        pop.pathName = "Path " + to_string(i + 1);
        pop.path = pathPopulation[i];
        pop.cost = pathCosts[i];
        populationArr.push_back(pop);
    }

    return populationArr;
}

vector<Population>selectionOperator(vector<Population> population, int pop)
{
	vector<Population> matingPool;
	for(int i = 0; i < pop; i++)
	{
		unsigned int path1 = rand () % pop;
		unsigned int path2 = rand () % pop;
		matingPool.push_back((population[path1].cost < population[path2].cost) ? population[path1] : population[path2]);
	}
	return matingPool;
}


vector<vector<string>> crossOver(vector<Population> population, int pop, int numCity)
{
	int location = numCity - 3;
	vector<string> parent1;
	vector<string> parent2;
	vector<vector<string>> offPath;
	vector<string> shuffleArr;
	for(int i = 0; i < pop; i+=2)
	{
		if(i+1 < pop)
		{
		
		parent1 = population[i].path;
		parent2 = population[i+1].path; 
		for(int j = 0; j < 3; j++)
		{
			for(int s = 0; s < parent1.size(); s++)
			{
				if(parent1[location+j] == parent2[s] && parent1[location+j] != parent2[0])
				{
					shuffleArr.push_back(parent1[location + j]);
        			parent2.erase(parent2.begin() + s);
        			break;
				}
			}	
		}
		
		random_shuffle(shuffleArr.begin(), shuffleArr.end());
		parent2.insert(parent2.end() - 1, shuffleArr.begin(), shuffleArr.end());
		offPath.push_back(parent2);
		shuffleArr.clear();
		}
	}
	
	return offPath;
}
vector<Population> offSpring(vector<vector<string>> offPath, vector<vertex> graph, int numCity, int popSize, vector<vector<unsigned int>> mat, vector<int>pathCosts)
{
	vector<Population> pathOffspring;

    for (int i = 0; i < popSize/2; i++)
    {
        Population off;
        off.pathName = "Off " + to_string(i + 1);
        off.path = offPath[i];
        off.cost = pathCosts[i];
        pathOffspring.push_back(off);
    }
    return pathOffspring;
}

void elitistInd(vector<Population> pop, vector<Population> off)
{
	for(int i = 0; i < off.size(); i++){
		pop.push_back(off[i]);
	}
	
	sort(pop.begin(), pop.end(), comparePop());
	
	for(int i = 0; i < off.size(); i++)
	{
	
		pop.pop_back();
	}
	for (const auto path : pop)
	{
	    cout << "PathName: " << path.pathName << " | ";
	    for (const string city : path.path)
	    {
	        cout << city << " ";
	    }
	    cout << "| Cost: " << path.cost; 
	    cout << endl;
	}
}



