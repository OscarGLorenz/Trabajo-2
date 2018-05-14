#include <random>
#include <list>

#include "Klondike.hpp"
#include "GL/glut.h"

#define WIDTH 900
#define HEIGHT 1074
#define FILESIZE (54+3*WIDTH*HEIGHT)
#define NSIZE 38

#define RELATIVE_PATH "graphics/"
#define EMPTY_MAP "empty.bmp"
#define OUTPUT_MAP "img.bmp"
#define ORIGINAL_MAP "klondike.bmp"

class MapGenerator {
public:
  MapGenerator(Klondike * klondike);
  void random(unsigned int minMoves);
  void createMap();

  GLuint loadMap();
  void displayMap(GLuint texture);
private:
  Klondike * lab;
  std::list<Point> path;
  std::list<Point> ady;
  std::default_random_engine generator;
  bool original;
  int checkPath(Point point, bool write, unsigned int minMoves);
};
