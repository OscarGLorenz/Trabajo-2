/******************************************************************************
* ARCHIVO :        MapGenerator.hpp
*
* DESCRIPCIÓN :
*       Contiene una clase para generar mapas aleatorios y cargar el mapa en openGL
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/

#ifndef MAPGENERATOR_HPP
#define MAPGENERATOR_HPP

#include <random>
#include <list>
#include <string>

#include "Klondike.hpp"

#ifdef __APPLE__
    #include "GLUT/glut.h"
#else
    #include "GL/glut.h"
#endif

#define WIDTH 900 // Ancho de la imagen
#define HEIGHT 1074 // Alto de la imagen
#define FILESIZE (54+3*WIDTH*HEIGHT) // Tamaño del archivo
#define NSIZE 38 // Tamaño de una casilla

#define RELATIVE_PATH "graphics/" // Carpeta de gráficos
#define EMPTY_MAP "empty.bmp" // Imagen vacia
#define OUTPUT_MAP "img.bmp" // Imagen generada
#define ORIGINAL_MAP "klondike.bmp" // Imagen original

/*
 * Clase: MapGenerator
 * --------------------------------------------------------
 *   Clase principal, para generar mapas aleatorios y cargar el mapa en openGL
 */
class MapGenerator {
public:
  /*
   * Constructor
   * --------------------------------------------------------
   *  Genera un objeto MapGenerator a partir de un puntero a
   *  un objeto klondike que contiene el mapa
   */
  MapGenerator(Klondike * klondike);

  /*
   * Función: random
   * --------------------------------------------------------
   *  El mapa referenciado pasa a ser un mapa aleatorio, cuya
   *  salida tiene un número MÍNIMO de movimientos
   *
   *  minMoves: mínimo número de movimientos hasta la salida
   */
  void random(unsigned int minMoves);

  /*
   * Función: createMap
   * --------------------------------------------------------
   *  Genera una imagen de mapa con el mapa actual.
   *  En los defines de arriba estan las rutas a las las imagenes
   */
  void createMap();

  /*
   * Función: loadMap
   * --------------------------------------------------------
   *  Carga el mapa actual en la memoria, el original si no
   *  se ha generado uno nuevo, o el generado aleatoriamente
   *
   *  resultado: número de GL de la textura
   */
  GLuint loadMap();

  /*
   * Función: displayMap
   * --------------------------------------------------------
   *  Muestra el mapa en la ventana de openGL
   *
   *  texture: número de GL de la textura
   */
  void displayMap(GLuint texture);

  /*
   * Función: getCmdPath
   * --------------------------------------------------------
   *  Actualiza la ruta donde esta el ejecutable, para encontrar las
   *  imagenes correctamente
   *
   *  argv: Argumentos de ejecución del main
   *
   *  resultado:  ruta al ejecutable
   */
  void getCmdPath(char ** argv);

private:
  Klondike * lab; // Referencia al objeto que contiene el mapa
  std::list<Point> path; // Auxiliar: Camino a la salida
  std::list<Point> ady; // Auxiliar: Casillas adyacentes
  std::default_random_engine generator; // Generador de número aleatorios
  std::string relative_path; // Ruta al ejecutable
  bool original; // Flag para ver si estamos usando el mapa original

  /*
   * Función: checkPath
   * --------------------------------------------------------
   *  Genera la siguiente posición aleatoria del mapa aleatorio
   *
   *  point: punto actual
   *  write: true si generamos el camino, false si rellenamos el resto del mapa
   *  minMoves: mínimo número de movimientos para salir
   *
   *  resultado:  1 si la siguiente posición es válida
   *              -1 si la siguiente posición es inválida (se cruza sobre si
   *              mismo el camino, o se puede hacer trampa o no tenemos el
   *              número de movimientos mínimo si vamos a salir esta vez)
   *              0 si hemos llegado a la salida
   */
  int checkPath(Point point, bool write, unsigned int minMoves);
};

#endif
