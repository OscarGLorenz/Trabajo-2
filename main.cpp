#include "Mapa.h"

#include "Klondike.hpp"

#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>

int main() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(1,9);

  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (MAPA[i][j] > 0)
        MAPA[i][j] = distribution(generator);
    }
  }


  Klondike lab(MAPA);
  std::list<Point> sol = lab.solve(Point((SIZE-1)/2,(SIZE-1)/2));

  std::cout << "EJES" << std::endl;
  std::cout << ".--y" << std::endl;
  std::cout << "|" << std::endl;
  std::cout << "x" << std::endl << std::endl;


  std::cout << "MAPA" << std::endl;
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      std::cout << std::setw(2) << MAPA[i][j];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

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
}
