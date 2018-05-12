#include <chrono>
#include <random>
#include <list>
#include <algorithm>
#include <fstream>
#include <string>

#include "MapGenerator.hpp"
#include "Klondike.hpp"

MapGenerator::MapGenerator() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  generator = std::default_random_engine(seed);
  original = true;
}
short MapGenerator::map(Point p) {
  return lab.getMap(p);
}

void MapGenerator::createMap() {
  // Bitmap
  std::vector<char> img(3*WIDTH*HEIGHT);

  // Abrir plantilla
  std::fstream input;
  std::string file = std::string(RELATIVE_PATH) + std::string(EMPTY_MAP);
  input.open (file, std::fstream::in | std::fstream::binary);

  // Saltar cabecera del bmp
  input.seekg(139);

  // Leer colores de la plantilla
  for(int j=0; j<HEIGHT; j++) {
    for(int i=0; i<WIDTH; i++) {
      int x=i; int y=(HEIGHT-1)-j;
      img[(x+y*WIDTH)*3+0] = input.get();
      img[(x+y*WIDTH)*3+1] = input.get();
      img[(x+y*WIDTH)*3+2] = input.get();
      input.seekg((4-(WIDTH*3)%4)%4+1, input.cur);
    }
  }

  // Cerrar plantilla
  input.close();

  // Iterar por el mapa
  for (int a=0; a < 23; a++) {
    for (int b=0; b < 23; b++) {
      if (MAPA[a][b] > 0) {
        std::fstream number;

        // Generar nombre del archivo
        std::string str;
        str += RELATIVE_PATH;
        str.push_back( (char) MAPA[a][b] + '0');
        if (a == 11 && b == 11) // El del centro va al revés
        str.push_back('N');
        else
        str.push_back( ((a+b)%2) ? 'N' : 'B');
        str += ".bmp";

        // Abrir archivo de número
        number.open (str.c_str(), std::fstream::in | std::fstream::binary);

        // Saltar cabecera
        number.seekg(139);

        // Iterar por la imagen de la plantilla
        for(int j=0; j<NSIZE; j++) {
          for(int i=0; i<NSIZE; i++) {

            // Obtener esquina donde se va a poner el número
            int x=i+415+(a-11)*NSIZE; int y=(HEIGHT-1)-j-(HEIGHT-1-574-(b-11)*NSIZE);

            // Leer colores
            unsigned char blue = number.get();
            unsigned char green = number.get();
            unsigned char red = number.get();
            number.seekg(1, input.cur);

            // No poner el negro en la casilla central (el corazoncito)
            if(a==11 && b == 11 && blue == 0 && green == 0 && red == 0)
            continue;

            // Copiar colores del número
            img[(x+y*WIDTH)*3+0] = blue;
            img[(x+y*WIDTH)*3+1] = green;
            img[(x+y*WIDTH)*3+2] = red;

          }
        }

        // Cerrar número
        number.close();
      }
    }
  }

  /******************************BMP HEADER*********************************/
  char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  char bmppad[3] = {0,0,0};

  bmpfileheader[2] = (unsigned char)(FILESIZE);
  bmpfileheader[3] = (unsigned char)(FILESIZE>> 8);
  bmpfileheader[4] = (unsigned char)(FILESIZE>>16);
  bmpfileheader[5] = (unsigned char)(FILESIZE>>24);

  bmpinfoheader[4] = (unsigned char)(WIDTH);
  bmpinfoheader[5] = (unsigned char)(WIDTH>> 8);
  bmpinfoheader[6] = (unsigned char)(WIDTH>>16);
  bmpinfoheader[7] = (unsigned char)(WIDTH>>24);
  bmpinfoheader[8] = (unsigned char)(HEIGHT);
  bmpinfoheader[9] = (unsigned char)(HEIGHT>> 8);
  bmpinfoheader[10] = (unsigned char)(HEIGHT>>16);
  bmpinfoheader[11] = (unsigned char)(HEIGHT>>24);
  /******************************BMP HEADER*********************************/

  // Abrir archivo destino
  std::fstream output;
  file = std::string(RELATIVE_PATH) + std::string(OUTPUT_MAP);
  output.open (file, std::fstream::out | std::fstream::binary);

  // Escribir cabecera
  output.write(bmpfileheader,14);
  output.write(bmpinfoheader,40);

  for(int i=0; i<HEIGHT; i++) {
    // Escribir colores
    output.write(&img[(WIDTH*(HEIGHT-i-1)*3)],3*WIDTH);
    output.write(bmppad,(4-(WIDTH*3)%4)%4);
  }

  // Cerrar archivo destino
output.close();
}

void MapGenerator::random() {
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

}

int MapGenerator::checkPath(Point point, bool write) {
  static std::uniform_int_distribution<int> distr(1,9);

  lab.setMap(point, distr(generator));
  ady = lab.adyacent(point);

  for (std::list<Point>::iterator it = ady.begin(); it != ady.end();++it){
    bool isPath = std::find(path.begin(), path.end(), *it) != path.end();
    if (isPath) return -1;
    else if(lab.getMap(*it) == 0) {
      if(write) path.push_back(point);
      return 0;
    }
  }
  return 1;
}
