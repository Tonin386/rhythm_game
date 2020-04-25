#include "classes/tile.hpp"
#include "classes/keyentity.hpp"

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

vector<KeyEntity> initKeysHistory()
{
	vector<KeyEntity> v;
	KeyEntity d('D');
	KeyEntity f('F');
	KeyEntity j('J');
	KeyEntity k('K');

	v.push_back(d);
	v.push_back(f);
	v.push_back(j);
	v.push_back(k);

	return v;
}

Tile* createTile(Tile* previousTile = nullptr)
{
	return new Tile(MAX_ROWS, previousTile);
}

void updateTiles(vector<Tile*> &tiles)
{
	for(int i = 0; i < tiles.size(); i++)
	{
		tiles[i]->timePassed(3);

		if(tiles[i]->getTimeToHit() < -HEIGHT/MAX_TILES) 
		{
			if(i+1 < tiles.size()) tiles[i+1]->setPreviousNull();
			if(!tiles[i]->getTapped()) cout << "Player lost!" << endl;
			tiles.erase(tiles.begin()+i);
			i--;
		}


	}
}

void checkingHits(vector<KeyEntity> k, vector<Tile*> t, double time)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < t.size(); j++)
		{
			if(k[i].getPressTime() > t[j]->getTimeToHit() - HEIGHT/MAX_TILES + time && t[j]->getTimeToHit() + time > k[i].getReleaseTime() && i == t[j]->getRow())
			{
				t[j]->destroy();
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	Clock processClock;
	Clock tickClock;

	long int tickCount = 0;

	map<char, bool> keys;
	keys['D'] = false;
	keys['F'] = false;
	keys['J'] = false;
	keys['K'] = false;

	vector<KeyEntity> keysHistory;
	keysHistory = initKeysHistory();

	vector<Tile*> tiles;
	tiles.push_back(createTile());

	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Feel the beat");

	while (window.isOpen())
	{
		if(tickClock.getElapsedTime().asMilliseconds() >= 4)
		{
			tickClock.restart();
			tickCount++;
			checkingHits(keysHistory, tiles, tickCount);
			updateTiles(tiles);
		}

		while(tiles.size() < MAX_TILES+2)
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
					if(event.key.code == Keyboard::D && keys['D'] == false)
					{
						keys['D'] = true;
						keysHistory[0].setPressTime(tickCount);
						cout << "D Pressed at " << tickCount << endl; 

					}
					else if(event.key.code == Keyboard::F && keys['F'] == false)
					{
						keys['F'] = true;
						keysHistory[1].setPressTime(tickCount);
						cout << "F Pressed at " << tickCount << endl; 
					}
					else if(event.key.code == Keyboard::J && keys['J'] == false)
					{
						keys['J'] = true;
						keysHistory[2].setPressTime(tickCount);
						cout << "J Pressed at " << tickCount << endl; 
					}
					else if(event.key.code == Keyboard::K && keys['K'] == false)
					{	
						keys['K'] = true;
						keysHistory[3].setPressTime(tickCount);
						cout << "K Pressed at " << tickCount << endl; 
					}
					break;

				case Event::KeyReleased:
					if(event.key.code == Keyboard::D)
					{
						keys['D'] = false;
						keysHistory[0].setReleaseTime(tickCount);
						cout << "D released at " << tickCount << endl;
					}
					else if(event.key.code == Keyboard::F)
					{
						keys['F'] = false;
						keysHistory[1].setReleaseTime(tickCount);
						cout << "F released at " << tickCount << endl;
					}
					else if(event.key.code == Keyboard::J)
					{
						keys['J'] = false;
						keysHistory[2].setReleaseTime(tickCount);
						cout << "J released at " << tickCount << endl;
					}
					else if(event.key.code == Keyboard::K)
					{	
						keys['K'] = false;
						keysHistory[3].setReleaseTime(tickCount);
						cout << "K released at " << tickCount << endl;
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