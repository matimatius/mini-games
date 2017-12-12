#pragma once
#define cellSize 60
#define cellCount 4

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	enum directions {
		up,
		down,
		right,
		left
	};


	struct cell {
		ofVec2f pos;
		ofVec2f *futurePos = NULL;
		int value = 2;
		int speed = 8;

		int mergeRow;
		int mergeCol;

		void draw() {
			ofColor c;
			if (value == 2)
				c = ofColor(246, 255, 156);
			if (value == 4)
				c = ofColor(238, 215, 143);
			if (value == 8)
				c = ofColor(252, 199, 108);
			if (value == 16)
				c = ofColor(229, 136, 65);
			if (value == 32)
				c = ofColor(220, 90, 86);
			if (value == 64)
				c = ofColor(216, 58, 10);
			if (value == 128)
				c = ofColor(255, 255, 102);
			if (value == 256)
				c = ofColor(255, 255, 51);
			if (value == 512)
				c = ofColor(104, 104, 0);
			if (value == 1024)
				c = ofColor::mediumTurquoise;
			if (value >= 2014)
				c = ofColor::gold;

			ofSetColor(c);
			ofRect(pos, cellSize, cellSize);
			ofSetColor(ofColor::black);
			ofDrawBitmapString(ofToString(value), pos.x + 24, pos.y + 32);
		}

		void update() {
			if (futurePos) {
				if (futurePos->x > pos.x) {
					pos.x += speed;
				}
				else if (futurePos->x < pos.x) {
					pos.x -= speed;
				}
				else if (futurePos->y < pos.y) {
					pos.y -= speed;
				}
				else if (futurePos->y > pos.y) {
					pos.y += speed;
				}

				if (futurePos->distance(pos) < speed) {
					pos = *futurePos;
					futurePos = NULL;
				}
			}
		}
	};


	struct grid {
		cell * cells[cellCount][cellCount];
		vector <cell*> freeCells;
		bool moving = false;
		bool checkmove = false;
		bool addNew = true;

		void mergeHelpRow(int i, int row, int col) {
			cells[i][col]->futurePos = &cells[row][col]->pos;
			cells[i][col]->mergeCol = col;
			cells[i][col]->mergeRow = row;
			freeCells.push_back(cells[i][col]);
			cells[i][col] = NULL;
		}

		void mergeHelpCol(int i, int row, int col) {
			cells[row][i]->futurePos = &cells[row][col]->pos;
			cells[row][i]->mergeCol = col;
			cells[row][i]->mergeRow = row;
			freeCells.push_back(cells[row][i]);
			cells[row][i] = NULL;
		}

		void merge(int dir, int row, int col) {
			if (dir == up) {
				for (int i = row + 1; i < cellCount; i++) {
					if (cells[i][col]) {
						if (cells[i][col]->value == cells[row][col]->value) {
							mergeHelpRow(i, row, col);
						}
						return;
					}
				}
			}
			else if (dir == down) {
				for (int i = row - 1; i >= 0; i--) {
					if (cells[i][col]) {
						if (cells[i][col]->value == cells[row][col]->value) {
							mergeHelpRow(i, row, col);
						}
						return;
					}
				}
			}
			else if (dir == left) {
				for (int i = col + 1; i < cellCount; i++) {
					if (cells[row][i]) {
						if (cells[row][i]->value == cells[row][col]->value) {
							mergeHelpCol(i, row, col);
						}
						return;
					}
				}
			}
			else if (dir == right) {
				for (int i = col - 1; i >= 0; i--) {
					if (cells[row][i]) {
						if (cells[row][i]->value == cells[row][col]->value) {
							mergeHelpCol(i, row, col);
						}
						return;
					}
				}
			}
		}

		void cellPlacement(int dir, int &row, int &col) {
			if (dir == up) {
				for (int i = row - 1; i >= 0; i--) {
					if (cells[i][col]) {
						cells[i + 1][col] = cells[row][col];
						if (row != i + 1) {
							cells[row][col] = NULL;
							delete cells[i + 1][col]->futurePos;
							cells[i + 1][col]->futurePos = new ofVec2f(col * (cellSize + 4) + 2, (i + 1) * (cellSize + 4) + 2);
							row = i + 1;
						}
						return;
					}
				}
				cells[0][col] = cells[row][col];
				cells[row][col] = NULL;
				delete cells[0][col]->futurePos;
				cells[0][col]->futurePos = new ofVec2f(col * (cellSize + 4) + 2, 0 * (cellSize + 4) + 2);
				row = 0;
			}
			else if (dir == down) {
				for (int i = row + 1; i < cellCount; i++) {
					if (cells[i][col]) {
						if (row != i - 1) {
							cells[i - 1][col] = cells[row][col];
							cells[row][col] = NULL;
							delete cells[i - 1][col]->futurePos;
							cells[i - 1][col]->futurePos = new ofVec2f(col * (cellSize + 4) + 2, (i - 1) * (cellSize + 4) + 2);
							row = i - 1;
						}
						return;
					}
				}
				cells[cellCount - 1][col] = cells[row][col];
				cells[row][col] = NULL;
				delete cells[cellCount - 1][col]->futurePos;
				cells[cellCount - 1][col]->futurePos = new ofVec2f(col * (cellSize + 4) + 2, (cellCount - 1) * (cellSize + 4) + 2);
				row = cellCount - 1;
			}
			else if (dir == left) {
				for (int i = col - 1; i >= 0; i--) {
					if (cells[row][i]) {
						cells[row][i + 1] = cells[row][col];
						if (col != i + 1) {
							cells[row][col] = NULL;
							delete cells[row][i + 1]->futurePos;
							cells[row][i + 1]->futurePos = new ofVec2f((i + 1) * (cellSize + 4) + 2, row * (cellSize + 4) + 2);
							col = i + 1;
						}
						return;
					}
				}
				cells[row][0] = cells[row][col];
				cells[row][col] = NULL;
				delete cells[row][0]->futurePos;
				cells[row][0]->futurePos = new ofVec2f(0 * (cellSize + 4) + 2, row * (cellSize + 4) + 2);
				col = 0;
			}
			else if (dir == right) {
				for (int i = col + 1; i < cellCount; i++) {
					if (cells[row][i]) {
						cells[row][i - 1] = cells[row][col];
						if (col != i - 1) {
							cells[row][col] = NULL;
							delete cells[row][i - 1]->futurePos;
							cells[row][i - 1]->futurePos = new ofVec2f((i - 1) * (cellSize + 4) + 2, row * (cellSize + 4) + 2);
							col = i - 1;
						}
						return;
					}
				}
				cells[row][cellCount - 1] = cells[row][col];
				cells[row][col] = NULL;
				delete cells[row][cellCount - 1]->futurePos;
				cells[row][cellCount - 1]->futurePos = new ofVec2f((cellCount - 1) * (cellSize + 4) + 2, row * (cellSize + 4) + 2);
				col = cellCount - 1;
			}
		}

		void draw() {
			ofSetColor(ofColor::white);
			for (int i = 0; i < cellCount + 1; i++) {
				ofLine(0, i * (cellSize + 4), (cellSize + 4) * cellCount, i * (cellSize + 4));
			}
			for (int i = 0; i < cellCount + 1; i++) {
				ofLine(i * (cellSize + 4), 0, i * (cellSize + 4), cellCount * (cellSize + 4));
			}
			for (int i = 0; i < cellCount; i++) {
				for (int j = 0; j < cellCount; j++) {
					if (cells[i][j]) {
						cells[i][j]->draw();
					}
				}
			}
			for (int i = 0; i < freeCells.size(); i++) {
				freeCells[i]->draw();
			}
		}

		void update() {
			if (addNew) {
				vector <pair<int, int>> p;
				for (int i = 0; i < cellCount; i++) {
					for (int j = 0; j < cellCount; j++) {
						if (!cells[i][j])
							p.push_back(pair<int, int>(i, j));
					}
				}
				random_shuffle(p.begin(), p.end());
				cout << p[0].first << " " << p[0].second << endl;
				cells[p[0].first][p[0].second] = new cell;
				cells[p[0].first][p[0].second]->pos = ofVec2f(p[0].second * (cellSize + 4) + 2, p[0].first * (cellSize + 4) + 2);
				addNew = false;
			}

			if (moving) {
				checkmove = true;
			}

			moving = false;

			for (int i = 0; i < cellCount; i++) {
				for (int j = 0; j < cellCount; j++) {
					if (cells[i][j]) {
						cells[i][j]->update();
						if (cells[i][j]->futurePos) {
							moving = true;
						}
					}
				}
			}
			if (freeCells.size() > 0) {
				moving = true;
				for (int i = freeCells.size() - 1; i >= 0; i--) {
					freeCells[i]->update();
					if (!freeCells[i]->futurePos) {
						cells[freeCells[i]->mergeRow][freeCells[i]->mergeCol]->value *= 2;
						delete freeCells[i];
						freeCells.erase(freeCells.begin() + i);
					}
				}
			}

			if (checkmove) {
				if (!moving) {
					checkmove = false;
					addNew = true;
				}
			}
		}

		void move(int dir) {
			if (!moving) {
				for (int i = 0; i < cellCount; i++) {
					for (int j = 0; j < cellCount; j++) {
						if (cells[i][j]) {
							if (cells[i][j]->futurePos)
								return;
						}
					}
				}
				if (freeCells.size() > 0)
					return;

				if (dir == up) {
					for (int row = 0; row < cellCount; row++) {
						for (int col = 0; col < cellCount; col++) {
							if (cells[row][col]) {
								int newRow = row;
								if (row > 0)
									cellPlacement(dir, newRow, col);
								merge(dir, newRow, col);
							}
						}
					}
				}
				else if (dir == down) {
					for (int row = cellCount - 1; row >= 0; row--) {
						for (int col = 0; col < cellCount; col++) {
							if (cells[row][col]) {
								int newRow = row;
								if (row < cellCount - 1)
									cellPlacement(dir, newRow, col);
								merge(dir, newRow, col);
							}
						}
					}
				}
				else if (dir == left) {
					for (int col = 0; col < cellCount; col++) {
						for (int row = 0; row < cellCount; row++) {
							if (cells[row][col]) {
								int newCol = col;
								if (col > 0)
									cellPlacement(dir, row, newCol);
								merge(dir, row, newCol);
							}
						}
					}
				}
				else if (dir == right) {
					for (int col = cellCount - 1; col >= 0; col--) {
						for (int row = 0; row < cellCount; row++) {
							if (cells[row][col]) {
								int newCol = col;
								if (col < cellCount - 1)
									cellPlacement(dir, row, newCol);
								merge(dir, row, newCol);
							}
						}
					}
				}
			}
		}
		void restart() {
			for (int i = 0; i < cellCount; i++) {
				for (int j = 0; j < cellCount; j++) {
						delete cells[i][j];
					cells[i][j] = NULL;
				}
			}
			freeCells.clear();
			moving = false;
			addNew = true;
			checkmove = false;
		}
	};




	grid g;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

};
