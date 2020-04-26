g++ -std=c++11 -c classes/tile.hpp
g++ -std=c++11 -c classes/tile.cpp
g++ -std=c++11 -c classes/keyentity.hpp
g++ -std=c++11 -c classes/keyentity.cpp
g++ -std=c++11 -c main.cpp
g++ -std=c++11 tile.o keyentity.o main.o -o game -lsfml-graphics -lsfml-window -lsfml-system
game.exe
