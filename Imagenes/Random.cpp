#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>

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

int main() {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(1,9);

  for (unsigned int i = 0; i < 23; i++) {
    for (unsigned int j = 0; j < 23; j++) {
      if (MAPA[i][j] > 0)
        MAPA[i][j] = distribution(generator);
    }
  }

  int w = 900;
  int h = 1074;

  FILE *f, *origin, *number;
  unsigned char *img = NULL;
  int filesize = 54 + 3*w*h;

  img = (unsigned char *)malloc(3*w*h);
  memset(img,0,3*w*h);

  origin = fopen("empty.bmp","rb");

fseek(origin, 139, SEEK_SET);
for(int j=0; j<h; j++) {
  for(int i=0; i<w; i++) {
      int x=i; int y=(h-1)-j;
      img[(x+y*w)*3+0] = fgetc(origin);
      img[(x+y*w)*3+1] = fgetc(origin);
      img[(x+y*w)*3+2] = fgetc(origin);
      fseek(origin, (4-(w*3)%4)%4+1, SEEK_CUR);
    }
  }
fclose(origin);

for (int a=0; a < 23; a++) {
  for (int b=0; b < 23; b++) {
    if (MAPA[a][b] > 0) {
      std::string str;
      str.push_back( (char) MAPA[a][b] + '0');
      str.push_back( ((a+b)%2) ? 'N' : 'B');
      str += ".bmp";

      number = fopen(str.c_str(),"rb");
      fseek(number, 139, SEEK_SET);
      for(int j=0; j<38; j++) {
        for(int i=0; i<38; i++) {
            int x=i+415+(a-11)*38; int y=(h-1)-j-(h-1-574-(b-11)*38);
            unsigned char b = fgetc(number);
            unsigned char g = fgetc(number);
            unsigned char r = fgetc(number);
            fseek(number, 1, SEEK_CUR);

            img[(x+y*w)*3+0] = b;
            img[(x+y*w)*3+1] = g;
            img[(x+y*w)*3+2] = r;

          }
      }
      fclose(number);
    }
  }
}

  /******************************BMP HEADER*********************************/
  unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  unsigned char bmppad[3] = {0,0,0};

  bmpfileheader[2] = (unsigned char)(filesize);
  bmpfileheader[3] = (unsigned char)(filesize>> 8);
  bmpfileheader[4] = (unsigned char)(filesize>>16);
  bmpfileheader[5] = (unsigned char)(filesize>>24);

  bmpinfoheader[4] = (unsigned char)(w);
  bmpinfoheader[5] = (unsigned char)(w>> 8);
  bmpinfoheader[6] = (unsigned char)(w>>16);
  bmpinfoheader[7] = (unsigned char)(w>>24);
  bmpinfoheader[8] = (unsigned char)(h);
  bmpinfoheader[9] = (unsigned char)(h>> 8);
  bmpinfoheader[10] = (unsigned char)(h>>16);
  bmpinfoheader[11] = (unsigned char)(h>>24);
  /******************************BMP HEADER*********************************/

  f = fopen("img.bmp","wb");
  fwrite(bmpfileheader,1,14,f);
  fwrite(bmpinfoheader,1,40,f);
  for(int i=0; i<h; i++)
  {
    fwrite(img+(w*(h-i-1)*3),3,w,f);
    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
  }

  free(img);
  fclose(f);

}
