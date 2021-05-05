#include "game.h"

#include <iostream>
#include <string>

const int DIFF_MULTIPLIER = 10;

CGame::CGame(const int& width, const int& height, const int& tw, const int& th)
	: mWidth(width), mHeight(height), mTw(tw), mTh(th)
{
	for (size_t i = 0; i < height; i += th)
	{
		std::vector <std::shared_ptr<Tile>> new_row;
		for (size_t j = 0; j < width; j += tw)
		{
			new_row.push_back(std::make_shared<Tile>());
			new_row.back()->mTile.setSize(sf::Vector2f(tw, th));
			new_row.back()->mTile.setPosition(sf::Vector2f(j, i));
			new_row.back()->mTile.setFillColor(sf::Color(222,222,222,255));
			new_row.back()->mTile.setOutlineColor(sf::Color::Black);
			new_row.back()->mTile.setOutlineThickness(1.0f);
		}
		this->mBoard.push_back(new_row);
	}

	// Load Font
	if (!mFont.loadFromFile("fonts/arial.ttf"))
	{
		std::cerr << "Error loading font!\n";
		exit(-1);
	}

	mText.setFont(mFont);

	PlaceMines();

	AssignNumbers();
}

void CGame::Render(sf::RenderWindow& window)
{
	for (auto row : mBoard)
	{
		for (auto col : row)
		{
			window.draw(col->mTile);

			if (!col->mHidden)
			{
				mText.setString(std::to_string(col->mNum));
				mText.setPosition(col->mTile.getPosition() + sf::Vector2f(12, 12));
				mText.setCharacterSize(24);
				mText.setFillColor(sf::Color::Black);
				mText.setStyle(sf::Text::Bold);

				window.draw(mText);
			}
		}
	}
}

void CGame::PickTile(const int& row, const int& col)
{
	if (mBoard[row][col]->mBomb == true)
	{
		std::cout << "GAME OVER!!!!!!!!\n";
	}
	else if (mBoard[row][col]->mNum != 0)
	{
		mBoard[row][col]->mHidden = false;
	}
	else if (mBoard[row][col]->mNum == 0)
	{
		PropogateFlips(row, col);
	}
}

void CGame::AssignNumbers()
{
	for (size_t row = 0; row < (this->mWidth / mTw); row++)
	{
		for (size_t col = 0; col < (this->mHeight / mTh); col++)
		{
			mBoard[row][col]->mNum = CountBombs(row, col);
		}
	}
}

int CGame::CountBombs(const int& row, const int& col)
{
	int total = 0;
	int t = -1, b = -1, l = -1, r = -1;

	if (row > 0 && col > 0 && row < ((mHeight / mTh) - 1) && col < ((mWidth / mTw) - 1))
	{
		t = row - 1;
		b = row + 1;
		l = col - 1;
		r = col + 1;
	}

	if (row == 0) { t = (mHeight / mTh) - 1; }

	if (col == 0) { l = (mWidth / mTw) - 1; }

	if (row == (mHeight / mTh) - 1) { b = 0; }

	if (col == (mWidth / mTw) - 1) { r = 0; }

	(t == -1) ? t = row - 1 : 1;
	(b == -1) ? b = row + 1 : 1;
	(l == -1) ? l = col - 1 : 1;
	(r == -1) ? r = col + 1 : 1;

	if (mBoard[row][r]->mBomb) { total++; }

	if (mBoard[t][col]->mBomb) { total++; }

	if (mBoard[t][r]->mBomb) { total++; }

	if (mBoard[t][l]->mBomb) { total++; }

	if (mBoard[row][l]->mBomb) { total++; }

	if (mBoard[b][col]->mBomb) { total++; }

	if (mBoard[b][r]->mBomb) { total++; }

	if (mBoard[b][l]->mBomb) { total++; }

	return total;
}

void CGame::PlaceMines()
{
	int mines = mLevel * DIFF_MULTIPLIER;
	auto boundX = this->mWidth / mTw;
	auto boundY = this->mHeight / mTh;
	
	for (size_t i = 0; i < mines; i++)
	{
		int rX = (rand() % boundX);
		int rY = (rand() % boundY);
		
		this->mBoard[rY][rX]->mBomb = true;
		this->mBoard[rY][rX]->mTile.setFillColor(sf::Color::Black);
	}
}

void CGame::PropogateFlips(const int& row, const int& col)
{
}
