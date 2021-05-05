#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

struct Tile
{
	sf::RectangleShape mTile;
	bool mBomb = false;
	int mNum = -1;
	bool mHidden = true;
};

class CGame
{
public:
	CGame(const int& width, const int& height, const int& tw, const int& th);
	CGame() = delete;
	~CGame() {}


	void Render(sf::RenderWindow& window);

	void PickTile(const int& row, const int& col);

private:
	void AssignNumbers();
	void PlaceMines();
	void PropogateFlips(const int& row, const int& col);

	int CountBombs(const int& row, const int& col);

	sf::Font mFont;
	sf::Text mText;

	// Board width, board height, tile width, tile height
	int mWidth, mHeight, mTw, mTh;
	std::vector<std::vector<std::shared_ptr<Tile>>> mBoard;

	// Current level
	int mLevel = 1;
};

