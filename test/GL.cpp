#include "GL/glut.h"
#include <iomanip>
#include <iostream>
#include "Klondike.hpp"
#include "MapGenerator.hpp"
#include "Mapa.h"

#define  ANCHO 800
#define ALTO 800
#define W 900.0
#define H 1074.0

#define ESC 27

Klondike lab(MAPA);
MapGenerator map(&lab);

class GL {
public:
  static void OnDibuja(void) {
    //Borrado de la pantalla
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // proyeccion de las figuras
    map.displayMap(KLtexture);
    drawSnowMan(-0.144f+(oy-11.0)*0.422,-0.190f+(11.0-ox)*0.422,0);

    glLoadIdentity();

    // posicciona el punto de vista
    gluLookAt(0,-10,10,  // posicion del  ojo
  	    0.0, 0, 0.0,		        // hacia que punto mira
  	    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)

    //Al final, cambiar el buffer
    glutSwapBuffers();
    glutPostRedisplay();//se le indica que redibuje la pantalla
  }

  static void OnKeyboardDown(unsigned char key, int x, int y) {
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
      case 'S':
      case 's':
      map.random(10);
      map.createMap();
        ox = oy = 11;
        KLtexture = map.loadMap();
        glBindTexture (GL_TEXTURE_2D, KLtexture);

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

  static void drawSnowMan(float x, float y, float z) {
    glColor3f(1.0f, 0 , 1.0f); // Color RGB
    glPushMatrix(); // Crear matriz
    glTranslatef(x,y,z); // Mover muñeco
    glutSolidSphere(0.15f,20,20); // Dibujar esfera
    glPopMatrix(); // Eliminar matriz
  }

  static void init(int * argc,char* argv[]) {


    GL::ox = 11;
    GL::oy = 11;
    glutInit(argc, argv);
    glutInitWindowSize(ANCHO,ALTO);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Ejemplo GLUT");

    glutGameModeString("1024x768:32@75"); //the settings
    glutEnterGameMode(); //set glut to fullscreen using the
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
    glBindTexture (GL_TEXTURE_2D, KLtexture);

    // bucle del programa
    glutMainLoop();
  }

private:
  static int ox, oy;
  static GLuint KLtexture;
};

int GL::ox, GL::oy;
GLuint GL::KLtexture;

int main(int argc, char *argv[]) {
  GL::init(&argc,argv);

  return 0;
}
