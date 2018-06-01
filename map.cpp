#include<iostream>
#include<math.h>
#include<GL/glut.h>
#include "map.hpp"
#include<stdlib.h>
#define  ANCHO 800
#define ALTO 800
#define W 900.0
#define H 1074.0
#include <fstream>
#include <vector>
#include <list>
#include <limits> 

// Carga la textura
GLuint LoadTexture( int width, int height, const char * filename ) {
  GLuint texture;

  std::vector<char> data(3*W*H); // Mapa de bits
  std::fstream file; // Archivo
  file.open ("klondike.bmp", std::fstream::in | std::fstream::binary);

  file.seekg(139); // Saltar cabecera
  for(int j=0; j<height; j++) {
    for(int i=0; i<width; i++) {
      // Copiar colores BGR
      data[(i+j*width)*3+2] = file.get();
      data[(i+j*width)*3+1] = file.get();
      data[(i+j*width)*3+0] = file.get();
      file.seekg((4-(width*3)%4)%4+1, file.cur);
    }
  }

  file.close(); // Cerrar

  // **************Magia*****************
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data.data());
  // **************Magia*****************
   
  return texture;
}

// Muestra el mapa
void displaytext(GLuint Thetexture) {
  // **************Magia*****************
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, Thetexture);
   glBegin(GL_QUADS);
   // **************Magia*****************

   // Esquinas del mapa
   glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0*H/W, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0*H/W, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0*H/W, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0*H/W, 0.0);

   glEnd();
   glFlush();
   glDisable(GL_TEXTURE_2D);
}
