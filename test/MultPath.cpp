#ifdef __APPLE__
    #include "GLUT/glut.h"
#else
    #include "GL/glut.h"
#endif

#include <iomanip>
#include <iostream>
#include "Klondike.hpp"
#include "MapGenerator.hpp"

#include "Mapa.h"
#include <list>

#define  ANCHO 800
#define ALTO 800
#define W 900.0
#define H 1074.0

#define ESC 27

Klondike lab(MAPA);
MapGenerator map(&lab);
std::list<Point> solution;

int ox = 11, oy = 11;
GLuint KLtexture;



void OnDibuja(void) {
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // proyeccion de las figuras
  map.displayMap(KLtexture);
//  drawSnowMan(-0.144f+(oy-11.0)*0.422,-0.190f+(11.0-ox)*0.422,0);

  glLineWidth(3.5);
  glColor3f(1.0, 0.0, 0.0);
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
  glEnd();

  glLoadIdentity();

  // posicciona el punto de vista
  gluLookAt(0,-10,10,  // posicion del  ojo
    0.0, 0, 0.0,		        // hacia que punto mira
    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)

    //Al final, cambiar el buffer
    glutSwapBuffers();
    glutPostRedisplay();//se le indica que redibuje la pantalla
  }

  void OnKeyboardDown(unsigned char key, int x, int y) {
    // Casillas posibles
    // Detectar tecla
    switch(key) {
      case ESC:     exit(1);
    }

    }

  int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(ANCHO,ALTO);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Ejemplo GLUT");

    glutGameModeString("1024x768:32@75"); //the settings
    //glutEnterGameMode(); //set glut to fullscreen using the
    //glutLeaveGameMode();

    //Habilitar las luces, la renderizacion y el color de los materiales
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    //glutFullScreen();

    //definir la proyeccion
    glMatrixMode(GL_PROJECTION);
    gluPerspective( 40.0, 1027.0/768.0, 0.1, 50);

    // color del fondo
    glClearColor(1.0f, 1.0f, 1.0f, 0.5f);

    // Define el punto de vista
    glMatrixMode(GL_MODELVIEW);

    // Definici�n call backs de GLUT
    glutDisplayFunc(OnDibuja);
    glutKeyboardFunc(OnKeyboardDown);

    // Carga la textura
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    KLtexture = map.loadMap();
    solution = lab.solve(Point(11,11));

    // bucle del programa
    glutMainLoop();

    return 0;
  }
