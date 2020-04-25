#include "classes/tile.hpp"

#include <iostream>
#include <map>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ROWS 4
#define WIDTH 400
#define HEIGHT 1000
#define MAX_TILES 5

using namespace sf;
using namespace std;

//debug functions
void showKeysPressed(map<char, bool> keys);

Tile* createTile(Tile* previousTile = nullptr)
{
	return new Tile(MAX_ROWS, previousTile);
}

void updateTiles(vector<Tile*> tiles, double timeSinceLastTick)
{
	for(int i = 0; i < tiles.size(); i++)
	{
		tiles[i]->timePassed(timeSinceLastTick);
		if(tiles[i]->getTimeToHit() < -HEIGHT/MAX_TILES) 
		{
			if(i+1 < tiles.size()) tiles[i+1]->setPreviousNull();
			tiles.erase(tiles.begin()+i);
			i--;
		}

	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	Clock tickingClock;
	double lastTick = tickingClock.getElapsedTime().asMilliseconds();
	double timeSinceLastTick = 0;

	map<char, bool> keys;
	keys['D'] = false;
	keys['F'] = false;
	keys['J'] = false;
	keys['K'] = false;

	vector<Tile*> tiles;
	tiles.push_back(createTile());

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Feel the beat");

	while (window.isOpen())
	{
		timeSinceLastTick = tickingClock.getElapsedTime().asMilliseconds() - lastTick;
		lastTick = tickingClock.getElapsedTime().asMilliseconds();

		if(timeSinceLastTick >= 1)
		{
			updateTiles(tiles, timeSinceLastTick);
		}

		while(tiles.size() < 1000)
		{
			tiles.push_back(createTile(tiles.back()));
		}

		Event event;
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
				case Event::Closed:
					window.close();
					break;

				case Event::KeyPressed:
					if(event.key.code == Keyboard::D)
					{
						keys['D'] = true;
					}
					else if(event.key.code == Keyboard::F)
					{
						keys['F'] = true;
					}
					else if(event.key.code == Keyboard::J)
					{
						keys['J'] = true;
					}
					else if(event.key.code == Keyboard::K)
					{	
						keys['K'] = true;
					}
					break;

				case Event::KeyReleased:
					if(event.key.code == Keyboard::D)
					{
						keys['D'] = false;
					}
					else if(event.key.code == Keyboard::F)
					{
						keys['F'] = false;
					}
					else if(event.key.code == Keyboard::J)
					{
						keys['J'] = false;
					}
					else if(event.key.code == Keyboard::K)
					{	
						keys['K'] = false;
					}
					break;

			}

		}


		VertexArray tileQuads(Quads, tiles.size()*4);
		VertexArray lineQuads(Quads, tiles.size()*4);

		for(int i = 0; i < tiles.size(); i++)
		{
			Tile *t = tiles[i];
			int t_row = t->getRow();
			double t_timeToHit = t->getTimeToHit();

			tileQuads[i * 4    ].position = Vector2f(t_row * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit);
			tileQuads[i * 4 + 1].position = Vector2f((t_row+1) * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit);
			tileQuads[i * 4 + 2].position = Vector2f((t_row+1) * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);
			tileQuads[i * 4 + 3].position = Vector2f(t_row * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);

			tileQuads[i * 4    ].color = Color(255,255,255);
			tileQuads[i * 4 + 1].color = Color(255,255,255);
			tileQuads[i * 4 + 2].color = Color(255,255,255);
			tileQuads[i * 4 + 3].color = Color(255,255,255);

			lineQuads[i * 4    ].position = Vector2f(0, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);
			lineQuads[i * 4 + 1].position = Vector2f(WIDTH, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);
			lineQuads[i * 4 + 2].position = Vector2f(WIDTH, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES + 2);
			lineQuads[i * 4 + 3].position = Vector2f(0, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES + 2);

			lineQuads[i * 4    ].color = Color(0,255,0);
			lineQuads[i * 4 + 1].color = Color(0,255,0);
			lineQuads[i * 4 + 2].color = Color(0,255,0);
			lineQuads[i * 4 + 3].color = Color(0,255,0);
		}

		//showKeysPressed(keys);
		window.clear(Color(2,5,100));
		//	window.draw(shape);
		window.draw(tileQuads);
		window.draw(lineQuads);
		window.display();
	}

	return 0;
}

void showKeysPressed(map<char, bool> keys)
{
	cout << "D: " << keys['D'] << endl;
	cout << "F: " << keys['F'] << endl;
	cout << "J: " << keys['J'] << endl;
	cout << "K: " << keys['K'] << endl;
}