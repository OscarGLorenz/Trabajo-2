/******************************************************************************
* ARCHIVO :        Klondike.cpp
*
* DESCRIPCIÓN :
*       Clases principales del programa, algoritmo de resolución y navegación
*       por el mapa
* NOTA:
*       Documentación de uso de las funciones en la cabecera (include/)
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/

#include "Klondike.hpp"

#include <algorithm>
#include <limits>
#include <list>

class std::numeric_limits<int>; // Forward declaration, máximo de los enteros

Point::Point(short X, short Y) : x(X), y(Y) {}
Point::Point(short X, short Y, Direction d) : x(X), y(Y), dir(d) {}

bool Point::operator==(const Point other) const{
  return (this->x == other.x) && (this->y == other.y);
}

bool Point::operator!=(const Point other) const{
  return (this->x != other.x) || (this->y != other.y);
}

Klondike::Klondike(short klondikeMap[][SIZE]) {
  // Iterar por la matriz
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      map[i][j] = klondikeMap[i][j]; // Guardar mapa
      dist[i][j] = std::numeric_limits<int>::max(); // Poner distancia máxima
    }
  }
}

std::list<Point> Klondike::solve(Point start, std::vector<SearchResult> * v_search) {
  bool s = v_search != nullptr; // Si se proporciona una lista, dar pasos de resolución
  std::list<SearchResult> search; // Lista de pasos de resolución
  std::list<Point> steps; // Auxiliar para los pasos de resolución

  std::list<Point> path; // Camino a la salida

  std::list<Point> sol;  // Lista prinicipal del algoritmo
  sol.push_back(start);
  setDist(start, 0); // Coste 0, posición inicial

  // Mientras haya algo en la lista....
  while(!sol.empty()) {
    Point now = sol.front(); // Coger el primer elemento
    sol.pop_front(); // Eliminar el primer elemento

    if(s) steps.clear(); // Reiniciar el auxiliar en modo pasos

    // Obtener las posibles casillas a las que se puede ir
    std::list<Point> ady = adyacent(now);
    // Recorrer todas las casillas adyacentes
    for (auto it = ady.begin(); it != ady.end(); ++it){

      // Si no ha sido visitada (coste = maximo) y no esta fuera del mapa
      if (getDist(*it) == std::numeric_limits<int>::max())  {

        setDist(*it, getDist(now) + 1); // Actualizar distancia
        sol.push_back(*it); // Añadir a la lista principal

        if(s) steps.push_back(*it); // Añadir casilla en modo pasos

        // Si la casilla es una del final y esta más cerca que ninguna otra
        // hasta el momento
        if( getMap(*it) == 0) {
          path.push_front(*it); // Poner la casilla en la lista de la solución
          // Buscar solo una solución
          sol.clear(); // Se acabó la búsqueda
          break;
        }

      }
    }

    // Añadir si se esta en modo pasos esta iteración
    if(s) search.push_back(SearchResult(now, steps, getDist(now) + 1));

  }

  if (path.empty()) // Si no hay solución, terminar
  return path;

  // Recorrer el camino hacia atrás desde la meta
  for(int c = getDist(path.front()); c > 0; c--) {

    // Buscar por el mapa
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {

        /* Mirar si estamos en una casilla que pueda ser adyacente de path.front(),
           En dirección horizontal, vertical o las diagonales        */
        if (i == path.front().x || j == path.front().y || abs(path.front().x-i) == abs(path.front().y-j)) {

          // Si el coste es justo uno menor que el anterior
          if (dist[i][j] == c-1) {

            // Obtener casillas adyacentes
            std::list<Point> ady = adyacent(Point(i,j));

            // Busca si esta casilla tiene como adyacente a path.front()
            auto search = std::find(ady.begin(),ady.end(),path.front());

            // Si hemos encontrado alguna
            if (search != ady.end()) {
              // Guardar la primera y cortar
              path.push_front(Point(i,j)); //Añadir a la solución

              // COMENTAR PARA EXPLORAR EL MAPA ENTERO
              i = j = SIZE; //Pasar a la siguiente valor
              break;
              // COMENTAR PARA EXPLORAR EL MAPA ENTERO
            }

          }
        }

      }
    }
  }

  // Modo pasos, Convertir la lista en un vector para su manejo más simple
  if(s) {
    v_search->reserve(search.size());
    v_search->assign(search.begin(), search.end());
  }

  return path;
}

void Klondike::setDist(Point p, int newDist) {
  dist[p.x][p.y] = newDist;
}
int Klondike::getDist(Point p) {
  return dist[p.x][p.y];
}
short Klondike::getMap(Point p) {
  return map[p.x][p.y];
}
void Klondike::setMap(Point p, short newMap) {
  map[p.x][p.y] = newMap;
}

std::list<Point> Klondike::adyacent(Point now) {
  std::list<Point> ady; // Lista de casillas adyacentes
  short i = now.x;
  short j = now.y;
  short mov = map[i][j]; // Valor de la casilla dada
  int k; // Índice para ver si nos salimos del mapa

  // Dirección SUR
  if (i + mov <= SIZE - 1) { // No salirse del tablero
    // Mirar si nos pasamos por alguna casilla de 0 antes de llegar a la meta
    for(k = 1; k <= mov-1; k++) {
      if (map[i+k][j] == 0)
      break;
    }
    //Añade a la lista si es todo correcto
    if(k == mov && map[i+mov][j] != -1)
    ady.push_back(Point(i+mov,j,Direction::SOUTH));
  }

  // Dirección NORTE
  if (i - mov >= 0) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i-k][j] == 0)
      break;
    }
    if(k == mov && map[i-mov][j] != -1)
    ady.push_back(Point(i-mov,j,Direction::NORTH));
  }

  // Dirección ESTE
  if (j + mov <= SIZE - 1) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i][j+k] == 0)
      break;
    }
    if(k == mov && map[i][j+mov] != -1)
    ady.push_back(Point(i,j+mov,Direction::EAST));
  }

  // Dirección OESTE
  if (j - mov >= 0) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i][j-k] == 0)
      break;
    }
    if(k == mov && map[i][j-mov] != -1)
    ady.push_back(Point(i,j-mov,Direction::WEST));
  }

  // Dirección SUDESTE
  if (i + mov <= SIZE - 1 && j + mov <= SIZE - 1) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i+k][j+k] == 0)
      break;
    }
    if(k == mov && map[i+mov][j+mov] != -1)
    ady.push_back(Point(i+mov,j+mov,Direction::SOUTHEAST));
  }

  // Dirección NORESTE
  if (i - mov >= 0 && j + mov <= SIZE - 1) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i-k][j+k] == 0)
      break;
    }
    if(k == mov && map[i-mov][j+mov] != -1)
    ady.push_back(Point(i-mov,j+mov,Direction::NORTHEAST));
  }

  // Dirección SUROESTE
  if (i + mov <= SIZE - 1 && j - mov >= 0) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i+k][j-k] == 0)
      break;
    }
    if(k == mov && map[i+mov][j-mov] != -1)
    ady.push_back(Point(i+mov,j-mov,Direction::SOUTHWEST));
  }

  // Dirección NOROESTE
  if (i - mov >= 0 && j - mov >= 0) {
    for(k = 1; k <= mov-1; k++) {
      if (map[i-k][j-k] == 0)
      break;
    }
    if(k == mov && map[i-mov][j-mov] != -1)
    ady.push_back(Point(i-mov,j-mov,Direction::NORTHWEST));
  }

  return ady;
}

SearchResult::SearchResult(Point p, std::list<Point> l, int d) :
from(p), adyacent(l), distance(d){}
