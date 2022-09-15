#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <stdio.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::Vector2f round(const sf::Vector2f vector)
{
	return sf::Vector2f{ std::round(vector.x), std::round(vector.y) };
}

class gameBoard {
private:
	unsigned int gameArray[4][4];
	std::map<unsigned int, sf::Color> colorMap;
	void initializeColorMap() {
		colorMap[2] =     sf::Color(238, 228, 218, 255);
		colorMap[4] =     sf::Color(238, 225, 201, 255);
		colorMap[8] =     sf::Color(243, 178, 122, 255);
		colorMap[16] =    sf::Color(246, 150, 100, 255);
		colorMap[32] =    sf::Color(51, 173, 86, 255);
		colorMap[64] =    sf::Color(62, 193, 62, 255);
		colorMap[128] =   sf::Color(50, 168, 164, 255);
		colorMap[256] =   sf::Color(50, 146, 168, 255);
		colorMap[512] =   sf::Color(186, 47, 47, 255);
		colorMap[1024] =  sf::Color(219, 35, 112, 255);
		colorMap[2048] =  sf::Color(184, 37, 184, 255);
		colorMap[4096] =  sf::Color(255, 77, 187, 255);
		colorMap[8192] =  sf::Color(214, 0, 107, 255);
		colorMap[16384] = sf::Color(204, 51, 255, 255);
		colorMap[32768] = sf::Color(82, 82, 82, 255);
	}
public:
	sf::Color getNumberColor(unsigned int value) {
		if (colorMap.count(value)) {
			return colorMap[value];
		}
		return colorMap[32768];
	}
	void drawGrid(sf::RenderWindow * window) {
		sf::Font font;
		if (!font.loadFromFile("OSReg.ttf"))
		{
			std::cout << "Failed to load font" << std::endl;
		}
		int row, col;
		for (row = 0; row < 4; row++) {
			for (col = 0; col < 4; col++) {
				
				if (gameArray[row][col] != 0) {
					sf::Text number{ std::to_string(gameArray[row][col]), font};
					sf::RectangleShape tile(sf::Vector2f(100.f, 100.f));
					tile.setPosition(200 + row * 100, 200 + col * 100);
					sf::Color numberColor = getNumberColor(gameArray[row][col]);
					tile.setFillColor(numberColor);
					number.setCharacterSize(24);
					number.setFillColor(sf::Color::Black);
					number.setPosition(210 + row * 100, 210 + col * 100);
					

					window->draw(tile);
					window->draw(number);
				}

				sf::Vertex line_col[] =
				{
					sf::Vertex(sf::Vector2f(100 + ((row + 1) * 100), 200 + ((col + 1) * 100))),
					sf::Vertex(sf::Vector2f(100 + ((row + 1) * 100),  100 + ((col + 1) * 100)))
				};

				sf::Vertex line_row[] =
				{
					sf::Vertex(sf::Vector2f(200 + ((row + 1) * 100), 100 + ((col + 1) * 100))),
					sf::Vertex(sf::Vector2f(100 + ((row + 1) * 100), 100 + ((col + 1) * 100)))
				};

				if (col == 0 && row == 0) {
					sf::Vertex line_row_end[] =
					{
						sf::Vertex(sf::Vector2f(200, 600)),
						sf::Vertex(sf::Vector2f(600, 600))
					};
					sf::Vertex line_col_end[] =
					{
						sf::Vertex(sf::Vector2f(600, 200)),
						sf::Vertex(sf::Vector2f(600, 600))
					};
					window->draw(line_col_end, 2, sf::Lines);
					window->draw(line_row_end, 2, sf::Lines);
				}

				window->draw(line_col, 2, sf::Lines);
				window->draw(line_row, 2, sf::Lines);
			}
		}
	}
	unsigned int * returnRandomTile() {
		int x, y, value;
		x = y = value = 99;
		unsigned int randomTile[3];
		bool emptySpots = false;

		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int col = 0; col < 4; col++) {
				if (gameArray[row][col] == 0) {
					emptySpots = true;
					break;
				}
			}
		}
		srand((unsigned)time(NULL));
		do {
			if (emptySpots == true){
				x = rand() % 4;
				y = rand() % 4;
			}
		} while (gameArray[x][y] != 0 && emptySpots == true);


		value = 2;

		if (rand() % 100 > 55) {
			value = 4;
		}
		randomTile[0] = x;
		randomTile[1] = y;
		randomTile[2] = value;

		return randomTile;
	}
	bool setRandomTile(unsigned int row, unsigned int col, unsigned int val) {
		std::cout << row;
		std::cout << col << val << std::endl;
		if (row == 99 || col == 99) return false;
		gameArray[row][col] = val;
		return true;
	}
	void initGameBoard() {
		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int col = 0; col < 4; col++) {
				gameArray[row][col] = 0;
			}
		}
		unsigned int* newRandomTileDat = returnRandomTile();
		if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
			std::cout << "Gameover?";
		}
	}
	bool moveRight() {
		bool success = false;
		for (int row = 0; row < 4; row++) {
			int streak = 0;
			int memValue = 0;
			for (int col = 3; col >= 0; col--) {
				int thisValue = gameArray[col][row];

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					int col_2 = col;
					while (col_2 + 1 < 4 && gameArray[col_2 + 1][row] == 0) {
						col_2++;
					}
					if (col_2 != col) {
						gameArray[col][row] = 0;
						gameArray[col_2][row] = thisValue;
						success = true;
					}
					if (thisValue == memValue && streak == 2 && col_2 + 1 < 4) {
						gameArray[col_2 + 1][row] = thisValue * 2;
						gameArray[col_2][row] = 0;
					}
				}

			}
		}
		return success;
	}
	bool moveLeft() {
		bool success = false;
		
		for (int row = 0; row < 4; row++) {
			int streak = 0;
			int memValue = 0;
			for (int col = 0; col < 4; col++) {
				int thisValue = gameArray[col][row];

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue = thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					int col_2 = col;
					while (col_2 - 1 >= 0 && gameArray[col_2 - 1][row] == 0) {
						col_2--;
					}

					if (col_2 != col) {
						gameArray[col][row] = 0;
						gameArray[col_2][row] = thisValue;
						success = true;
					}
					if (thisValue == memValue && streak == 2 && col_2 - 1 >= 0) {
						gameArray[col_2 - 1][row] = thisValue * 2;
						gameArray[col_2][row] = 0;
					}
				}
			}
		}

		return success;
	}
	bool moveDown(){
		bool success = false;
		unsigned int moveTotal = 0;

		for (int col = 0; col < 4; col++) {
			int streak = 0;
			int memValue = 0;

			for (int row = 3; row >= 0; row--) {
				int thisValue = gameArray[col][row];

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					int row_2 = row;
					while (row_2 + 1 < 4 && gameArray[col][row_2 + 1] == 0) {
						row_2++;
					}
					if (row_2 != row) {
						gameArray[col][row_2] = thisValue;
						gameArray[col][row] = 0;
						success = true;
					}

					if (thisValue == memValue && streak == 2 && row_2 + 1 < 4) {
						gameArray[col][row_2 + 1] = thisValue * 2;
						gameArray[col][row_2] = 0;
					}
				}
			}
		}
		return success;
	}
	bool moveUp() {
		bool success = false;
		unsigned int moveTotal = 0;

		for (unsigned int row = 0; row < 4; row++) {
			unsigned int streak = 0;
			int memValue = 0;

			for (unsigned int col = 0; col < 4; col++) {
				
				unsigned int thisValue = gameArray[row][col];

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					unsigned int col_2 = col;

					do {
						col_2--;
					} while (col_2 < 4 && gameArray[row][col_2] == 0);
					col_2++;

					if (col_2 != col) {
						gameArray[row][col_2] = thisValue;
						gameArray[row][col] = 0;
						success = true;
					}

					if (thisValue == memValue && streak == 2) {
						for (unsigned int index = 0; index < 2; index++) {
							gameArray[row][col_2 - index] = 0;
							if (index == 2 - 1) {
								// this.actuateTile(row, col_2, row, col_2 - index, thisValue * this.requiredStreak);
								gameArray[row][col_2 - index] = thisValue * 2;
								col_2 = col_2 - index;
							}
						}
					}
				}

			}
		}
		return success;
	}
	bool actuateBoard(unsigned int keycode) {
		bool success = false;
		unsigned int * newRandomTileDat;
		switch (keycode) {
		case 73:
			success = moveUp();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		case 74 :
			success = moveDown();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		case 71:
			success = moveLeft();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		case 72:
			success = moveRight();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		}
		return success;
	}
	gameBoard() {
		initGameBoard();
		initializeColorMap();
	}
};

gameBoard newGameBoard;

void mainControls(sf::RenderWindow* window) {
	sf::Event event;

	while (window->pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
			// window closed
		case sf::Event::Closed:
			window->close();
			break;

			// key pressed
		case sf::Event::KeyPressed:
			/*std::cout << "Keypressed!";
			newGameBoard.doSomething();*/
			//left = 71 up = 73 right = 72 down = 74
			newGameBoard.actuateBoard(event.key.code);
			break;

			// we don't process other types of events
		default:
			break;
		}
	}
}
int main(int argc, char** argv) {
	sf::RenderWindow window;
	window.create(sf::VideoMode(768, 768), "2048");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		window.clear(sf::Color::Black);
		newGameBoard.drawGrid(&window);
		mainControls(&window);
		window.display();
	}
	return 0;
}
