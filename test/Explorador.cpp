#ifdef __APPLE__
    #include "GLUT/glut.h"
#else
    #include "GL/glut.h"
#endif

#include "MapGenerator.hpp"
#include "Klondike.hpp"
#include "Mapa.h"

#include <fstream>
#include <vector>
#include <cmath>
#include <list>
#include <iostream>

// Limita una variable entre un máximo y un mínimo. Arduino
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define  ANCHO 800
#define ALTO 800

#define ESC 27

Klondike lab(MAPA);
MapGenerator map(&lab);
std::list<Point> solution;

// Texture datas structure
GLuint KLtexture;

// Posición de la tetera
int ox = 11, oy = 11;

GLdouble tras[3] = {0,0,0}; // Traslación actual
GLdouble pos[3] = {0,-10,10}; // Posición actual
GLdouble oldPos[3] = {0,-10,10}; // Posición anterior
GLdouble oldTras[3] = {0,0,0}; // Traslación anterior
int oldX = ANCHO/2, oldY = ALTO/2; // Posición del cursor anterior

// Modos rotación y movimientos con el ratón
bool rotate = false, move= false;

// Funci�n que actualiza la pantalla
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


  glLoadIdentity();

  // Posiciona el punto de vista
  gluLookAt(pos[0]+tras[0],pos[1]+tras[1],pos[2]+tras[2],
    tras[0],tras[1],tras[2],	        // hacia que punto mira
    0.0, 0.0, 1.0);         // vector "UP"  (vertical positivo)

    //Al final, cambiar el buffer
    glutSwapBuffers();
    glutPostRedisplay();//se le indica que redibuje la pantalla
  }

  // Transforma la posición del cursos en la pantalla a la posición en el mundo
  void screenToWorld(int x, int y, GLdouble *outX, GLdouble *outY, GLdouble *outZ) {
    GLfloat wx = x,wy,wz; // Coordenadas auxiliares
    GLint viewport[4]; // Punto de vista
    glGetIntegerv(GL_VIEWPORT,viewport); // Obtener punto de vista
    y = wy = viewport[3] -y; // Trasformar la componente vertical de la pantalla
    GLdouble modelview[16],projection[16]; // Matriz de proyección y posición
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview); // Obtener matriz de posición
    glGetDoublev(GL_PROJECTION_MATRIX,projection); // Obtener matriz de posición

    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz); // Obtener profundidad
    // Proyección inversa
    gluUnProject(wx,wy,wz,modelview,projection,viewport,outX,outY,outZ);
    glutPostRedisplay();
  }

  void OnMouseBtn(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      move = true;
      // Obtener traslación del mundo y guardar
      screenToWorld(x, y, &oldTras[0], &oldTras[1], &oldTras[2]);
      // Limitar para no salirse del mapa
      oldTras[0] = constrain(oldTras[0],-5.0f,5.0f);
      oldTras[1] = constrain(oldTras[1],-5.0f,5.0f);
      oldTras[2] = 0;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      move = false;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      rotate = true;
      // Guardar punto de la pantalla
      oldX = x;
      oldY = y;
      // Guardar posición del mundo
      oldPos[0] = pos[0];
      oldPos[1] = pos[1];
      oldPos[2] = pos[2];
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
      rotate = false;
    } else if (button == 4) {
      if (1.1*sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]) < 20) {
        oldPos[0] = pos[0] *= 1.1;
        oldPos[1] = pos[1] *= 1.1;
        oldPos[2] = pos[2] *= 1.1;
      }
    } else if (button == 3) {
      if (0.9*sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]) > 1) {
        oldPos[0] = pos[0] *= 0.9;
        oldPos[1] = pos[1] *= 0.9;
        oldPos[2] = pos[2] *= 0.9;
      }
    }

  }

  void OnMouseMove(int x, int y) {
    if (move) {
      GLdouble aux[3];
      // Obtener traslación del mundo
      screenToWorld(x, y, &aux[0], &aux[1], &aux[2]);

      // Desplazar
      tras[0] += (oldTras[0] - aux[0])/10.0;
      tras[1] += (oldTras[1] - aux[1])/10.0;

      // Limitar para no salirse del mapa
      tras[0] = constrain(tras[0],-5.0,5.0);
      tras[1] = constrain(tras[1],-5.0,5.0);
      tras[2] = 0; // Forzar al plano horizontal
    } else if (rotate) {
      float al = (float) (oldX-x)/200.0;  // Ángulo polar, horizontal
      float be = (float) (y-oldY)/200.0; // Azimut, vertical

      // Antiguo azimut
      float oldBe = atan(oldPos[2]/sqrt(oldPos[0]*oldPos[0]+oldPos[1]*oldPos[1]));

      // Limitar ángulo beta a 0,pi/2
      be = constrain(be+oldBe, 0.01, 3.141/2) - oldBe;

      // Vector unitario para eje de beta
      float a = (float)oldPos[1]/sqrt(oldPos[0]*oldPos[0]+oldPos[1]*oldPos[1]);
      float b = -(float)oldPos[0]/sqrt(oldPos[0]*oldPos[0]+oldPos[1]*oldPos[1]);

      // Girar respecto a (a,b,0) un ángulo be
      pos[0] = oldPos[0]*(cos(be)+a*a*(1-cos(be))) + oldPos[1]*a*b*(1-cos(be))           + oldPos[2]*b*sin(be);
      pos[1] = oldPos[0]*a*b*(1-cos(be))           + oldPos[1]*(cos(be)+b*b*(1-cos(be))) + oldPos[2]*-a*sin(be);
      pos[2] = oldPos[0]*-b*sin(be)                + oldPos[1]*a*sin(be)                 + oldPos[2]*cos(be);

      // Girar respecto a (0,0,1) un ángulo al
      pos[0] = pos[0]*cos(al)+pos[1]*-sin(al);
      pos[1] = pos[0]*sin(al)+pos[1]*cos(al) ;
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
      case 'S':
      case 's': ox = 11; oy = 11; break;
      case 'F':
      case 'f': solution = lab.solve(Point(ox,oy)); break;
      case 'G':
      case 'g': solution.clear(); break;
      case 'R':
      case 'r':

      map.random(10);
      map.createMap();
      ox = oy = 11;
      KLtexture = map.loadMap();
      if (!solution.empty()) {
        solution.empty();
        solution = lab.solve(Point(ox,oy));
      }

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
    // Actualizar ruta del ejecutable
    map.getCmdPath(argv);

    // Inicializaciones openGL
    //Creacion y definicion de la ventana
    glutInit(&argc, argv);
    glutInitWindowSize(ANCHO,ALTO);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Ejemplo GLUT");

    //Habilitar las luces, la renderizacion y el color de los materiales
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //definir la proyeccion
    glMatrixMode(GL_PROJECTION);
    gluPerspective( 40.0, ANCHO/ALTO, 0.1, 50);

    // color del fondo
    glClearColor(1.0f,1.0f,1.0f,0.0f);

    // Define el punto de vista
    glMatrixMode(GL_MODELVIEW);

    // Definici�n call backs de GLUT
    glutDisplayFunc(OnDibuja);
    glutKeyboardFunc(OnKeyboardDown);
    glutMouseFunc(OnMouseBtn);
    glutMotionFunc(OnMouseMove);

    // Carga la textura
    KLtexture= map.loadMap();

    std::cout << "Movimiento" << std::endl;
    std::cout << "   QWE    " << std::endl;
    std::cout << "   A D    " << std::endl;
    std::cout << "   ZXC    " << std::endl;
    std::cout << "Nuevo mapa: R" << std::endl;
    std::cout << "Mostrar solución: F" << std::endl;
    std::cout << "Quitar solución: G" << std::endl;
    std::cout << "Volver al centro: S" << std::endl;
    std::cout << "Mover cámara: clic izq y arrastrar" << std::endl;
    std::cout << "Rotar cámara: clic der y arrastrar" << std::endl;
    std::cout << "Zoom: rueda del ratón" << std::endl;


    // bucle del programa
    glutMainLoop();

    return 0;
  }
