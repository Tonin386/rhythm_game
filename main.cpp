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
#define TOTAL_TILES 100

using namespace sf;
using namespace std;

//display functions
VertexArray createDisplayedLines(vector<Tile*> tiles);
VertexArray createDisplayedTiles(vector<Tile*> tiles);
VertexArray createBackgroundTile(VertexArray bg);

//motor functions
vector<KeyEntity> initKeysHistory();
Tile* createTile(Tile* previousTile = nullptr);
bool updateTiles(vector<Tile*> &tiles);
bool checkingHits(vector<KeyEntity> k, vector<Tile*> t, double time);

//debug functions
void showKeysPressed(map<char, bool> keys);


int main(int argc, char const *argv[])
{
	srand(time(NULL));

	Clock processClock;
	Clock tickClock;
	Clock bgClock;

	long int tickCount = 0;
	int tileCount = 1;

	double score = 0;
	double combo = 0;

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
	VertexArray bg(Quads, 4);

	while (window.isOpen())
	{
		if(tickClock.getElapsedTime().asMilliseconds() >= 4)
		{
			tickClock.restart();
			tickCount++;
			bool tapped = checkingHits(keysHistory, tiles, tickCount);
			bool success = updateTiles(tiles);
			if(tapped)
			{
				combo++;
				score += 0.5*combo + 10;
				cout << "Score: " << score << endl << "Combo: " << combo << endl;
			}
			
			if(!success)
			{
				combo = 0;
				score -= 5;
				cout << "Score: " << score << endl << "Combo: " << combo << endl;
			}
		}

		if(bgClock.getElapsedTime().asMilliseconds() > 20)
		{
			bgClock.restart();
			bg = createBackgroundTile(bg);
		}

		while(tiles.size() < MAX_TILES+2 && tileCount < TOTAL_TILES)
		{
			tiles.push_back(createTile(tiles.back()));
			tileCount++;
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
					// cout << "D Pressed at " << tickCount << endl; 

				}
				else if(event.key.code == Keyboard::F && keys['F'] == false)
				{
					keys['F'] = true;
					keysHistory[1].setPressTime(tickCount);
					// cout << "F Pressed at " << tickCount << endl; 
				}
				else if(event.key.code == Keyboard::J && keys['J'] == false)
				{
					keys['J'] = true;
					keysHistory[2].setPressTime(tickCount);
					// cout << "J Pressed at " << tickCount << endl; 
				}
				else if(event.key.code == Keyboard::K && keys['K'] == false)
				{	
					keys['K'] = true;
					keysHistory[3].setPressTime(tickCount);
					// cout << "K Pressed at " << tickCount << endl; 
				}
				break;

				case Event::KeyReleased:
				if(event.key.code == Keyboard::D)
				{
					keys['D'] = false;
					keysHistory[0].setReleaseTime(tickCount);
					// cout << "D released at " << tickCount << endl;
				}
				else if(event.key.code == Keyboard::F)
				{
					keys['F'] = false;
					keysHistory[1].setReleaseTime(tickCount);
					// cout << "F released at " << tickCount << endl;
				}
				else if(event.key.code == Keyboard::J)
				{
					keys['J'] = false;
					keysHistory[2].setReleaseTime(tickCount);
					// cout << "J released at " << tickCount << endl;
				}
				else if(event.key.code == Keyboard::K)
				{	
					keys['K'] = false;
					keysHistory[3].setReleaseTime(tickCount);
					// cout << "K released at " << tickCount << endl;
				}
				break;

			}

		}

		//showKeysPressed(keys);
		window.clear();
		window.draw(bg);
		window.draw(createDisplayedTiles(tiles));
		window.draw(createDisplayedLines(tiles));
		window.display();

		if(tiles.size() == 0)
		{
			Clock stop;
			while(stop.getElapsedTime().asSeconds() < 3) {}
			window.close();
		}
	}

	cout << "Thanks for playing!" << endl;

	return 0;
}

//display functions
VertexArray createDisplayedLines(vector<Tile*> tiles)
{
	VertexArray lineQuads(Quads, tiles.size()*4);

	for(int i = 0; i < tiles.size(); i++)
	{
		Tile *t = tiles[i];
		int t_row = t->getRow();
		double t_timeToHit = t->getTimeToHit();

		lineQuads[i * 4    ].position = Vector2f(0, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);
		lineQuads[i * 4 + 1].position = Vector2f(WIDTH, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);
		lineQuads[i * 4 + 2].position = Vector2f(WIDTH, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES + 2);
		lineQuads[i * 4 + 3].position = Vector2f(0, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES + 2);

		lineQuads[i * 4    ].color = Color(0,255,0);
		lineQuads[i * 4 + 1].color = Color(0,255,0);
		lineQuads[i * 4 + 2].color = Color(0,255,0);
		lineQuads[i * 4 + 3].color = Color(0,255,0);
	}

	return lineQuads;
}
VertexArray createDisplayedTiles(vector<Tile*> tiles)
{
	VertexArray tileQuads(Quads, tiles.size()*4);

	for(int i = 0; i < tiles.size(); i++)
	{
		Tile *t = tiles[i];
		int t_row = t->getRow();
		double t_timeToHit = t->getTimeToHit();

		tileQuads[i * 4    ].position = Vector2f(t_row * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit);
		tileQuads[i * 4 + 1].position = Vector2f((t_row+1) * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit);
		tileQuads[i * 4 + 2].position = Vector2f((t_row+1) * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);
		tileQuads[i * 4 + 3].position = Vector2f(t_row * WIDTH/MAX_ROWS, HEIGHT - t_timeToHit + HEIGHT/MAX_TILES);

		tileQuads[i * 4    ].color = t->getColor();
		tileQuads[i * 4 + 1].color = t->getColor();
		tileQuads[i * 4 + 2].color = t->getColor();
		tileQuads[i * 4 + 3].color = t->getColor();
	}

	return tileQuads;
}

VertexArray createBackgroundTile(VertexArray bg)
{
	for(int i = 0; i < 4; i++)
	{
		bg[i].color.r += rand() % 10 - 5;
		bg[i].color.g += rand() % 10 - 5;
		bg[i].color.b += rand() % 10 - 5;
		if(bg[i].color.r < 200) bg[i].color.r = 202;
		if(bg[i].color.r > 255) bg[i].color.r = 253;
		if(bg[i].color.g < 200) bg[i].color.g = 202;
		if(bg[i].color.g > 255) bg[i].color.g = 253;
		if(bg[i].color.b < 200) bg[i].color.b = 202;
		if(bg[i].color.b > 255) bg[i].color.b = 253;
	}

	bg[0].position = Vector2f(0,0);
	bg[1].position = Vector2f(WIDTH,0);
	bg[2].position = Vector2f(WIDTH,HEIGHT);
	bg[3].position = Vector2f(0, HEIGHT);

	return bg;
}

//motor functions
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

Tile* createTile(Tile* previousTile)
{
	return new Tile(MAX_ROWS, previousTile);
}

bool updateTiles(vector<Tile*> &tiles)
{
	bool success = true;

	for(int i = 0; i < tiles.size(); i++)
	{
		tiles[i]->timePassed(3);

		if(tiles[i]->getTimeToHit() < -HEIGHT/MAX_TILES) 
		{
			if(i+1 < tiles.size()) tiles[i+1]->setPreviousNull();
			if(!tiles[i]->getTapped()) success = false;
			tiles.erase(tiles.begin()+i);
			i--;
		}
	}

	return success;
}

bool checkingHits(vector<KeyEntity> k, vector<Tile*> t, double time)
{
	bool tapped = false;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < t.size(); j++)
		{
			if(k[i].getPressTime() > t[j]->getTimeToHit() - HEIGHT/MAX_TILES + time && t[j]->getTimeToHit() + time > k[i].getReleaseTime() && i == t[j]->getRow())
			{
				t[j]->destroy();
				tapped = true;
			}
		}
	}

	return tapped;
}

//debug functions
void showKeysPressed(map<char, bool> keys)
{
	cout << "D: " << keys['D'] << endl;
	cout << "F: " << keys['F'] << endl;
	cout << "J: " << keys['J'] << endl;
	cout << "K: " << keys['K'] << endl;
}