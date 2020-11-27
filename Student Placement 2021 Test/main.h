/*
Playground Games: Engineering Student Placement Test 2012
Daniel Martinez Miguel.
Email: danielmartinezdev@gmail.com
LinkedIn: https://www.linkedin.com/in/daniel-martinez-miguel-172219140/
*/

#pragma once


//Libraries
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <chrono>
#include <thread>
#include <windows.h>
#include <queue>
#include <unordered_set>
#include <fstream>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;
using chrono::duration_cast;
using chrono::milliseconds;
typedef chrono::steady_clock the_clock;

struct Node {
	int x, y; //2D Coords of the Node.
	bool bObstacle = false; 
	bool bVisited = false;
	bool bPath = false;
	float DistanceToEndGlobal;
	float DistanceToEndLocal;
	unordered_set<Node*> conections_neighbours;
	Node* parent;
};

struct compare {
	bool operator()(const Node* A, const Node* B) const {
		return A->DistanceToEndGlobal > B->DistanceToEndGlobal;
	}
};



class main
{
private:

	Node* nodes = nullptr;
	Node* node_start = nullptr;
	Node* node_end = nullptr;

public:
	//Draw Grid with Debugging purposes
	void DrawGrid(int start_x, int start_y, int end_x, int end_y, int grid_length);

	//2D Node Array Initialization
	void InitArray(int grid_length);

	//Create Node Conection
	void ConectNodes(int grid_length);

	//Add Random Obstacles
	void InsertObstacles(int start_x, int start_y, int end_x, int end_y, int randomizer, int grid_length);

	//Initialize grid, conections and start and end point. 
	void Initialization(int start_x, int start_y, int end_x, int end_y, int randomizer, int grid_length);

	//Reset node states. 
	void Reset_Nodes(int grid_length);

	//Pathfinding Algorithm
	void A_star(int start_x, int start_y, int end_x, int end_y, int grid_length);


};

