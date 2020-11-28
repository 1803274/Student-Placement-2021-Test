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

#define MAX_OPTIONS 5 

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
	char cValue = '~';
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

struct GridSize {
	int x = 0, y = 1; 
};

class main
{
private:


public:



};

