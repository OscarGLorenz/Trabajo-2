#include "Mapa.h"

#include <list>
#include <iostream>
#include <limits>
#include <algorithm>
#include <stdio.h>

#define SIZE 23

class Point {
public:
  Point(short X, short Y) : x(X), y(Y) {}
  short x;
  short y;
  bool operator==(Point other) {
    return (this->x == other.x) && (this->y == other.y);
  }
} ;

class Klondike {
public:
  Klondike(short klondikeMap[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        map[i][j] = klondikeMap[i][j];
        cost[i][j] = std::numeric_limits<int>::max();
      }
    }
  }

  int solve(std::list<Point>& path, Point now) {
    int soluciones = 0;
    std::list<Point> sol;
    sol.push_back(now);
    setCost(now, 0);
    int minD = std::numeric_limits<int>::max();

    while(!sol.empty()) {
      Point s = sol.front();
      sol.pop_front();

      std::list<Point> ady = adyacente(s);
      for (std::list<Point>::const_iterator it = ady.begin(); it != ady.end(); ++it){
        if (getCost(*it) == std::numeric_limits<int>::max() && getMap(*it) != -1)  {
          setCost(*it, getCost(s) + 1);
          sol.push_back(*it);
          if (getMap(*it) == 0 && getCost(*it) == minD) {
            soluciones++;
          } else if( getMap(*it) == 0 && getCost(*it) < minD) {
            minD = getCost(*it);
            path.clear();
            path.push_front(*it);
            soluciones = 1;
          }
        }
      }
    }

    for(int c = minD, d = minD; c > 0; c--) {
      for (unsigned int i = 0; i < SIZE && c == d; i++) {
        for (unsigned int j = 0; j < SIZE && c == d; j++) {
          if (cost[i][j] == d-1) {
            std::list<Point> ady = adyacente(Point(i,j));
            std::list<Point>::iterator it = std::find(ady.begin(), ady.end(), path.front());
            if (it != ady.end()) {
              path.push_front(Point(i,j));
              d--;
            }
          }
        }
      }
    }
    return soluciones;
  }

  void setCost(Point p, int newCost) {
    cost[p.x][p.y] = newCost;
  }
  int getCost(Point p) {
    return cost[p.x][p.y];
  }
  int getMap(Point p) {
    return map[p.x][p.y];
  }
private:

  std::list<Point> adyacente(Point now) {
    std::list<Point> ady;
    short i = now.x;
    short j = now.y;
    short mov = map[i][j];
    bool flag;

    if (i + mov <= SIZE - 1) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i+k][j] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i+mov,j));
    }

    if (i - mov >= 0) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i-k][j] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i-mov,j));
    }

    if (j + mov <= SIZE - 1) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i][j+k] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i,j+mov));
    }

    if (j - mov >= 0) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i][j-k] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i,j-mov));
    }

    if (i + mov <= SIZE - 1 && j + mov <= SIZE - 1) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i+k][j+k] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i+mov,j+mov));
    }

    if (i - mov >= 0 && j + mov <= SIZE - 1) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i-k][j+k] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i-mov,j+mov));
    }

    if (i + mov <= SIZE - 1 && j - mov >= 0) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i+k][j-k] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i+mov,j-mov));
    }

    if (i - mov >= 0 && j - mov >= 0) {
      flag = true;
      for(int k = 1; k <= mov-1; k++) {
        if (map[i-k][j-k] == 0) flag = false;
      }
      if(flag)
      ady.push_back(Point(i-mov,j-mov));
    }

    return ady;
  }

  int cost[SIZE][SIZE];
  short map[SIZE][SIZE];
};


int main() {
  Klondike lab(MALOMAPA);
  std::list<Point> sol;
  std::cout << lab.solve(sol, Point((SIZE-1)/2,(SIZE-1)/2)) << std::endl;

  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      printf("%4d", MALOMAPA[i][j]);
    }
    printf("\n");
  }
  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      printf("%4d", (lab.getCost(Point(i,j)) == std::numeric_limits<int>::max()) ? -1 : lab.getCost(Point(i,j)));
    }
    printf("\n");
  }

  for (std::list<Point>::const_iterator it = sol.begin(); it != sol.end(); ++it){
    std::cout << it->x << " " << it->y << std::endl;
  }

  for (unsigned int i = 0; i < SIZE; i++) {
    for (unsigned int j = 0; j < SIZE; j++) {
      if (lab.getMap(Point(i,j)) != 0 || lab.getCost(Point(i,j)) == std::numeric_limits<int>::max())
      printf("%4c", '-');
      else
      printf("%4d", lab.getCost(Point(i,j)));
    }
    printf("\n");
  }


}
