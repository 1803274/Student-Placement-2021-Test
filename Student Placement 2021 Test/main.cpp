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

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void main::PrintIntro()
{
	cout << "Playground Games: Student Placement Test 2021\nName: Daniel Martinez Miguel\nEmail: danielmartinezdev@gmail.com";

}

void main::DrawGrid(int start_x, int start_y, int end_x, int end_y, int grid_length)
{
}

void InitArray(vector<char>& InputVector, GridSize& gridSize)
{
	
	nodes = new Node[gridSize.x * gridSize.y];
	for (int x = 0; x < gridSize.x; x++) {
		for (int y = 0; y < gridSize.y; y++) {
			int assemble = y * gridSize.x + x;
			cout << assemble << endl;
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
			cout << "Node: " << assembler << endl<< endl;
			if (y > 0) {
				cout << "Conected to: " << (y - 1) * gridSize.y + (x + 0) << endl;
				nodes[assembler].conections_neighbours.insert(&nodes[(y - 1) * gridSize.x + (x + 0)]);
			}
			if (y < gridSize.y - 1) {
				cout << "Conected to: " << (y + 1) * gridSize.x + (x + 0) << endl;

				nodes[assembler].conections_neighbours.insert(&nodes[(y + 1) * gridSize.x + (x + 0)]);
			}
			if (y > 0) {
				cout << "Conected to: " << (y + 0) * gridSize.x + (x - 1) << endl;

				nodes[assembler].conections_neighbours.insert(&nodes[(y + 0) * gridSize.x + (x - 1)]);
			}
			if (x < gridSize.x - 1) {
				cout << "Conected to: " << (y + 0) * gridSize.x + (x + 1) << endl;

				nodes[assembler].conections_neighbours.insert(&nodes[(y + 0) * gridSize.x + (x + 1)]);
			}
	//		if (x < gridSize.x - 1 && y < gridSize.y - 1) {
	//			nodes[assembler].conections_neighbours.insert(&nodes[(y + 1) * gridSize.x + (x + 1)]);
	//		}
	//		if (x > 0 && y > 0) {
	//			nodes[assembler].conections_neighbours.insert(&nodes[(y - 1) * gridSize.x + (x - 1)]);
	//		}
	//		if (x > 0 && y < gridSize.y - 1) {
	//			nodes[assembler].conections_neighbours.insert(&nodes[(y + 1) * gridSize.x + (x - 1)]);
	//		}
	//		if (x < gridSize.x - 1 && y > 0) {
	//			nodes[assembler].conections_neighbours.insert(&nodes[(y - 1) * gridSize.x + (x + 1)]);
	//		}
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
			p = p->parent;
		}
	}


}

void DrawGrid(GridSize& gridSize) {
	for (int x = 0; x < gridSize.x; x++) {
		for (int y = 0; y < gridSize.y; y++)
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

int main() {

	//Intro
	cout << "Playground Games: Student Placement Test 2021\nName: Daniel Martinez Miguel\nEmail: danielmartinezdev@gmail.com";

	string sOption = "quickest_route_4.txt";

	string InputInfo;
	GridSize gridSize;
	bool bValidInput = false;
	ifstream inFile;
	vector<char> charVector;


	do
	{
		//sOption = "";
		cout << "Write the desired maze to load. <Example: quickest_route_1.txt>\n";
		//cin >> sOption;

		cout << sOption << endl;

		inFile.open(sOption);
		

		bool bCountColumnsComplete = false;
		if (inFile) {
			cout << "LOADED SUCCESSFULLY\n";
			char a;
			while (!inFile.eof()) {
				inFile.get(a);
				cout << a;
				charVector.push_back(a);
				
				if(!bCountColumnsComplete && a != '\n'){
					gridSize.x++;
				}
				if (a == '\n') {
					bCountColumnsComplete = true;
					gridSize.y++;
					cout << endl;
				}
			}

		}
		else {
			cout << "ERROR LOADED\n";
		}

		


		cout << endl << "Input File have " << gridSize.x << " columns and " << gridSize.y << " Rows.\n";


		bValidInput = true;

	} while (!bValidInput);

	inFile.close();

	InitArray(charVector, gridSize);
	ConectNodes(gridSize);
	if (ReadInputFile(charVector, gridSize)) { //If the Input file have Start and End Point.
		A_Star_Algorithm(gridSize);
		DrawGrid(gridSize);
	
	}


	return 0;
}
