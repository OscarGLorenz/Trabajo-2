#include <random>
#include <list>

#include "Klondike.hpp"
#include "Mapa.h"

#define WIDTH 900
#define HEIGHT 1074
#define FILESIZE (54+3*WIDTH*HEIGHT)
#define NSIZE 38

#define RELATIVE_PATH "graphics/"
#define EMPTY_MAP "empty.bmp"
#define OUTPUT_MAP "img.bmp"
#define ORIGINAL_MAP "klondike.bmp"

class MapGenerator {
  MapGenerator();
  short map(Point p);
  void createMap();
  void random();

private:
  Klondike lab = Klondike(MAPA);
  std::list<Point> path;
  std::list<Point> ady;
  std::default_random_engine generator;
  bool original;
  int checkPath(Point point, bool write);

};
