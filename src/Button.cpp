/******************************************************************************
* ARCHIVO :        Button.cpp
*
* DESCRIPCIÓN :
*       Clase para crear botones y asignares funciones
*
*       Todo el código para dibujar los botones en sus diferentes posciones y
*       comprobar sus estados y el ratón han sido extraidos de los ejemplos de
*       esta página:
*       https://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html
*
*       Por nuestra parte hemos encapsulado todo su código en una clase y
*       mejorado los callback usando std::function
*
* NOTA:
*       Documentación de uso de las funciones en la cabecera (include/)
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/

#include "Button.hpp"

#ifdef __APPLE__
#include "GLUT/glut.h"
#elif __unix__
#include <GL/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glut.h"
#endif

void Button::font(void *font,unsigned char *text,int x,int y) {
  glRasterPos2i(x, y); // Posicón del texto
  while( *text != '\0' ) { // Representar los caracteres
    glutBitmapCharacter( font, *text );
    ++text;
  }
}

Button::Button (int ox, int oy, int width, int height, std::string str, std::function<void()> call) {
  x = ox;
  y = oy;
  w = width;
  h = height;
  state = highlighted = 0;
  label = str;
  callback = call;
}

bool Button::clickTest(int cx,int cy) {
  return cx > x && cx < x+w && cy > y && cy < y+h;
}

bool Button::press(int x,int y)  {
    oldX = x;
    oldY = y;
    // Si el botón ha sido pulsado cambiar de estado
    if( clickTest(x,y) ) {
      state = 1;
      return true;
    } else
    return false;
}

void Button::release(int x,int y) {
    oldX = x;
    oldY = y;
    // Si el botón ha sido soltado ejecutar la funcionalidad
    if( clickTest(oldX,oldY) &&	clickTest(x,y) )
      callback();

    state = 0;
}

void Button::passive(int x,int y) {
  if(clickTest(x,y) )	{
    // Si el cursor está encima del botón, marcar el botón y redibujar
    if( highlighted == 0 ) {
      highlighted = 1;
      glutPostRedisplay();
    }
  }
  else {
    //Si el cursor se sale del botón, desmarcar y redibujar
    if( highlighted == 1 ) {
      highlighted = 0;
      glutPostRedisplay();
    }
  }
}

void Button::motion(int x, int y) {
  if(clickTest(x,y) ) {
    if(highlighted == 0 ) {
      highlighted = 1;
      glutPostRedisplay();
    }
  }
  else
  if( highlighted == 1 ) {
    highlighted = 0;
    glutPostRedisplay();
  }
}

void Button::draw() {
  int fontx;
  int fonty;

  // Cambiar el color si esta marcado el botón
  if (highlighted)
  glColor3f(0.7f,0.7f,0.8f);
  else
  glColor3f(0.6f,0.6f,0.6f);

  // Dibujar el fondo del botón
  glBegin(GL_QUADS);
  glVertex2i(x, y);
  glVertex2i(x, y+h);
  glVertex2i(x+w, y+h);
  glVertex2i(x+w, y);
  glEnd();

  // Dibujar el marco
  glLineWidth(3);

  // Si está pulsado, cambiar los colores
  if (state)
  glColor3f(0.4f,0.4f,0.4f);
  else
  glColor3f(0.8f,0.8f,0.8f);

  glBegin(GL_LINE_STRIP);
  glVertex2i(x+w, y);
  glVertex2i(x, y);
  glVertex2i(x, y+h);
  glEnd();

  if (state)
  glColor3f(0.8f,0.8f,0.8f);
  else
  glColor3f(0.4f,0.4f,0.4f);

  glBegin(GL_LINE_STRIP);
  glVertex2i(x, y+h);
  glVertex2i(x+w, y+h);
  glVertex2i(x+w, y);
  glEnd();

  glLineWidth(1);

  // Calcular las coordenadas para poner el texto
  fontx = x + (w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(unsigned char *) label.c_str())) / 2 ;
  fonty = y + (h+10)/2;

  // Si el botón está pulsado mover las letras un poco abajo
  if (state) {
    fontx+=2;
    fonty+=2;
  }

  // Si el cursor esta sobre el botón dibujar una sombra
  if(highlighted){
    glColor3f(0,0,0);
    font(GLUT_BITMAP_HELVETICA_10,(unsigned char *) label.c_str(),fontx,fonty);
    fontx--;
    fonty--;
  }

  glColor3f(1,1,1);
  font(GLUT_BITMAP_HELVETICA_10,(unsigned char *) label.c_str(),fontx,fonty);
}

std::vector<Button> Button::Buttons;
