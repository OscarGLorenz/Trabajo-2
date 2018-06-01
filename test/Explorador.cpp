#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "MapGenerator.hpp"
#include "Klondike.hpp"
#include "Mapa.h"
#include "Camera.hpp"

#include <fstream>
#include <vector>
#include <cmath>
#include <list>
#include <iostream>
#include <string>
#include <cstring>
#include <functional>

#define ANCHO 800
#define ALTO 800

#define ESC 27

Klondike lab(MAPA);
MapGenerator map(&lab);
std::list<Point> solution;

Camera camera;

// Texture datas structure
GLuint KLtexture;

// Posición de la tetera
int ox = 11, oy = 11;

int oldX, oldY;
void Font(void *font,unsigned char *text,int x,int y) {
  glRasterPos2i(x, y);

  while( *text != '\0' )
  {
    glutBitmapCharacter( font, *text );
    ++text;
  }
}

class Button {
private:
  int   x;							/* top left x coord of the button */
  int   y;							/* top left y coord of the button */
  int   w;							/* the width of the button */
  int   h;							/* the height of the button */
  std::string label;						/* the text label of the button */
  std::function<void()> callback;

public:

  int	  state;						/* the state, 1 if pressed, 0 otherwise */
  int	  highlighted;					/* is the mouse cursor over the control? */

  Button (int ox, int oy, int width, int height, std::string str, std::function<void()> call) {
    x = ox;
    y = oy;
    w = width;
    h = height;
    state = highlighted = 0;
    label = str;
    callback = call;
  }

  bool ButtonClickTest(int cx,int cy) {
    return cx > x && cx < x+w && cy > y && cy < y+h;
  }

  bool ButtonPress(int x,int y)  {
      // Si el botón ha sido pulsado cambiar de estado
  		if( ButtonClickTest(x,y) ) {
  			state = 1;
        return true;
      } else
      return false;
  }

  void ButtonRelease(int x,int y) {
      // Si el botón ha sido soltado ejecutar la funcionalidad
  		if( ButtonClickTest(oldX,oldY) &&	ButtonClickTest(x,y) )
        callback();

  		state = 0;
  }

  void ButtonPassive(int x,int y) {
		if(ButtonClickTest(x,y) )	{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( highlighted == 0 ) {
				highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw.
		 */
		if( highlighted == 1 ) {
			highlighted = 0;
			glutPostRedisplay();
		}

}

  void draw() {
    int fontx;
    int fonty;
    /*
    *	We will indicate that the mouse cursor is over the button by changing its
    *	colour.
    */
    if (highlighted)
    glColor3f(0.7f,0.7f,0.8f);
    else
    glColor3f(0.6f,0.6f,0.6f);

    /*
    *	draw background for the button.
    */
    glBegin(GL_QUADS);
    glVertex2i( x     , y      );
    glVertex2i( x     , y+h );
    glVertex2i( x+w, y+h );
    glVertex2i( x+w, y      );
    glEnd();

    /*
    *	Draw an outline around the button with width 3
    */
    glLineWidth(3);

    /*
    *	The colours for the outline are reversed when the button.
    */
    if (state)
    glColor3f(0.4f,0.4f,0.4f);
    else
    glColor3f(0.8f,0.8f,0.8f);

    glBegin(GL_LINE_STRIP);
    glVertex2i( x+w, y      );
    glVertex2i( x     , y      );
    glVertex2i( x     , y+h );
    glEnd();

    if (state)
    glColor3f(0.8f,0.8f,0.8f);
    else
    glColor3f(0.4f,0.4f,0.4f);

    glBegin(GL_LINE_STRIP);
    glVertex2i( x     , y+h );
    glVertex2i( x+w, y+h );
    glVertex2i( x+w, y      );
    glEnd();

    glLineWidth(1);

    fontx = x + (w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(unsigned char *) label.c_str())) / 2 ;
    fonty = y + (h+10)/2;

    /*
    *	if the button is pressed, make it look as though the string has been pushed
    *	down. It's just a visual thing to help with the overall look....
    */
    if (state) {
      fontx+=2;
      fonty+=2;
    }

    /*
    *	If the cursor is currently over the button we offset the text string and draw a shadow
    */
    if(highlighted)
    {
      glColor3f(0,0,0);
      Font(GLUT_BITMAP_HELVETICA_10,(unsigned char *) label.c_str(),fontx,fonty);
      fontx--;
      fonty--;
    }

    glColor3f(1,1,1);
    Font(GLUT_BITMAP_HELVETICA_10,(unsigned char *) label.c_str(),fontx,fonty);
  }

};
std::vector<Button> buttons;


// Función que actualiza la pantalla
void OnDibuja(void) {
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Mapa
  map.displayMap(KLtexture);

  // Tetera
  glColor3f(0.0f,1.0f,0.0f);
  glEnable(GL_LIGHTING);
  glPushMatrix();
  glTranslatef(-0.144f+(oy-11.0)*0.422,-0.190f+(11.0-ox)*0.422,0.12); // Mover muñeco
  glRotatef(90, 1,0,0);
  glutSolidTeapot(0.15);
  glPopMatrix();
  glDisable(GL_LIGHTING);

  // Plano horizontal
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glBegin(GL_QUADS);
  glVertex3f(-100.0f,-100.0f,-0.05f);
  glVertex3f(100.0f,-100.0f,-0.05f);
  glVertex3f(100.0f,100.0f,-0.05f);
  glVertex3f(-100.0f,100.0f,-0.05f);
  glEnd();


  if (!solution.empty()) {
    glLineWidth(3.5);
    glColor4f(1.0, 0.0, 0.0,0.5f);
    glBegin(GL_LINE_STRIP);
    for (auto sol = solution.begin(); sol != --solution.end(); ++sol) {
      double x1 = -0.144f+(sol->y-11.0)*0.422;
      double x2 = -0.144f+(std::next(sol)->y-11.0)*0.422;
      double y1 = -0.190f+(11.0-sol->x)*0.422;
      double y2 = -0.190f+(11.0-std::next(sol)->x)*0.422;

      for (int t = 0; t <= 20; t++) {
        double alpha = (double)t*M_PI/20.0;
        double x = (x1-x2)/2.0 * cos(alpha) + (x1+x2)/2.0;
        double y = (y1-y2)/2.0 * cos(alpha) + (y1+y2)/2.0;
        double z = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))/2.0 * sin(alpha);
        glVertex3f(x,y,z);
      }
    }
    glColor4f(1.0, 0.0, 0.0,1.0f);
    glEnd();
  }

  camera.look2D();

  for (auto &b : buttons)
    b.draw();

  camera.look3D();

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();
}

void OnMouseBtn(int button, int state, int x, int y) {
  // Guardar punto de la pantalla
  oldX = x;
  oldY = y;

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    for (auto &b : buttons) {
      if(b.ButtonPress(x,y))
        return;
    }

    camera.moveMode(true,x,y);
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    for (auto &b : buttons)
      b.ButtonRelease(x,y);

    camera.moveMode(false,x,y);
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    camera.rotateMode(true,x,y);
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    camera.rotateMode(false,x,y);
  } else if (button == 4) {
    camera.zoom(1.1,20);
  } else if (button == 3) {
    camera.zoom(0.9,1);
  }

}

void OnMouseMove(int x, int y) {
  camera.mouseMove(x,y);
}

void OnMotion(int x, int y) {

  /*
  *	if the mouse moved over the control
  */
  for (auto& b : buttons) {

    if( b.ButtonClickTest(x,y) ) {
      /*
      *	If the cursor has just arrived over the control, set the highlighted flag
      *	and force a redraw. The screen will not be redrawn again until the mouse
      *	is no longer over this control
      */
      if( b.highlighted == 0 ) {
        b.highlighted = 1;
        glutPostRedisplay();
      }
    }
    else

    /*
    *	If the cursor is no longer over the control, then if the control
    *	is highlighted (ie, the mouse has JUST moved off the control) then
    *	we set the highlighting back to false, and force a redraw.
    */
    if( b.highlighted == 1 ) {
      b.highlighted = 0;
      glutPostRedisplay();
    }
  }
}
// Al pulsar una tecla
void OnKeyboardDown(unsigned char key, int x, int y) {
  Direction c = Direction::NONE;
  // Casillas posibles
  std::list<Point> ady = lab.adyacent(Point(ox,oy));
  // Detectar tecla
  switch(key) {
    case ESC:     exit(1);
    case 'W':
    case 'w': c = Direction::NORTH; break;
    case 'D':
    case 'd': c = Direction::EAST; break;
    case 'X':
    case 'x': c = Direction::SOUTH; break;
    case 'A':
    case 'a': c = Direction::WEST; break;
    case 'Q':
    case 'q': c = Direction::NORTHWEST; break;
    case 'E':
    case 'e': c = Direction::NORTHEAST; break;
    case 'Z':
    case 'z': c = Direction::SOUTHWEST; break;
    case 'C':
    case 'c': c = Direction::SOUTHEAST; break;

  }
  // Buscar si es un movimiento posible
  for (std::list<Point>::const_iterator it = ady.begin(); it != ady.end(); ++it){
    if (it->dir == c) {
      // Mover muñeco
      ox = it->x;
      oy = it->y;
    }
  }

}

int main(int argc,char* argv[]) {

  // Inicializaciones openGL
  //Creacion y definicion de la ventana
  glutInit(&argc, argv);
  glutInitWindowSize(ANCHO,ALTO);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Ejemplo GLUT");
  glEnable(GL_LIGHT0);

  camera.setAspect(ANCHO,ALTO);
  camera.setPrespective(40.0, ANCHO/ALTO, 0.1, 50);
  camera.setPosition(0,-10,10);

  // Definici�n call backs de GLUT
  glutDisplayFunc(OnDibuja);
  glutKeyboardFunc(OnKeyboardDown);
  glutMouseFunc(OnMouseBtn);
  glutMotionFunc(OnMouseMove);
  glutPassiveMotionFunc(OnMotion);

  // Actualizar ruta del ejecutable
  map.getCmdPath(argv);
  // Carga la textura
  KLtexture= map.loadMap();

  buttons.push_back(Button(5,5, 100,25, "INICIO", [&](){
    ox=11;
    oy=11;
  }));

  buttons.push_back(Button(115,5, 100,25, "RANDOM", [&](){
    map.random(10);
    map.createMap();
    ox = oy = 11;
    KLtexture = map.loadMap();
    if (!solution.empty()) {
      solution.empty();
      solution = lab.solve(Point(ox,oy));
    }
  }));

  buttons.push_back(Button(225,5, 100,25, "OCULTAR", [&](){
    solution.clear();
  }));

  buttons.push_back(Button(335,5, 100,25, "MOSTRAR", [&](){
    solution = lab.solve(Point(ox,oy));
  }));
  // bucle del programa
  glutMainLoop();

  return 0;
}
