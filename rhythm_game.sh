g++ -c classes/tile.hpp
g++ -c classes/tile.cpp
g++ -c main.cpp
g++ tile.o main.o -o game -lsfml-graphics -lsfml-window -lsfml-system
./game
