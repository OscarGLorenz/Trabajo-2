#include <chrono>
#include <fstream>
#include <random>
#include <string>
#include <vector>

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

int main() {
  // Semilla, tiempo actual
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  // Generador aleatorio
  std::default_random_engine generator(seed);
  // Distribución personalizada
  std::discrete_distribution<int> distribution {5,5,5,5,4,4,4,2,2};

  // Mapa aleatorio, cambia las casillas >0 por aleatorios
  for (unsigned int i = 0; i < 23; i++) {
    for (unsigned int j = 0; j < 23; j++) {
      if (MAPA[i][j] > 0)
      MAPA[i][j] = distribution(generator)+1;
    }
  }

  // Bitmap
  std::vector<char> img(3*WIDTH*HEIGHT);

  // Abrir plantilla
  std::fstream input;
  input.open ("graphics/empty.bmp", std::fstream::in | std::fstream::binary);

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
        str += "graphics/";
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
  output.open ("graphics/img.bmp", std::fstream::out | std::fstream::binary);

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
