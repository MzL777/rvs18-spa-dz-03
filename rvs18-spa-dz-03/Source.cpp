#include <math.h>
#include "Maze.h"

using namespace std;

// Variables (pointers) used in rendering thread
Maze* pgame;
string* proute; 
Unit* pUnit = nullptr;
bool calculating = 0;

// Rendering thread function
void renderingThread(sf::RenderWindow* window) {
	sf::Clock clock;
	sf::Font font;
	sf::Text text;
	bool ldd = font.loadFromFile("res/font/arial.ttf");
	if (!ldd)cout << "Unable to load font!\n";
	else {
		text.setFont(font);
		text.setString("Press Right Mouse Button to help your sibling find his way!");
		text.setCharacterSize(16);
		text.setStyle(sf::Text::Bold);
		text.setPosition(window->getPosition().x / 2, window->getPosition().y / 2 );
	}
	clock.restart();
	window->setActive(true);
	while (window->isOpen()) {
		text.setFont(font);
		window->clear();
		pgame->Draw(proute, pUnit, &calculating);
		if (ldd) {
			if (clock.getElapsedTime().asMilliseconds() > 7000)
				clock.restart();
			else if (clock.getElapsedTime().asMilliseconds() > 3500) {
				text.setFillColor(sf::Color(255, 0, 0, abs(7000 - clock.getElapsedTime().asMilliseconds()) / 14.f));
				window->draw(text);
			}
			else {
				text.setFillColor(sf::Color(255, 0, 0, clock.getElapsedTime().asMilliseconds() / 14.f));
				window->draw(text);
			}
		}
		window->display();
	}
}


//	---  A* PATHFINDING  ---
const int n = 20;							// vertical size of the map
const int m = 30;							// horizontal size size of the map
static int _map[m * 4][n * 4];
static int closed_nodes_map[m * 4][n * 4];	// map of closed (tried-out) nodes
static int open_nodes_map[m * 4][n * 4];	// map of open (not-yet-tried) nodes
static int dir_map[m * 4][n * 4];			// map of directions
const int dir = 8;							// number of possible directions to go at any position
static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

class node {
	int xPos;	// current position
	int yPos;
	int level;	// total distance already travelled to reach the node
	int priority;  // priority=level+remaining distance estimate -> smaller: higher priority

public:
	node(int xp, int yp, int d, int p) {
		xPos = xp; yPos = yp; level = d; priority = p;
	}

	int getxPos() const { return xPos; }
	int getyPos() const { return yPos; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }

	void updatePriority(const int & xDest, const int & yDest) {
		priority = level + estimate(xDest, yDest) * 10; //A*
	}

	// give better priority to going straight instead of diagonally
	void nextLevel(const int & i) {	// i: direction
		level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	}

	// Estimation function for the remaining distance to the goal.
	const int & estimate(const int & xDest, const int & yDest) const {
		static int xd, yd, d;
		xd = xDest - xPos;
		yd = yDest - yPos;

		// Euclidian Distance
		//d = static_cast<int>(sqrt(xd * xd + yd * yd));

		// Manhattan distance
		d=abs(xd)+abs(yd);

		// Chebyshev distance
		//d=max(abs(xd), abs(yd));

		return(d);
	}
};

// Determine priority (in the priority queue)
bool operator<(const node & a, const node & b) {
	return a.getPriority() > b.getPriority();
}

// The route returned is a string of direction digits.
string pathFind(const int xStart, const int yStart, const int xFinish, const int yFinish) {
	static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
	static int pqi; // pq index
	static node* n0;
	static node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqi = 0;

	// reset the node maps
	for (y = 0; y < n * 4; y++)
		for (x = 0; x < m * 4; x++) {
			closed_nodes_map[x][y] = 0;
			open_nodes_map[x][y] = 0;
		}

	// create the start node and push into list of open nodes
	n0 = new node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

											  // A* search
	while (!pq[pqi].empty()) {
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x][y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish) {
			// generate the path from finish to start
			// by following the directions
			string path = "";
			while (!(x == xStart && y == yStart)) {
				j = dir_map[x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i < dir; i++) {
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx < 0 || xdx > (m * 4 - 1) || ydy < 0 || ydy > (n * 4 - 1) || _map[xdx][ydy] == 1 || closed_nodes_map[xdx][ydy] == 1)) {
				// generate a child node
				m0 = new node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish);

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx][ydy] == 0) {
					open_nodes_map[xdx][ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					// mark its parent node direction
					dir_map[xdx][ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx][ydy]>m0->getPriority()) {
					// update the priority info
					open_nodes_map[xdx][ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx][ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx && pq[pqi].top().getyPos() == ydy)) {
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty()) {
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	cout << "No route found!" << endl;
	return ""; // no route found
}


int main() {
	// Variables
	bool isPressedR = 0;

	string route = ""; proute = &route;
	Unit El;	pUnit = &El;

	sf::Vector2i Finish(0, 0);
	srand(time(nullptr));

	// Initialize window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello, SFML world! &ln(a)", sf::Style::Default, settings);
	window.setFramerateLimit(45);
	sf::View view(sf::FloatRect(0, 0, 20 * m, 20 * n));
	window.setView(view);
	window.setActive(false);

	// Generate map
	Maze game(&window, m, n);
	pgame = &game;
	game.Update();

	// Save map to array on heap
	auto v = game.getMap();
	for (int i = 0; i < n * 4; ++i)
		for (int j = 0; j < m * 4; ++j)
			_map[j][i] = v[j][i];

	// Start rendering thread
	cout << "Looping...\n";
	sf::Thread rthread(&renderingThread, &window);
	rthread.launch();

	// Loop event detection
	while (window.isOpen()) {
		// Events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// Mouse
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			if (!isPressedR) {
				calculating = 1;	// set flag used in rendering thread
				isPressedR = 1;		// disable rmb press detection while it is already down

				// save pressed position to unit's targer point
				auto temp = sf::Mouse::getPosition(window);
				if (temp.x >= 0 && temp.x <= window.getSize().x)
					Finish.x = int(temp.x / (window.getSize().x / (m * 4.0)));
				if (temp.y >= 0 && temp.y <= window.getSize().y)
					Finish.y = int(temp.y / (window.getSize().y / (n * 4.0)));

				// calculate route from unit's position to it's new target point
				route = pathFind(El.X, El.Y, Finish.x, Finish.y);
				El.tX = Finish.x; El.tY = Finish.y;
				
				if(route != "")
					cout << "Start(" << El.X << ", " << El.Y << ");\tFinish(" << Finish.x << ", " << Finish.y << ")\n";
				calculating = 0;	// clear flag used in rendering thread
				game.Step();
			}
		}
		else isPressedR = false;
	}
	return(0);
}