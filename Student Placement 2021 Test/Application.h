/*
Playground Games: Engineering Student Placement Test 2012
Daniel Martinez Miguel.
Email: danielmartinezdev@gmail.com
LinkedIn: https://www.linkedin.com/in/daniel-martinez-miguel-172219140/
*/

#pragma once

//Libraries
#include <iostream>  
#include <thread>
#include <windows.h>
#include <queue>
#include <unordered_set>
#include <fstream>

#define MAX_OPTIONS 5  //Max options on the menu array.

using namespace std;
using namespace std::this_thread;

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

struct GridSize {
	int x = 0, y = 1;
};

struct Point {
	int x = 0;
	int y = 0;
};

class Application
{
public:
	//Application Constructor
	Application();
	//Application Destructor
	~Application();

	//RunApplication
	void runApp();

private:
	//Reset variable values
	void RestartApplication();
	//Print Title, my name and email
	void PrintIntro();
	//Initialize Nodes Array using the grid length
	void InitArray();
	//Connect each node with their array neighbours. 
	void ConnectNodes();
	//Transform the info transfered in Input file to Node information
	bool ReadInputFile();
	//Reset Node heuristic variables.
	void ResetNodes();
	//Pathfinding Algorithm based in heuristics and nodes
	void A_Star_Algorithm();
	//Output the grid to the console with colours to show the taken path. 
	void DrawGrid();
	//Print Options menu
	void PrintMenu();

protected:
	//Start and end Point of the maze.
	Point StartPoint, EndPoint;
	//Node pointers
	Node* nodes;
	Node* nodeStart;
	Node* nodeEnd;
	//Output string
	string sOutput = "";
	//Handle to change the comand window parameters
	HANDLE h;

private:
	GridSize gridSize;
	string sOption;
	string InputInfo;
	ifstream inFile;
	vector<char> charVector;
	bool bValidInput;
	bool bMenuValid;
	bool bExitProgram;


};

