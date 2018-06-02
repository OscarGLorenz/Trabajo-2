/*************************************************************
*
* FICHERO: Figuras.cpp
*
* DESCRIPCIÓN :
* 	Contiene la implemenración de los distintos métodos
* 	prototipados en figuras.hpp
*
* AUTORES:
*
* 	Alejandro Redondo Ayala
*
*        Implementación y estructuración en clases de las
*        distintas figuras.
*
*     Óscar García Lorenz
*
*        Implementación y forma base del pico.
*
*************************************************************/

#include <iostream>
#include "Figuras.hpp"

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

void Figuras::setColor(Colors col){
  switch(col){
    case Colors::VOID:
    Color[0]=0.0f;
    Color[1]=0.0f;
    Color[2]=0.0f;
    Color[3]=0.0f;

    break;
    case Colors::RED:
    Color[0]=1.0f;
    Color[1]=0.0f;
    Color[2]=0.0f;
    Color[3]=0.5f;

    break;
    case Colors::GREEN:
    Color[0]=0.0f;
    Color[1]=1.0f;
    Color[2]=0.0f;
    Color[3]=0.5f;

    break;
    case Colors::BLUE:
    Color[0]=0.0f;
    Color[1]=0.0f;
    Color[2]=1.0f;
    Color[3]=0.5f;

    break;
  }
}

Minero::Minero(){
  position[0]=-0.180;
  position[1]=-0.205;
  position[2]=0.0;
  position[3]=0.0;
  position[4]=10.0;
  position[5]=0.0;

}


/***** DIBUJO DE PICO***/
void Minero::drawCube(float x, float y, float z, float R, float G, float B, float size,int xx,int yy) {
  x*=size;
  y*=size;
  z*=size;

  glPushMatrix();
  glDisable(GL_LIGHTING);
  glTranslatef((-0.350f+(yy-11.0)*0.422),(-0.205f+(11.0-xx)*0.422),0.125);
  glRotatef(0,1,0,0);
  glRotatef(position[4],0,1,0);
  glRotatef(0,0,0,1);
  glBegin(GL_POLYGON);

  glColor3f(R,G,B);
  glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );
  glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );
  glVertex3f( x-size*0.5,  y+size*0.5, z-size*0.5 );
  glVertex3f( x-size*0.5, y-size*0.5, z-size*0.5 );
  glEnd();

  glBegin(GL_POLYGON);
  glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
  glVertex3f(  x+size*0.5,  y+size*0.5, z+size*0.5 );
  glVertex3f( x-size*0.5,  y+size*0.5, z+size*0.5 ) ;
  glVertex3f( x-size*0.5, y-size*0.5, z+size*0.5 );
  glEnd();

  glBegin(GL_POLYGON);
  glVertex3f( x+size*0.5, y-size*0.5, z-size*0.5 );
  glVertex3f( x+size*0.5,  y+size*0.5, z-size*0.5 );
  glVertex3f( x+size*0.5,  y+size*0.5,  z+size*0.5 );
  glVertex3f( x+size*0.5, y-size*0.5,  z+size*0.5 );
  glEnd();

  glBegin(GL_POLYGON);
  glVertex3f( x-size*0.5, y-size*0.5,  z+size*0.5 );
  glVertex3f( x-size*0.5,  y+size*0.5,  z+size*0.5 );
  glVertex3f( x-size*0.5,  y+size*0.5, z-size*0.5 );
  glVertex3f( x-size*0.5, y-size*0.5, z-size*0.5 );
  glEnd();

  glBegin(GL_POLYGON);
  glVertex3f(  x+size*0.5,  y+size*0.5,  z+size*0.5 );
  glVertex3f(  x+size*0.5,  y+size*0.5, z-size*0.5 );
  glVertex3f( x-size*0.5,  y+size*0.5,z-size*0.5 );
  glVertex3f( x-size*0.5,  y+size*0.5,  z+size*0.5 );
  glEnd();

  glBegin(GL_POLYGON);
  glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );
  glVertex3f(  x+size*0.5, y-size*0.5, z+ size*0.5 );
  glVertex3f( x-size*0.5,y-size*0.5,  z+size*0.5 );
  glVertex3f( x-size*0.5, y-size*0.5, z-size*0.5 );
  glEnd();

  glFlush();

  glColor3f(0,0,0 );
  glLineWidth(1.5);
  glBegin(GL_LINES);
  glVertex3f(  x+size*0.5, y+size*0.5, z+size*0.5 );
  glVertex3f(  x-size*0.5, y+size*0.5, z+size*0.5 );

  glVertex3f(  x+size*0.5, y+size*0.5, z+size*0.5 );
  glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );

  glVertex3f(  x+size*0.5, y+size*0.5, z+size*0.5 );
  glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );

  glVertex3f(  x-size*0.5, y-size*0.5, z-size*0.5 );
  glVertex3f(  x-size*0.5, y-size*0.5, z+size*0.5 );

  glVertex3f(  x-size*0.5, y-size*0.5, z-size*0.5 );
  glVertex3f(  x-size*0.5, y+size*0.5, z-size*0.5 );

  glVertex3f(  x-size*0.5, y-size*0.5, z-size*0.5 );
  glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );

  glVertex3f(  x-size*0.5, y+size*0.5, z+size*0.5 );
  glVertex3f(  x-size*0.5, y+size*0.5, z-size*0.5 );

  glVertex3f(  x-size*0.5, y+size*0.5, z+size*0.5 );
  glVertex3f(  x-size*0.5, y-size*0.5, z+size*0.5 );

  glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );
  glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );

  glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );
  glVertex3f(  x-size*0.5, y+size*0.5, z-size*0.5 );

  glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
  glVertex3f(  x-size*0.5, y-size*0.5, z+size*0.5 );

  glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
  glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );
  glEnd();

  glFlush();

  glEnable(GL_LIGHTING);
  glPopMatrix();
  glFlush();
}

void Minero::draw(int x, int y){
  drawCube(0,0,0, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(1,0,0, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(0,0,1, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(1,0,1, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(2,0,1, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(1,0,2, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(2,0,2, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(3,0,2, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(2,0,3, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(3,0,3, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(4,0,3, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(11,0,3,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(3,0,4, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(4,0,4, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(5,0,4, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(10,0,4,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(11,0,4,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(12,0,4,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(4,0,5, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(5,0,5, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(6,0,5, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(10,0,5,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(11,0,5,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(12,0,5,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(5,0,6, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(6,0,6, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(7,0,6, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(10,0,6,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(11,0,6,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(12,0,6,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(6,0,7, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(7,0,7, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(8,0,7, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(10,0,7,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(11,0,7,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(12,0,7,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(7,0,8, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(8,0,8, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(9,0,8, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(10,0,8,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(11,0,8,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(12,0,8,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(8,0,9, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(9,0,9,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(10,0,9,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(11,0,9,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(4,0,10,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(5,0,10,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(6,0,10,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(7,0,10,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(8,0,10,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(9,0,10,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(10,0,10, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(11,0,10, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(3,0,11,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(4,0,11,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(5,0,11,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(6,0,11,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(7,0,11,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(8,0,11,0.2f, 0.922f, 0.796f, 0.035,x,y);
  drawCube(9,0,11,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(10,0,11, 0.3f, 0.15f, 0.05f, 0.035,x,y);
  drawCube(11,0,11, 0.5f, 0.35f, 0.05f, 0.035,x,y);
  drawCube(4,0,12,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(5,0,12,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(6,0,12,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(7,0,12,0.055f, 0.247f, 0.212f, 0.035,x,y);
  drawCube(8,0,12,0.055f, 0.247f, 0.212f, 0.035,x,y);
}

Cuadro::Cuadro(){
  Color[0]=0.0f;
  Color[1]=0.0f;
  Color[2]=0.0f;
}

void Cuadro::draw(int x, int y){
  glDisable(GL_LIGHTING);
  glPushMatrix();

  glColor4f(Color[0],Color[1],Color[2],Color[3]);

  glBegin(GL_POLYGON);

  glVertex3f(0.035f+(y-11.0)*0.422, 0.0f+(11.0-x)*0.422,0.01);
  glVertex3f(0.035f+(y-11.0)*0.422, -0.415f+(11.0-x)*0.422,0.01);
  glVertex3f(-0.380f+(y-11.0)*0.422, -0.415f+(11.0-x)*0.422,0.01);
  glVertex3f(-0.380f+(y-11.0)*0.422, 0.0f +(11.0-x)*0.422,0.01);
  glEnd();
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

void Plano::draw(int x, int y){
  glDisable(GL_LIGHTING);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glBegin(GL_QUADS);
  glVertex3f(-100.0f,-100.0f,-0.05f);
  glVertex3f(100.0f,-100.0f,-0.05f);
  glVertex3f(100.0f,100.0f,-0.05f);
  glVertex3f(-100.0f,100.0f,-0.05f);
  glEnd();
  glEnable(GL_LIGHTING);
}
