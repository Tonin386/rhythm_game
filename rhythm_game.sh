g++ -c classes/tile.hpp
g++ -c classes/tile.cpp
g++ -c classes/keyentity.hpp
g++ -c classes/keyentity.cpp
g++ -c main.cpp
g++ tile.o keyentity.o main.o -o game -lsfml-graphics -lsfml-window -lsfml-system
./game
