#include "Klondike.hpp"

#include <algorithm>
#include <limits>
#include <list>

/*
*  Constructor
* --------------------------------------------------------
*   Genera un punto con las coordenadas x e y dados.
*   Es opcional una dirección
*/
Point::Point(short X, short Y) : x(X), y(Y) {}
Point::Point(short X, short Y, Direction d) : x(X), y(Y), dir(d) {}

/*
*  Operador ==
* --------------------------------------------------------
*   Comprueba si dos puntos son iguales
*/
bool Point::operator==(const Point other) const{
  return (this->x == other.x) && (this->y == other.y);
}

/*
*  Operador !=
* --------------------------------------------------------
*   Comprueba si dos puntos son iguales
*/
bool Point::operator!=(const Point other) const{
  return (this->x != other.x) || (this->y != other.y);
}


/*
*  Constructor
* --------------------------------------------------------
*   Genera un objeto Klondike en el que se guarda el mapa dado
*   inicializa además la matriz de distancias al máximo
*/
Klondike::Klondike(short klondikeMap[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      map[i][j] = klondikeMap[i][j];
      dist[i][j] = std::numeric_limits<int>::max();
    }
  }
}

/*
*  Función: solve
* --------------------------------------------------------
*   Resuelve el mapa, desde el punto dado, en la lista se guarda el camino a la
*   salida más corto, la matriz de distancias se verá completada
*
*   start: punto de origen para resolver el mapa
*
*   resultado: lista de puntos con el camino a la salida
*/
std::list<Point> Klondike::solve(Point start, std::vector<SearchResult> * v_search) {
  bool s = v_search != nullptr; // Si se proporciona una lista, dar pasos de resolución
  std::list<Point> steps; // Auxiliar para s true
  std::list<SearchResult> search;

  std::list<Point> path;
  std::list<Point> sol;  // Lista prinicipal del algoritmo
  sol.push_back(start);
  setDist(start, 0); // Coste 0, posición inicial

  // Mientras haya algo en la lista....
  while(!sol.empty()) {
    Point now = sol.front(); // Coger el primer elemento
    sol.pop_front(); // Eliminar el primer elemento

    if(s) steps.clear();

    // Obtener las posibles casillas a las que se puede ir
    std::list<Point> ady = adyacent(now);

    // Recorrer todas las casillas adyacentes
    for (std::list<Point>::const_iterator it = ady.begin(); it != ady.end(); ++it){

      // Si no ha sido visitada (coste = maximo) y no esta fuera del mapa
      if (getDist(*it) == std::numeric_limits<int>::max())  {

        setDist(*it, getDist(now) + 1); // Actualizar coste (distancia)
        sol.push_back(*it); // Añadir a la lista principal

        if(s) steps.push_back(*it); // Añadir casilla en modo pasos

        // Si la casilla es una del final y esta más cerca que ninguna otra
        // hasta el momento
        if( getMap(*it) == 0) {
          path.push_front(*it); // Poner la casilla en la lista de la solución
          sol.clear(); // Se acabó la búsqueda
          break;
        }

      }
    }

    // Añadir si se esta en modo pasos esta iteración
    if(s) search.push_back(SearchResult(now, steps, getDist(now) + 1, false));

  }

  if (path.empty()) // Si no hay solución, terminar
  return path;

  // Recorrer el camino hacia atrás desde la meta
  for(int c = getDist(path.front()); c > 0; c--) {

    // Buscar por el mapa
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        if(s) steps.clear();

        /* Mirar si estamos en una casilla que pueda ser adyacente de path.front(),
           En dirección horizontal, vertical o las diagonales        */
        if (i == path.front().x || j == path.front().y || abs(path.front().x-i) == abs(path.front().y-j)) {

          // Si el coste es justo uno menor que el anterior
          if (dist[i][j] == c-1) {

            // Obtener casillas adyacentes
            std::list<Point> ady = adyacent(Point(i,j));

            // Buscar si coincide esta casilla con path.front()
            for (std::list<Point>::const_iterator it = ady.begin(); it != ady.end(); ++it){

              if(s) steps.push_back(*it); // Añadir casilla en modo pasos

              if(path.front() == *it) { // Casilla encontrada
                path.push_front(Point(i,j)); //Añadir a la solución
                i = j = SIZE; //Pasar a la siguiente valor
                break;
              }
            }
          }
        }

        // Añadir si se esta en modo pasos esta iteración
        if(s) search.push_back(SearchResult(path.front(), steps, c, true));
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

/*
*  Función: setDist
* --------------------------------------------------------
*   Actualiza la distancia de una casilla
*
*   p: punto que corresponde a la casilla en cuestión
*   newDist: nueva distancia
*/
void Klondike::setDist(Point p, int newDist) {
  dist[p.x][p.y] = newDist;
}

/*
*  Función: getDist
* --------------------------------------------------------
*   Obtiene la distancia de una casilla
*
*   p: punto que corresponde a la casilla en cuestión
*
*  resultado: distancia de la casilla en cuestión
*/
int Klondike::getDist(Point p) {
  return dist[p.x][p.y];
}

/*
*  Función: getMap
* --------------------------------------------------------
*   Obtiene el valor de una casilla
*
*   p: punto que corresponde a la casilla en cuestión
*
*  resultado: valor de la casilla en cuestión
*/
short Klondike::getMap(Point p) {
  return map[p.x][p.y];
}


/*
 *  Función: setMap
 * --------------------------------------------------------
 *   Actualiza los movimientos de una casilla
 *
 *   p: punto que corresponde a la casilla en cuestión
 *   newMap: nuevos movimientos
 */
void Klondike::setMap(Point p, short newMap) {
  map[p.x][p.y] = newMap;
}


/*
*  Función: adyacent
* --------------------------------------------------------
*   Calcula las casillas accesibles desde una dada
*
*   now: casilla desde la cual se desea calcular a cuales otras se puede ir
*
*   resultado: lista de puntos con las casillas posibles desde la dada
*/
std::list<Point> Klondike::adyacent(Point now) {
  std::list<Point> ady; // Lista de casillas adyacentes
  short i = now.x;
  short j = now.y;
  short mov = map[i][j]; // Valor de la casilla dada
  bool flag; // Auxiliar para detectar que nos salimos del mapa

  // Dirección SUR
  if (i + mov <= SIZE - 1) { // No salirse del tablero
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i+k][j] == 0)
      flag = false;
    }
    if(flag && map[i+mov][j] != -1)
    ady.push_back(Point(i+mov,j,Direction::SOUTH));
  }

  // Dirección NORTE
  if (i - mov >= 0) { // No salirse del tablero
    flag = true;
    // Mirar si nos pasamos por alguna casilla de 0 antes de llegar a la meta
    for(int k = 1; k <= mov-1; k++) {
      if (map[i-k][j] == 0)
      flag = false;
    }
    //Añade a la lista si es todo correcto
    if(flag && map[i-mov][j] != -1)
    ady.push_back(Point(i-mov,j,Direction::NORTH));
  }

  // Dirección ESTE
  if (j + mov <= SIZE - 1) {
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i][j+k] == 0)
      flag = false;
    }
    if(flag && map[i][j+mov] != -1)
    ady.push_back(Point(i,j+mov,Direction::EAST));
  }

  // Dirección OESTE
  if (j - mov >= 0) {
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i][j-k] == 0)
      flag = false;
    }
    if(flag && map[i][j-mov] != -1)
    ady.push_back(Point(i,j-mov,Direction::WEST));
  }

  // Dirección SUDESTE
  if (i + mov <= SIZE - 1 && j + mov <= SIZE - 1) {
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i+k][j+k] == 0)
      flag = false;
    }
    if(flag && map[i+mov][j+mov] != -1)
    ady.push_back(Point(i+mov,j+mov,Direction::SOUTHEAST));
  }

  // Dirección NORESTE
  if (i - mov >= 0 && j + mov <= SIZE - 1) {
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i-k][j+k] == 0)
      flag = false;
    }
    if(flag && map[i-mov][j+mov] != -1)
    ady.push_back(Point(i-mov,j+mov,Direction::NORTHEAST));
  }

  // Dirección SUROESTE
  if (i + mov <= SIZE - 1 && j - mov >= 0) {
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i+k][j-k] == 0)
      flag = false;
    }
    if(flag && map[i+mov][j-mov] != -1)
    ady.push_back(Point(i+mov,j-mov,Direction::SOUTHWEST));
  }

  // Dirección NOROESTE
  if (i - mov >= 0 && j - mov >= 0) {
    flag = true;
    for(int k = 1; k <= mov-1; k++) {
      if (map[i-k][j-k] == 0)
      flag = false;
    }
    if(flag && map[i-mov][j-mov] != -1)
    ady.push_back(Point(i-mov,j-mov,Direction::NORTHWEST));
  }

  return ady;
}

SearchResult::SearchResult(Point p, std::list<Point> l, int d, bool ph) :
from(p), adyacent(l), distance(d), phase(ph){}
