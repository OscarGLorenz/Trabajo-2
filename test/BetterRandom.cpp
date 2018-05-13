#include <iomanip>
#include <iostream>
#include "Klondike.hpp"
#include "MapGenerator.hpp"
#include "Mapa.h"

Klondike lab(MAPA);
MapGenerator map(&lab);

int main() {
  map.random(10);
  map.createMap();

  std::list<Point> sol = lab.solve(Point(11,11));

  std::cout << "EJES" << std::endl;
  std::cout << ".--y" << std::endl;
  std::cout << "|" << std::endl;
  std::cout << "x" << std::endl << std::endl;


  std::cout << "MAPA" << std::endl;
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (lab.getMap(Point(i,j)) == -1)
      std::cout << std::setw(2) << '-';
      else
      std::cout << std::setw(2) << lab.getMap(Point(i,j));
    }
    std::cout << std::endl;
  }

  std::cout << "DISTANCIAS DESDE EL CENTRO" << std::endl;
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (lab.getDist(Point(i,j)) == std::numeric_limits<int>::max())
      std::cout << std::setw(2) << '-';
      else
      std::cout << std::setw(2) << lab.getDist(Point(i,j));
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << "CAMINO A UNA SALIDA ÓPTIMA" << std::endl;
  for (std::list<Point>::const_iterator it = sol.begin(); it != sol.end(); ++it){
    std::cout << it->x << " " << it->y << std::endl;
  }
  std::cout << std::endl;

  std::cout << "DISTANCIAS A LAS SALIDAS" << std::endl;
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (lab.getMap(Point(i,j)) != 0)
      std::cout << std::setw(2) << ' ';
      else if (lab.getDist(Point(i,j)) == std::numeric_limits<int>::max())
      std::cout << std::setw(2) << '-';
      else
      std::cout << std::setw(2) << lab.getDist(Point(i,j));
    }
    std::cout << std::endl;
  }

  return 0;
}
