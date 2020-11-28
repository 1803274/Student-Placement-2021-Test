#include "main.h"

struct Point {
	int x = 0;
	int y = 0;
};

Point StartPoint;
Point EndPoint;
Node* nodes = nullptr;
Node* node_start = nullptr;
Node* node_end = nullptr;

string OutputString = "";

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void PrintIntro()
{
	cout << "Playground Games: Student Placement Test 2021\nName: Daniel Martinez Miguel\nEmail: danielmartinezdev@gmail.com";

}


void InitArray(vector<char>& InputVector, GridSize& gridSize)
{
	
	nodes = new Node[gridSize.x * gridSize.y];
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++) {
			int assemble = y * gridSize.x + x;
			//cout << assemble << endl;
			nodes[assemble].x = x;
			nodes[assemble].y = y;
			nodes[assemble].bObstacle = false;
			nodes[assemble].parent = nullptr;
			nodes[assemble].bVisited = false;
			nodes[assemble].bPath = false;
		}
	}
}



void ConectNodes(GridSize& gridSize)
{
	for (int x = 0; x < gridSize.x; x++) {
		for (int y = 0; y < gridSize.y; y++) {
			int assembler = y * gridSize.x + x;
			if (y > 0) {
				nodes[assembler].conections_neighbours.insert(&nodes[(y - 1) * gridSize.x + (x + 0)]);
				//cout << "Node: " << nodes[assembler].x << "," << nodes[assembler].y << 
				//	" is neighbour of node " << nodes[(y - 1) * gridSize.x + (x + 0)].x << "," << nodes[(y - 1) * gridSize.x + (x + 0)].y << endl;
			}
			if (y < gridSize.y - 1) {
				nodes[assembler].conections_neighbours.insert(&nodes[(y + 1) * gridSize.x + (x + 0)]);
				//cout << "Node: " << nodes[assembler].x << "," << nodes[assembler].y <<
				//	" is neighbour of node " << nodes[(y + 1) * gridSize.x + (x + 0)].x << "," << nodes[(y + 1) * gridSize.x + (x + 0)].y << endl;
			}
			if (x > 0) {
				nodes[assembler].conections_neighbours.insert(&nodes[(y + 0) * gridSize.x + (x - 1)]);
				//cout << "Node: " << nodes[assembler].x << "," << nodes[assembler].y <<
				//	" is neighbour of node " << nodes[(y + 0) * gridSize.x + (x - 1)].x << "," << nodes[(y + 0) * gridSize.x + (x - 1)].y << endl;
			}
			if (x < gridSize.x - 1) {
				nodes[assembler].conections_neighbours.insert(&nodes[(y + 0) * gridSize.x + (x + 1)]);
				//cout << "Node: " << nodes[assembler].x << "," << nodes[assembler].y <<
				//	" is neighbour of node " << nodes[(y + 0) * gridSize.x + (x + 1)].x << "," << nodes[(y + 0) * gridSize.x + (x + 1)].y << endl;
			}
}
	}
}

bool ReadInputFile(vector<char>& InputVector, GridSize& gridSize) {
	int x = 0;
	int y = 0;
	bool bEndNode = false;
	bool bStartNode = false;

	for (auto c : InputVector) {
		switch (c)
		{
		case 'x':
			nodes[y * gridSize.x + x].bObstacle = true;
			x++;
			break;
		case 'X':
			
			nodes[y * gridSize.x + x].bObstacle = true;
			x++;
			break;
		case 'A':
			node_start = &nodes[y * gridSize.x + x];
			StartPoint.x = x;
			StartPoint.y = y;
			x++;
			break;
		case 'a':
			node_start = &nodes[y * gridSize.x + x];
			StartPoint.x = x;
			StartPoint.y = y;
			x++;
			break;
		case 'B':
			node_end = &nodes[y * gridSize.x + x];
			EndPoint.x = x;
			EndPoint.y = y;
			x++;
			break;
		case 'b':
			node_end = &nodes[y * gridSize.x + x];
			EndPoint.x = x;
			EndPoint.y = y;
			x++;
			break;
		
		case '\n':
			y++;
			x = 0;
			break;
		default:
			x++;
			break;
		}
	}
	
	if (!node_start || !node_end) {
		cout << "Missing Start Point or End Point\n";
		return false;
	}
	else {
		return true;
	}
}

void ResetNodes(GridSize& gridSize) {
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			int assemble = y * gridSize.x + x;
			nodes[assemble].bVisited = false;
			nodes[assemble].DistanceToEndGlobal = INFINITY;
			nodes[assemble].DistanceToEndLocal = INFINITY;
			nodes[assemble].parent = nullptr;
		}

	}
}

void A_Star_Algorithm(GridSize& gridSize) {
	
	//Reset Node States
	ResetNodes(gridSize);

	//Heuristics
	auto distance = [](Node* a, Node* b) {
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};
	
	auto heuristic = [distance](Node* a, Node* b) {
		return distance(a, b);
	};

	//Starting Conditions
	Node* currentNode = node_start;
	node_start->DistanceToEndLocal = 0.f;
	node_start->DistanceToEndGlobal = heuristic(node_start, node_end);

	//Create untested node list and add the start node
	priority_queue<Node*, vector<Node*>, compare> untested_nodes_list;
	untested_nodes_list.push(node_start);

	while (!untested_nodes_list.empty() && currentNode != node_end) {
		while (!untested_nodes_list.empty() && untested_nodes_list.top()->bVisited)
		{
			untested_nodes_list.pop();
		}
		if (untested_nodes_list.empty()) {
			break;
		}
		//Explore Node Once
		currentNode = untested_nodes_list.top();
		currentNode->bVisited = true;
		//Check all the neighbours of the current node
		for (auto node_neighbour : currentNode->conections_neighbours) {
			//Add Neighbours if they are not obstacles and they were not visited. 
			if (!node_neighbour->bVisited && !node_neighbour->bObstacle) {
				untested_nodes_list.push(node_neighbour);
			}
			//Calculate possible better routes
			float relax = currentNode->DistanceToEndLocal + distance(currentNode, node_neighbour);
			//If the new path is better than the actual we get the new one.
			if (relax < node_neighbour->DistanceToEndLocal) {
				node_neighbour->parent = currentNode;
				node_neighbour->DistanceToEndLocal = relax;
				node_neighbour->DistanceToEndGlobal = node_neighbour->DistanceToEndLocal + heuristic(node_neighbour, node_end);
			}
		}
	}

	if (node_end) {
		Node* p = node_end;
		
		while (p) {
			p->bPath = true;
			if (p->parent) {
				if (p->parent->x < p->x) {
					OutputString.insert(0,"E");
				}
				else if (p->parent->x > p->x) {
					OutputString.insert(0, "W");
				}
				else if (p->parent->y < p->y) {
					OutputString.insert(0, "S");
				}
				else if (p->parent->y > p->y) {
					OutputString.insert(0, "N");
				}
			}
			p = p->parent;
		}
		if (OutputString == "") {
			OutputString = "There is no escape from this maze. :(\n";
		}
	}
}

void DrawGrid(GridSize& gridSize) {
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++)
		{
			int assemble = y * gridSize.x + x;
			if (nodes[assemble].bObstacle) {

				cout << "X ";

			}
			else if (x == StartPoint.x && y == StartPoint.y) {
				SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << "A ";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
			}
			else if (x == EndPoint.x && y == EndPoint.y) {
				SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << "B ";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
			}
			else if (nodes[assemble].bPath) {
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "- ";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
			}
			else {
				cout << ". ";
			}
		}
		cout << endl;
	}
}

void RestartProgram() {
	StartPoint.x = NULL;
	StartPoint.y = NULL;
	EndPoint.x = NULL;
	EndPoint.y = NULL;
	nodes = nullptr;
	node_start = nullptr;
	node_end = nullptr;
	
}

void Intro() {
	cout << "Playground Games: Student Placement Test 2021\nName: Daniel Martinez Miguel\nEmail: danielmartinezdev@gmail.com" << endl << endl;

}

int main() {

	//Intro
	Intro();

	string sOption = "default value";
	string InputInfo;
	GridSize gridSize;
	bool bValidInput = false;
	ifstream inFile;
	vector<char> charVector;
	bool bMenuValid = false;
	bool bExitProgram = false; 
	do
	{
	do
	{
		int menu = 0;
		do
		{
			cout << "Map selection menu: \n0- Introduce new filename.\n1- Select map <Quicktest Route 1>\n2- Select map <Quicktest Route 2>\n3- Select map <Quicktest Route 3>\n4- Select map <Quicktest Route 4>\n5- Exit. \nSelect one of the options: ";
			cin >> menu;
			bMenuValid = true;
			switch (menu)
			{
			case 0:
				cout << "Write the desired maze to load. <Example: quickest_route_1.txt>\n";
				cin >> sOption;
				break;
			case 1:
				sOption = "quickest_route_1.txt";
				break;
			case 2:
				sOption = "quickest_route_2.txt";
				break;
			case 3:
				sOption = "quickest_route_3.txt";
				break;
			case 4:
				sOption = "quickest_route_4.txt";
				break;
			case 5:
				return 0;
				break;
			default:
				bMenuValid = false;
				break;
			}
			if (bMenuValid) {
				inFile.open(sOption);
				if (inFile) {
					bValidInput = true;
					bool bCountColumnsComplete = false;

					cout << "File loaded successfully\n";
					char inputValues;
					while (!inFile.eof()) {
						inFile.get(inputValues);
						charVector.push_back(inputValues);

						if (!bCountColumnsComplete && inputValues != '\n') {
							gridSize.x++;
						}
						if (inputValues == '\n') {
							bCountColumnsComplete = true;
							gridSize.y++;
							
						}
					}
					inFile.close();
				}
				else {
					cout << "Error 404: Filename not found.\n";
					bMenuValid = false;
				}
			}
		} while (!bMenuValid);
	} while (!bValidInput);
	Intro();
	system("cls");
	cout << "Map Name: " << sOption << endl << endl;
	InitArray(charVector, gridSize);
	ConectNodes(gridSize);
	if (ReadInputFile(charVector, gridSize)) { //If the Input file have Start and End Point.
		A_Star_Algorithm(gridSize);
		DrawGrid(gridSize);
	}
	else {
		cout << "Start or End Point not detected\n";
	}



	cout << endl << "Maze Solution: " <<OutputString << endl << endl;
	RestartProgram();
	charVector.clear();
	gridSize.x = 0;
	gridSize.y = 1;
	OutputString = "";
	} while (!bExitProgram);

	return 0;
}
