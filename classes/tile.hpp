#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

class Tile {

public:
	Tile(int maxRows, Tile* previousTile);

	virtual double getTimeToHit() const;
	virtual int getRow() const;
	virtual bool getTapped() const;
	virtual sf::Color getColor() const;
	virtual void timePassed(double t);
	virtual void setPreviousNull();
	virtual void destroy();

	virtual ~Tile();

protected:
private:
	int x_maxRows;
	int x_row;
	double x_timeToHit;
	Tile *x_previousTile;
	bool x_tapped;
	sf::Color x_color;
};

#endif