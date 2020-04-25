#include "tile.hpp"

#include <stdlib.h>
#include <iostream>

using namespace sf;
using namespace std;

Tile::Tile(int maxRows, Tile* previousTile) : x_maxRows(maxRows), x_previousTile(previousTile) 
{
	x_tapped = false;
	x_row = rand() % x_maxRows;
	if(previousTile == nullptr) x_timeToHit = 300;
	else 
	{
		x_timeToHit = x_previousTile->getTimeToHit() + 1000/5;
		while(x_row == previousTile->getRow())
		{
			x_row = rand() % x_maxRows;
		}
	}
}

double Tile::getTimeToHit() const
{
	return x_timeToHit;
}

int Tile::getRow() const
{
	return x_row;
}

bool Tile::getTapped() const
{
	return x_tapped;
}

void Tile::setPreviousNull()
{
	x_previousTile = nullptr;
}

void Tile::timePassed(double t)
{
	x_timeToHit -= t;
}

void Tile::destroy()
{
	x_tapped = true;
	x_timeToHit = -1000;
}

Tile::~Tile()
{
	if(x_previousTile != nullptr) delete x_previousTile;
	delete this;
}