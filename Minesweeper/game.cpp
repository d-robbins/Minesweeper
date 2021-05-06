#include "game.h"

#include <iostream>
#include <string>

const int DIFF_MULTIPLIER = 3;

CGame::CGame(const int& width, const int& height, const int& tw, const int& th)
	: mWidth(width - 2 * tw), mHeight(height), mTileWidth(tw), mTileHeight(th)
{
	for (size_t i = 0; i < mHeight; i += mTileHeight)
	{
		std::vector <std::shared_ptr<Tile>> new_row;
		for (size_t j = 0; j < mWidth; j += mTileWidth)
		{
			new_row.push_back(std::make_shared<Tile>(this));
			new_row.back()->GetTile().setPosition(sf::Vector2f(j, i));
		}

		this->mBoard.push_back(new_row);
	}

	// Load Font
	if (!mFont.loadFromFile("fonts/arial.ttf"))
	{
		std::cout << "Error loading font!\n";
		exit(-1);
	}

	mText.setFont(mFont);

	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::Black);
	mText.setStyle(sf::Text::Bold);

	mLevelText.setFont(mFont);

	mLevelText.setCharacterSize(24);
	mLevelText.setPosition(sf::Vector2f((float)mWidth + 5.0f, 0.0f));
	mLevelText.setFillColor(sf::Color::Black);
	mLevelText.setStyle(sf::Text::Bold);

	PlayNextLevel();
}

void CGame::Render(sf::RenderWindow& window)
{
	mLevelText.setString("Level:\n" + std::to_string(mLevel));
	
	window.draw(mLevelText);

	for (auto& row : mBoard)
	{
		for (auto &col : row)
		{
			if (!col->IsHidden() && !col->IsBomb())
			{
				col->GetTile().setFillColor(sf::Color(220, 220, 220, 255));
			}

			window.draw(col->GetTile());

			if (!col->IsHidden() && col->GetNum() != 0)
			{
				mText.setString(std::to_string(col->GetNum()));
				mText.setPosition(col->GetTile().getPosition() + sf::Vector2f(6, 6));

				window.draw(mText);
			}
		}
	}
}

void CGame::PickTile(const int& row, const int& col)
{
	auto tile = mBoard[row][col];

	if (tile->IsBomb() == true)
	{
		std::cout << "GAME OVER!!!!!!!!\n";
	}
	else if (tile->GetNum() != 0 && !tile->IsBomb())
	{
		tile->SetHidden(false);
	}
	else if (tile->GetNum() == 0)
	{
		tile->PropogateFlips();
	}
}

bool CGame::HasWon()
{
	for (const auto& row : mBoard)
	{
		for (const auto& col : row)
		{
			if (col->IsHidden() && !col->IsBomb())
			{
				return false;
			}
		}
	}
	
	return true;
}

void CGame::PlayNextLevel()
{
	mLevel++;
	
	for (auto& row : mBoard)
	{
		for (auto& col : row)
		{
			col->Reset();
		}
	}

	PlaceMines();

	AssignNumbers();
}

void CGame::AssignNumbers()
{
	for (size_t row = 0; row < (mHeight / mTileHeight); row++)
	{
		for (size_t col = 0; col < (mWidth / mTileWidth); col++)
		{
			mBoard[row][col]->SetNumber(CountBombs(row, col));
		}
	}
}

int CGame::CountBombs(const int& row, const int& col)
{
	if (mBoard[row][col]->IsBomb())
	{
		return -1;
	}

	int total = 0;
	int t = -1, b = -1, l = -1, r = -1;

	auto low_bound = (mHeight / mTileHeight) - 1;
	auto right_bound = (mWidth / mTileWidth) - 1;

	if (row > 0 && col > 0 && row < low_bound && col < right_bound)
	{
		t = row - 1;
		b = row + 1;
		l = col - 1;
		r = col + 1;
	}

	if (row == 0) { t = right_bound; }

	if (col == 0) { l = right_bound; }

	if (row == low_bound) { b = 0; }

	if (col == right_bound) { r = 0; }

	(t == -1) ? t = row - 1 : 1;
	(b == -1) ? b = row + 1 : 1;
	(l == -1) ? l = col - 1 : 1;
	(r == -1) ? r = col + 1 : 1;

	// Left & Right
	
	if (col < right_bound)
	{	
		if (mBoard[row][r]->IsBomb()) { total++; }
	}

	if (col > 0)
	{
		if (mBoard[row][l]->IsBomb()) { total++; }
	}

	// Top

	if (row > 0)
	{
		if (mBoard[t][col]->IsBomb()) { total++; }

		if (col < right_bound)
			if (mBoard[t][r]->IsBomb()) { total++; }
		
		if (col > 0)
			if (mBoard[t][l]->IsBomb()) { total++; }
	}

	// Bottom

	if (row < low_bound)
	{
		if (mBoard[b][col]->IsBomb()) { total++; }

		if (col < right_bound)
			if (mBoard[b][r]->IsBomb()) { total++; }

		if (col > 0)
			if (mBoard[b][l]->IsBomb()) { total++; }
	}


	return total;
}

void CGame::PlaceMines()
{
	auto area = ((mWidth / mTileWidth) * (mHeight / mTileHeight));
	auto scalar = (double)mLevel / 10;
	int mines = scalar * area;

	auto boundX = mWidth / mTileWidth;
	auto boundY = mHeight / mTileHeight;
	
	for (size_t i = 0; i < mines; i++)
	{
		int rX = (rand() % boundX);
		int rY = (rand() % boundY);
		
		this->mBoard[rY][rX]->SetBomb(true);
		this->mBoard[rY][rX]->GetTile().setFillColor(sf::Color::Black);
	}
}

CGame::Tile::Tile(CGame* game)
	: mGame(game)
{
	mTile.setSize(sf::Vector2f(mGame->mTileWidth, mGame->mTileHeight));
	mTile.setFillColor(sf::Color(128, 128, 128, 255));
	mTile.setOutlineColor(sf::Color::Black);
	mTile.setOutlineThickness(1.0f);
}

// TILE FUNCTIONS

void CGame::Tile::Reset()
{
	mNum = -1;
	mHit = false;
	mBomb = false;
	mHidden = true;
}

void CGame::Tile::PropogateFlips()
{
	this->mHidden = false;
	this->mHit = true;

	auto row = mTile.getPosition().y / mGame->mTileHeight;
	auto col = mTile.getPosition().x / mGame->mTileWidth;

	auto low_bound = (mGame->mHeight / mGame->mTileHeight) - 1;
	auto right_bound = (mGame->mWidth / mGame->mTileWidth) - 1;

	if (row < low_bound)
	{
		auto below = mGame->mBoard[row + 1][col];

		if (below->GetNum() == 0 && !below->IsHit())
		{
			below->PropogateFlips();
		}
	}
	
	if (row > 0)
	{
		auto above = mGame->mBoard[row - 1][col];

		if (above->GetNum() == 0 && !above->IsHit())
		{
			mGame->mBoard[row - 1][col]->PropogateFlips();
		}
	}
	
	if (col < right_bound)
	{
		auto right = mGame->mBoard[row][col + 1];

		if (right->GetNum() == 0 && !right->IsHit())
		{
			right->PropogateFlips();
		}
	}
	
	if (col > 0)
	{
		auto left = mGame->mBoard[row][col - 1];

		if (left->GetNum() == 0 && !left->IsHit())
		{
			left->PropogateFlips();
		}
	}
}
