#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

struct Unit {
	int X = 0;
	int Y = 0;
	int tX = 0;
	int tY = 0;
};

class Maze {
private:
	bool Done = 0;
	int step = 0;
	int tempx = 0, tempy = 0;
	const int dx[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
	const int dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };
	const int dr[8] = { 0, 45, 90, 135, 180, 225, 270, 315 };

	sf::Time Elapsed;
	sf::Clock PClock;
	sf::Texture tex;
	sf::RectangleShape line;
	sf::RectangleShape bruda;
	sf::Vertex lines[2] = {
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(0, 0))
	};
	sf::RenderWindow* window;

	chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

	enum {
		CELL_PATH_N = 0x01,
		CELL_PATH_E = 0x02,
		CELL_PATH_S = 0x04,
		CELL_PATH_W = 0x08,
		CELL_VISITED = 0x10,
	};

	int *m_maze;
	int  m_nMazeWidth;
	int  m_nMazeHeight;
	int  m_nVisitedCells;
	int  m_nPathWidth;
	stack<pair<int, int>> m_stack;	// (x, y) coordinate pairs

public:
	Maze(sf::RenderWindow* window, int Columns, int Rows) {
		this->window = window;

		// Maze parameters
		m_nMazeWidth = Rows;
		m_nMazeHeight = Columns;
		m_maze = new int[m_nMazeWidth * m_nMazeHeight];
		memset(m_maze, 0x00, m_nMazeWidth * m_nMazeHeight * sizeof(int));

		// Drawing-related
		m_nPathWidth = 4;
		line = sf::RectangleShape(sf::Vector2f(m_nPathWidth, m_nPathWidth));
		line.setScale(1, 1);
		line.setFillColor(sf::Color(200, 200, 200));
		line.setTexture(nullptr);

		if (!tex.loadFromFile("res/tex/tex.png")) cout << "Unable to load texture!" << endl;
		bruda = sf::RectangleShape(sf::Vector2f(2 * m_nPathWidth, 2 * m_nPathWidth));
		bruda.setScale(1, 1);
		bruda.setFillColor(sf::Color(255, 255, 255));
		bruda.setTexture(&tex);

		lines[0].color = sf::Color(240, 22, 130);
		lines[1].color = sf::Color(240, 22, 130);

		// Generation variables
		end = begin;
		Done = false;
		cout << "Generating maze...\n";
		begin = chrono::high_resolution_clock::now();
		int x = rand() % m_nMazeWidth;
		int y = rand() % m_nMazeHeight;

		// Maze
		m_stack.push(make_pair(x, y));
		m_maze[y * m_nMazeWidth + x] = CELL_VISITED;
		m_nVisitedCells = 1;
		PClock.restart();
		Elapsed = PClock.getElapsedTime();
	}
	~Maze() {
		delete[] m_maze;
	}

	vector<vector<bool>> getMap() {
		vector<vector<bool>> v;
		v.resize(m_nMazeHeight * m_nPathWidth);
		
		for (int y = 0; y < m_nMazeHeight; ++y) {
			v[y * m_nPathWidth].resize(m_nMazeWidth * (m_nPathWidth));
			v[y * m_nPathWidth + 1].resize(m_nMazeWidth * (m_nPathWidth));
			v[y * m_nPathWidth + 2].resize(m_nMazeWidth * (m_nPathWidth));
			v[y * m_nPathWidth + 3].resize(m_nMazeWidth * (m_nPathWidth));
			for (int x = 0; x < m_nMazeWidth; ++x) {
				v[y * m_nPathWidth][x * m_nPathWidth] = 0;
				v[y * m_nPathWidth][x * m_nPathWidth + 1] = 0;
				v[y * m_nPathWidth][x * m_nPathWidth + 2] = 0;
				v[y * m_nPathWidth + 1][x * m_nPathWidth] = 0;
				v[y * m_nPathWidth + 1][x * m_nPathWidth + 1] = 0;
				v[y * m_nPathWidth + 1][x * m_nPathWidth + 2] = 0;
				v[y * m_nPathWidth + 2][x * m_nPathWidth] = 0;
				v[y * m_nPathWidth + 2][x * m_nPathWidth + 1] = 0;
				v[y * m_nPathWidth + 2][x * m_nPathWidth + 2] = 0;

				if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_E) {
					v[y * m_nPathWidth][x * m_nPathWidth + 3] = 0;
					v[y * m_nPathWidth + 1][x * m_nPathWidth + 3] = 0;
					v[y * m_nPathWidth + 2][x * m_nPathWidth + 3] = 0;
				}
				else {
					v[y * m_nPathWidth][x * m_nPathWidth + 3] = 1;
					v[y * m_nPathWidth + 1][x * m_nPathWidth + 3] = 1;
					v[y * m_nPathWidth + 2][x * m_nPathWidth + 3] = 1;
				}

				if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_S) {
					v[y * m_nPathWidth + 3][x * m_nPathWidth] = 0;
					v[y * m_nPathWidth + 3][x * m_nPathWidth + 1] = 0;
					v[y * m_nPathWidth + 3][x * m_nPathWidth + 2] = 0;
				}
				else {
					v[y * m_nPathWidth + 3][x * m_nPathWidth] = 1;
					v[y * m_nPathWidth + 3][x * m_nPathWidth + 1] = 1;
					v[y * m_nPathWidth + 3][x * m_nPathWidth + 2] = 1;
				}
				v[y * m_nPathWidth + 3][x * m_nPathWidth + 3] = 1;
			}
		}
		
		return v;
	}

	void Draw(string* r, Unit* pUnit, bool* calc) {

		// Draw Maze
		for (int x = 0; x < m_nMazeWidth; x++) {
			for (int y = 0; y < m_nMazeHeight; y++) {
				// Each cell is inflated by m_nPathWidth, so fill it in
				for (int py = 0; py < m_nPathWidth; py++)
					for (int px = 0; px < m_nPathWidth; px++) {
						if (m_maze[y * m_nMazeWidth + x] & CELL_VISITED) {
							line.setPosition(m_nPathWidth * (y * (m_nPathWidth + 1) + px), m_nPathWidth * (x * (m_nPathWidth + 1) + py));
							window->draw(line);
						}
					}

				// Draw passageways between cells
				for (int p = 0; p < m_nPathWidth; p++) {
					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_S) {
						line.setPosition(m_nPathWidth * (y * (m_nPathWidth + 1) + m_nPathWidth), m_nPathWidth * (x * (m_nPathWidth + 1) + p));
						window->draw(line);
					}
					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_E) {
						line.setPosition(m_nPathWidth * (y * (m_nPathWidth + 1) + p), m_nPathWidth * (x * (m_nPathWidth + 1) + m_nPathWidth));
						window->draw(line);
					}
				}
			}
		}
		
		// Path variables
		if (step == 0) {
			tempx = pUnit->X;
			tempy = pUnit->Y;
		}
		int sx = tempx * (m_nPathWidth + 1);
		int sy = tempy * (m_nPathWidth + 1);
		
		// Draw path
		for (int i = 0; i < r->length(); ++i) {
			if (r->length() > 0 && !*calc) {
				lines[0].position = sf::Vector2f(sx + 3, sy + 3);
				sx += dx[int((*r)[i]) - 48] * (m_nPathWidth + 1);
				sy += dy[int((*r)[i]) - 48] * (m_nPathWidth + 1);
				lines[1].position = sf::Vector2f(sx + 3, sy + 3);
				window->draw(lines, 2, sf::Lines);
			}
		}

		// Unit calculations
		if (!*calc && step >= 0 && step < r->length()) {
			pUnit->X += dx[int((*r)[step]) - 48];
			pUnit->Y += dy[int((*r)[step++]) - 48];
		}
		// Target reached?
		if (pUnit->X == pUnit->tX && pUnit->Y == pUnit->tY) step = -1;
		
		// Draw unit
		bruda.setPosition(pUnit->X * (m_nPathWidth + 1), pUnit->Y * (m_nPathWidth + 1));
		window->draw(bruda);
	}
	void Step() { step = 0; }
	bool isDone() { return Done; }
	bool Update() {
		while (!Done) {
			// Little lambda function to calculate index in a readable way
			auto offset = [&](int x, int y) {
				return (m_stack.top().second + y) * m_nMazeWidth + (m_stack.top().first + x);
			};

			// Do Maze Algorithm
			if (m_nVisitedCells < m_nMazeWidth * m_nMazeHeight) {
				// Create a set of unvisted neighbours
				vector<int> neighbours;

				// North neighbour
				if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
					neighbours.push_back(0);
				// East neighbour
				if (m_stack.top().first < m_nMazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
					neighbours.push_back(1);
				// South neighbour
				if (m_stack.top().second < m_nMazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
					neighbours.push_back(2);
				// West neighbour
				if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
					neighbours.push_back(3);

				// Are there any neighbours available?
				if (!neighbours.empty()) {
					// Choose one available neighbour at random
					int next_cell_dir = neighbours[rand() % neighbours.size()];
					// Create a path between the neighbour and the current cell
					switch (next_cell_dir) {
					case 0: // North
						m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
						m_maze[offset(0, 0)] |= CELL_PATH_N;
						m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
						break;
					case 1: // East
						m_maze[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
						m_maze[offset(0, 0)] |= CELL_PATH_E;
						m_stack.push(make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
						break;
					case 2: // South
						m_maze[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
						m_maze[offset(0, 0)] |= CELL_PATH_S;
						m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));
						break;
					case 3: // West
						m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
						m_maze[offset(0, 0)] |= CELL_PATH_W;
						m_stack.push(make_pair((m_stack.top().first - 1), (m_stack.top().second + 0)));
						break;
					}

					m_nVisitedCells++;
				}
				else {
					// No available neighbours so backtrack!
					m_stack.pop();
				}
			}
			else {
				if (!Done) {
					end = chrono::high_resolution_clock::now();
					cout << "Maze generated in " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms\n";
					Done = true;
				}
			}
		}
		return true;
	}
};