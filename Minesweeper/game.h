#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

class CGame
{
public:
	CGame(const int& width, const int& height, const int& tw, const int& th);
	CGame() = delete;
	~CGame() {}

	void Render(sf::RenderWindow& window);

	void PickTile(const int& row, const int& col);

	bool HasWon();

	int GetLevel() const { return mLevel; }

	void PlayNextLevel();

	void Win() { mOver = true; }

	void Reset();

	bool HasLost() const { return mLost; }

	class Tile
	{
	public:
		Tile(CGame* game);
		Tile() = delete;
		~Tile() {}

		void SetBomb(bool val) { mBomb = val; }
		bool IsBomb() const { return mBomb; }

		void SetHidden(bool val) { mHidden = val; }
		bool IsHidden() const { return mHidden; }

		void SetNumber(const int& num) { mNum = num; }
		int GetNum() const { return mNum; }

		sf::RectangleShape& GetTile() { return mTile; }

		void Reset();

		void PropogateFlips();

		void Hit() { mHit = true; }
		bool IsHit() const { return mHit; }
	private:
		CGame* mGame = nullptr;

		sf::RectangleShape mTile;
		int mNum = -1;
		
		bool mHit = false;

		bool mBomb = false;
		bool mHidden = true;
	};

private:
	void AssignNumbers();
	void PlaceMines();
	void BuildBoard();
	void DestroyBoard();
	
	int CountBombs(const int& row, const int& col);

	sf::Font mFont;
	sf::Text mText;
	sf::Text mLevelText;

	// Board width, board height, tile width, tile height
	int mWidth, mHeight, mTileWidth, mTileHeight;

	bool mOver = false;
	bool mLost = false;

	// Board
	std::vector<std::vector<std::shared_ptr<Tile>>> mBoard;

	// Current level
	int mLevel = 0;
};

