/******************************************************************************
* ARCHIVO :        Klondike.hpp
*
* DESCRIPCIÓN :
*       Clases principales del programa, algoritmo de resolución y navegación
*       por el mapa
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/

#ifndef KLONDIKE_HPP
#define KLONDIKE_HPP

#include <list>
#include <limits>

// Tamaño del Laberinto
#define SIZE 23

/*
 *  Clase: Point
 * --------------------------------------------------------
 *   Representa un punto en el mapa, acorde con los índices del mapa.
 *
 */
class Point {
public:
  /*
   *  Constructor
   * --------------------------------------------------------
   *   Genera un punto con las coordenadas x e y dados
   */
  Point(short X, short Y);

  /*
   *  Operador ==
   * --------------------------------------------------------
   *   Comprueba si dos puntos son iguales
   */
  bool operator==(Point other);

  short x; // Primer índice
  short y; // Segundo índice
};

/*
 *  Clase: Klondike
 * --------------------------------------------------------
 *   Clase principal, contiene al mapa y tiene funciones para
 *   resolver el mapa, encontrar el camino más corto...
 *
 */
class Klondike {
public:
  /*
   *  Constructor
   * --------------------------------------------------------
   *   Genera un objeto Klondike en el que se guarda el mapa dado
   *   inicializa además la matriz de distancias al máximo
   */
  Klondike(short klondikeMap[][SIZE]);

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
  std::list<Point> solve(Point start);

  /*
   *  Función: adyacent
   * --------------------------------------------------------
   *   Calcula las casillas accesibles desde una dada
   *
   *   now: casilla desde la cual se desea calcular a cuales otras se puede ir
   *
   *   resultado: lista de puntos con las casillas posibles desde la dada
   */
  std::list<Point> adyacent(Point now);

  /*
   *  Función: setDist
   * --------------------------------------------------------
   *   Actualiza la distancia de una casilla
   *
   *   p: punto que corresponde a la casilla en cuestión
   *   newDist: nueva distancia
   */
  void setDist(Point p, int newDist);

  /*
   *  Función: getDist
   * --------------------------------------------------------
   *   Obtiene la distancia de una casilla
   *
   *   p: punto que corresponde a la casilla en cuestión
   *
   *  resultado: distancia de la casilla en cuestión
   */
  int getDist(Point p);

  /*
   *  Función: getMap
   * --------------------------------------------------------
   *   Obtiene el valor de una casilla
   *
   *   p: punto que corresponde a la casilla en cuestión
   *
   *  resultado: valor de la casilla en cuestión
   */
  short getMap(Point p);

private:

  int dist[SIZE][SIZE]; // Distancias a cada casilla desde el principio
  short map[SIZE][SIZE]; // Movimientos de cada casilla
};

#endif
