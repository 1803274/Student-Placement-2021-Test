#include "Application.h"

Application::Application()
{
	RestartApplication();	
}

Application::~Application()
{
	nodes = nullptr;
	delete nodes;

	nodeStart = nullptr;
	delete nodeStart;

	nodeEnd = nullptr;
	delete nodeEnd;
}

void Application::RestartApplication()
{
	nodes = nullptr;
	nodeStart = nullptr;
	nodeEnd = nullptr;
	StartPoint.x = NULL;
	StartPoint.y = NULL;
	sOutput = "";
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	charVector.clear();
	sOption = "default option";
	InputInfo = "";
	gridSize.x = 0;
	gridSize.y = 1;
	bValidInput = false;
	bMenuValid = false;
	bExitProgram = false;
}

void Application::PrintIntro()
{
	cout << "Playground Games: Student Placement Test 2021"<< endl <<
		"Name: Daniel Martinez Miguel" << endl << 
		"Email: danielmartinezdev@gmail.com" << endl;
}

void Application::InitArray()
{
	nodes = new Node[gridSize.x * gridSize.y];
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++) {
			int index = y * gridSize.x + x;
			nodes[index].x = x;
			nodes[index].y = y;
			nodes[index].bObstacle = false;
			nodes[index].parent = nullptr;
			nodes[index].bVisited = false;
			nodes[index].bPath = false;
		}
	}
}

void Application::ConnectNodes()
{
	for (int x = 0; x < gridSize.x; x++) {
		for (int y = 0; y < gridSize.y; y++) {
			int index = y * gridSize.x + x;
			if (y > 0) {
				nodes[index].conections_neighbours.insert(&nodes[(y - 1) * gridSize.x + (x + 0)]);
			}
			if (y < gridSize.y - 1) {
				nodes[index].conections_neighbours.insert(&nodes[(y + 1) * gridSize.x + (x + 0)]);
			}
			if (x > 0) {
				nodes[index].conections_neighbours.insert(&nodes[(y + 0) * gridSize.x + (x - 1)]);
			}
			if (x < gridSize.x - 1) {
				nodes[index].conections_neighbours.insert(&nodes[(y + 0) * gridSize.x + (x + 1)]);
			}
		}
	}
}

bool Application::ReadInputFile()
{
	int x = 0;
	int y = 0;
	bool bEndNode = false;
	bool bStartNode = false;

	for (auto c : charVector) {
		c = toupper(c);
		switch (c)
		{
		case 'X':
			nodes[y * gridSize.x + x].bObstacle = true;
			x++;
			break;
		case 'A':
			nodeStart = &nodes[y * gridSize.x + x];
			StartPoint.x = x;
			StartPoint.y = y;
			x++;
			break;
		case 'B':
			nodeEnd = &nodes[y * gridSize.x + x];
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

	if (!nodeStart || !nodeEnd) {
		if (!nodeStart) {
			cout << "Missing Start Point\n";
		}
		if (!nodeEnd) {
			cout << "Missing End Point\n";
		}
		return false;
	}
	else {
		return true;
	}
}

void Application::ResetNodes()
{
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			int index = y * gridSize.x + x;
			nodes[index].bVisited = false;
			nodes[index].DistanceToEndGlobal = INFINITY;
			nodes[index].DistanceToEndLocal = INFINITY;
			nodes[index].parent = nullptr;
		}
	}
}

void Application::A_Star_Algorithm()
{
	//Reset Node States
	ResetNodes();

	//Heuristics
	auto distance = [](Node* a, Node* b) {
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](Node* a, Node* b) {
		return distance(a, b);
	};

	//Starting Conditions
	Node* currentNode = nodeStart;
	nodeStart->DistanceToEndLocal = 0.f;
	nodeStart->DistanceToEndGlobal = heuristic(nodeStart, nodeEnd);

	//Create untested node list and add the start node
	priority_queue<Node*, vector<Node*>, compare> untested_nodes_list;
	untested_nodes_list.push(nodeStart);

	while (!untested_nodes_list.empty() && currentNode != nodeEnd) {
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
				node_neighbour->DistanceToEndGlobal = node_neighbour->DistanceToEndLocal + heuristic(node_neighbour, nodeEnd);
			}
		}
	}

	if (nodeEnd) {
		Node* p = nodeEnd;

		while (p) {
			p->bPath = true;
			if (p->parent) {
				if (p->parent->x < p->x) {
					sOutput.insert(0, "E");
				}
				else if (p->parent->x > p->x) {
					sOutput.insert(0, "W");
				}
				else if (p->parent->y < p->y) {
					sOutput.insert(0, "S");
				}
				else if (p->parent->y > p->y) {
					sOutput.insert(0, "N");
				}
			}
			p = p->parent;
		}
		if (sOutput == "") {
			sOutput = "There is no escape from this maze. :(\n";
		}
	}
}

void Application::DrawGrid()
{

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
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else if (x == EndPoint.x && y == EndPoint.y) {
				SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				cout << "B ";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else if (nodes[assemble].bPath) {
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "- ";
				SetConsoleTextAttribute(h, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				cout << ". ";
			}
		}
		cout << endl;
	}
}

void Application::runApp()
{
	PrintIntro();
	do
	{
		do
		{
			int menu = 0;
			cout << "Map selection menu: \n0- Introduce new filename.\n1- Select map <Quicktest Route 1>\n2- Select map <Quicktest Route 2>\n3- Select map <Quicktest Route 3>\n4- Select map <Quicktest Route 4>\n5- Exit. \nSelect one of the options: ";
			cin >> menu;
			bValidInput = true;
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
				return;
				break;
			default:
				bValidInput = false;
				break;
			}
			if (bValidInput)
			{
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
				else
				{
					cout << "Error 404: Filename not found.\n";
					bValidInput = false;
				}
			} 
		} while (!bValidInput);

		system("cls");
		PrintIntro();
		cout << "Map Name: " << sOption << endl << endl;
		InitArray();
		ConnectNodes();
		if (ReadInputFile()) {
			A_Star_Algorithm();
			DrawGrid();
		}
		cout << endl << "Maze Solution: " << sOutput << endl << endl;
		RestartApplication();
	} while (!bExitProgram);
}