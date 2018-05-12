#include <iomanip>
#include <iostream>
#include "../include/Klondike.hpp"

#include <chrono>
#include <random>
#include <list>
#include <algorithm>

short MAPA[23][23]=
{   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 4, 7, 7, 0, 0, 0, -1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1, 0, 0, 5, 4, 4, 8, 3, 3, 4, 6, 3, 0, 0, -1,-1,-1,-1,-1},
{-1,-1,-1,-1, 0, 1, 4, 5, 1, 1, 1, 4, 5, 1, 7, 1, 3, 5, 0, -1,-1,-1,-1},
{-1,-1,-1, 0, 4, 9, 4, 9, 6, 7, 5, 5, 5, 8, 7, 6, 6, 8, 5, 0, -1,-1,-1},
{-1,-1, 0, 3, 7, 2, 9, 8, 3, 5, 6, 7, 3, 9, 1, 8, 7, 5, 8, 5, 0, -1,-1},
{-1,-1, 0, 1, 4, 7, 8, 4, 2, 9, 2, 7, 1, 1, 8, 2, 2, 7, 6, 3, 0, -1,-1},
{-1, 0, 7, 2, 1, 8, 5, 5, 3, 1, 1, 3, 1, 3, 3, 4, 2, 8, 6, 1, 3, 0, -1},
{-1, 0, 4, 2, 6, 7, 2, 5, 2, 4, 2, 2, 5, 4, 3, 2, 8, 1, 7, 7, 3, 0, -1},
{-1, 0, 4, 1, 6, 5, 1, 1, 1, 9, 1, 4, 3, 4, 4, 3, 1, 9, 8, 2, 7, 0, -1},
{ 0, 4, 3, 5, 2, 3, 2, 2, 3, 2, 4, 2, 5, 3, 5, 1, 1, 3, 5, 5, 3, 7, 0},
{ 0, 2, 7, 1, 5, 1, 1, 3, 1, 5, 3, 3, 2, 4, 2, 3, 7, 7, 5, 4, 2, 7, 0},
{ 0, 2, 5, 2, 2, 6, 1, 2, 4, 4, 6, 3, 4, 1, 2, 1, 2, 6, 5, 1, 8, 8, 0},
{-1, 0, 4, 3, 7, 5, 1, 9, 3, 4, 4, 5, 2, 9, 4, 1, 9, 5, 7, 4, 8, 0, -1},
{-1, 0, 4, 1, 6, 7, 8, 3, 4, 3, 4, 1, 3, 1, 2, 3, 2, 3, 6, 2, 4, 0, -1},
{-1, 0, 7, 3, 2, 6, 1, 5, 3, 9, 2, 3, 2, 1, 5, 7, 5, 8, 9, 5, 4, 0, -1},
{-1,-1, 0, 1, 6, 7, 3, 4, 8, 1, 1, 1, 2, 1, 2, 2, 8, 9, 4, 1, 0, -1,-1},
{-1,-1, 0, 2, 5, 4, 7, 8, 7, 5, 6, 1, 3, 5, 7, 8, 7, 2, 9, 3, 0, -1,-1},
{-1,-1,-1, 0, 6, 5, 6, 4, 6, 7, 2, 5, 2, 2, 6, 3, 4, 7, 4, 0, -1,-1,-1},
{-1,-1,-1,-1, 0, 2, 3, 1, 2, 3, 3, 3, 2, 1, 3, 2, 1, 1, 0, -1,-1,-1,-1},
{-1,-1,-1,-1,-1, 0, 0, 7, 4, 4, 5, 7, 3, 4, 4, 7, 0, 0, -1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 3, 3, 4, 0, 0, 0, -1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1} };

#define WIDTH 900
#define HEIGHT 1074
#define FILESIZE (54+3*WIDTH*HEIGHT)
#define NSIZE 38

Klondike lab(MAPA);
// Semilla, tiempo actual
// Generador aleatorio
// Distribución personalizada
std::list<Point> path;
std::list<Point> ady;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine generator(seed);

int checkPath(Point point, bool write) {
  static std::uniform_int_distribution<int> distr(1,9);

  lab.setMap(point, distr(generator));
  ady = lab.adyacent(point);

  for (std::list<Point>::iterator it = ady.begin(); it != ady.end();++it){
    bool isPath = std::find(path.begin(), path.end(), *it) != path.end();
    if (isPath)
    return -1;
    else if(lab.getMap(*it) == 0) {
      if(write) path.push_back(point);
      return 0;
    }
  }
  return 1;
}

int main() {

  // Purgar mapa antiguo
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (lab.getMap(Point(i,j)) > 0)
      lab.setMap(Point(i,j), -2);
    }
  }

  // Inicio
  Point point(11,11);

  // Bandera de control del bucle
  int flag;
  // Salir si hemos llegado a la salida
  while ((flag = checkPath(point, true))) {
    // Si el camino se cruza volver a intentarlo
    if (flag == -1) continue;

    // Elegir una dirección aletoria de las válidas
    std::uniform_int_distribution<int> value(0, ady.size()-1);
    auto it = ady.begin();
    std::advance(it, value(generator));

    // Añadir al camino esta casilla
    path.push_back(*it);
    point = *it;
  }

  std::cout << "CAMINO EN EL MAPA" << std::endl;
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (lab.getMap(Point(i,j)) == -2) {
        std::cout << std::setw(2) << '-';
        //lab.setMap(Point(i,j),-1);
      }
      else
      std::cout << std::setw(2) << lab.getMap(Point(i,j));
    }
    std::cout << std::endl;
  }

  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      Point p(i,j);
      if (lab.getMap(p) == -2) {
        for (int t = 0;(flag = checkPath(p,false)) != 1;t++) {
          if (t == 30) {
            lab.setMap(p,-1);
            break;
          }
        }
      }
    }
  }

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
