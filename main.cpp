#include "Mapa.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>

// Clase punto, representa una posición en el mapa por sus índices
class Point {
public:
  Point(short X, short Y) : x(X), y(Y) {}
  short x;
  short y;
  bool operator==(Point other) {
    return (this->x == other.x) && (this->y == other.y);
  }
} ;

// Clase principal
class Klondike {
public:
  // Guarda el mapa e inicializa los costes al máximo posible
  Klondike(short klondikeMap[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        map[i][j] = klondikeMap[i][j];
        cost[i][j] = std::numeric_limits<int>::max();
      }
    }
  }

  // Resuelve el mapa, desde el punto dado, en la lista se guarda el camino a la
  // salida más corto
  void solve(std::list<Point>& path, Point start) {
    std::list<Point> sol;  // Lista prinicipal del algoritmo
    sol.push_back(start);
    setCost(start, 0); // Coste 0, posición inicial
    int minD = std::numeric_limits<int>::max(); // Distancia hasta la salida

    // Mientras haya algo en la lista....
    while(!sol.empty()) {
      Point s = sol.front(); // Coger el primer elemento
      sol.pop_front(); // Eliminar el primer elemento

      // Obtener las posibles casillas a las que se puede ir
      std::list<Point> ady = adyacente(s);
      // Recorrer todas las casillas adyacentes
      for (std::list<Point>::const_iterator it = ady.begin(); it != ady.end(); ++it){
        // Si no ha sido visitada (coste = maximo) y no esta fuera del mapa
        if (getCost(*it) == std::numeric_limits<int>::max() && getMap(*it) != -1)  {
          setCost(*it, getCost(s) + 1); // Actualizar coste (distancia)
          sol.push_back(*it); // Añadir a la lista principal
          // Si la casilla es una del final y esta más cerca que ninguna otra
          // hasta el momento
          if( getMap(*it) == 0 && getCost(*it) < minD) {
            minD = getCost(*it); // Actualizar coste
            path.clear(); // Borrar solucion anterior
            path.push_front(*it); // Poner la casilla en la lista de la solución
          }
        }
      }
    }

    // Recorrer el camino hacia atrás desde la meta
    for(int c = minD, d = minD; c > 0; c--) {
      // Buscar por el mapa
      for (unsigned int i = 0; i < SIZE && c == d; i++) {
        for (unsigned int j = 0; j < SIZE && c == d; j++) {
          // Si el coste es justo uno menor que el anterior
          if (cost[i][j] == d-1) {
            // Obtener casillas adyacentes
            std::list<Point> ady = adyacente(Point(i,j));
            // Buscar si alguna de las casillas adyacentes lleva a esta
            std::list<Point>::iterator it = std::find(ady.begin(), ady.end(), path.front());
            if (it != ady.end()) { // Si hemos encontrado algo
              path.push_front(Point(i,j)); //Añadir a la solución
              d--;
            }
          }
        }
      }
    }
  }

  // Actualiza el coste de una casilla
  void setCost(Point p, int newCost) {
    cost[p.x][p.y] = newCost;
  }

  // Obtiene el coste de una casilla
  int getCost(Point p) {
    return cost[p.x][p.y];
  }

  // Obtiene los movimientos de una casilla
  int getMap(Point p) {
    return map[p.x][p.y];
  }

private:
  // Devuelve las casillas adyacentes a un punto
  std::list<Point> adyacente(Point now) {
    std::list<Point> ady;
    short i = now.x;
    short j = now.y;
    short mov = map[i][j];
    bool flag;

    // Dirección SUR
    if (i + mov <= SIZE - 1) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i+k][j] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i+mov,j));
    }

    // Dirección NORTE
    if (i - mov >= 0) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i-k][j] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i-mov,j));
    }

    // Dirección ESTE
    if (j + mov <= SIZE - 1) { // No salirse del tablero
      flag = true;
      // Mirar si nos pasamos por alguna casilla de 0 antes de llegar a la meta
      for(int k = 1; k <= mov-1; k++) {
        if (map[i][j+k] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i,j+mov)); //Añade a la lista si es todo correcto
    }

    // Dirección OESTE
    if (j - mov >= 0) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i][j-k] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i,j-mov));
    }

    // Dirección SUDESTE
    if (i + mov <= SIZE - 1 && j + mov <= SIZE - 1) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i+k][j+k] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i+mov,j+mov));
    }

    // Dirección NORESTE
    if (i - mov >= 0 && j + mov <= SIZE - 1) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i-k][j+k] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i-mov,j+mov));
    }

    // Dirección SUROESTE
    if (i + mov <= SIZE - 1 && j - mov >= 0) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i+k][j-k] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i+mov,j-mov));
    }

    // Dirección NOROESTE
    if (i - mov >= 0 && j - mov >= 0) { // No salirse del tablero
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i-k][j-k] == 0)
          flag = false;
      }
      if(flag)
        ady.push_back(Point(i-mov,j-mov));
    }

    return ady;
  }

  int cost[SIZE][SIZE]; // Distancias a cada casilla desde el principio
  short map[SIZE][SIZE]; // Movimientos de cada casilla
};


int main() {
  Klondike lab(MAPA);
  std::list<Point> sol;
  lab.solve(sol, Point((SIZE-1)/2,(SIZE-1)/2));

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
      if (lab.getCost(Point(i,j)) == std::numeric_limits<int>::max())
        std::cout << std::setw(2) << '-';
      else
        std::cout << std::setw(2) << lab.getCost(Point(i,j));
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
      else if (lab.getCost(Point(i,j)) == std::numeric_limits<int>::max())
        std::cout << std::setw(2) << '-';
      else
        std::cout << std::setw(2) << lab.getCost(Point(i,j));
    }
    std::cout << std::endl;
  }
}
