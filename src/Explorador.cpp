/*************************************************************
*
* ARCHIVO : Explorador.cpp
*
* DESCRIPCIÓN:
* 	Contiene el main del programa, a lo largo de todo el
* 	fichero se hacen llamadas a funciones de OpenGl
*     y se definen funciones Callbacks
*
* AUTORES:
*
* 	Alejandro Redondo Ayala:
*     Implementación de las figuras para representar el desarrollo
*     del algoritmo y la lógica del minero
*
*   Óscar García Lorenz
*	    Implementación de los objetos para controlar la cámara y
*     los botones
*
*************************************************************/

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "MapGenerator.hpp"
#include "Klondike.hpp"
#include "Mapa.h"
#include "Camera.hpp"
#include "Button.hpp"
#include "Figuras.hpp"

#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>

#define ANCHO 800
#define ALTO 800

#define ESC 27

Klondike lab(MAPA);
MapGenerator map(&lab);
std::list<Point> solution;
std::vector<SearchResult> v;

Camera camera;

GLuint KLtexture; // Texture datas structure

Minero Jack;
Cuadro Azul;
Plano Base;
Colors Casillas[23][23];
bool visited[23][23];

//PRUEBAS MOV
// Casilla del mapa
int ox = 11, oy = 11,oxblue,oyblue;
unsigned int k=-1;
int i,j;

// Función que actualiza la pantalla
void OnDibuja(void) {
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Mapa
  map.displayMap(KLtexture);

  Base.draw(0,0);
  Jack.draw(ox,oy);

  for(i=0;i<23;i++){
    for(j=0;j<23;j++){
      Azul.setColor(Casillas[i][j]);
      Azul.draw(i,j);
    }
  }

  glDisable(GL_LIGHTING);
  if (!solution.empty()) {
    glLineWidth(3.5);
    glColor4f(1.0, 1.0, 0.0,0.7f);
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
  glEnable(GL_LIGHTING);

  camera.look2D();

  for (auto &b : Button::Buttons)
  b.draw();

  camera.look3D();

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();
}

void OnMouseBtn(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    for (auto &b : Button::Buttons) {
      if(b.press(x,y))
      return;
    }

    camera.moveMode(true,x,y);
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    for (auto &b : Button::Buttons)
    b.release(x,y);

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
  for (auto& b : Button::Buttons) {
    b.motion(x,y);
  }
}

void OnKeyboardDown(unsigned char key, int x, int y) {
  if (key == ESC)     exit(1);
}

void myLogic(){
  static int status = 1;

  if(Jack.position[4]>=20 && status ==1) status=0;
  if(Jack.position[4]<=-20 && status == 0) status=1;
  switch(status){
    case 1:
    Jack.position[4]+=2.0; break;
    case 0:
    Jack.position[4]-=2.0; break;
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
  //TRANSPARENCIAS
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  camera.setAspect(ANCHO,ALTO);
  camera.setPrespective(40.0, ANCHO/ALTO, 0.1, 50);
  camera.setPosition(0,-10,10);
  camera.setLimits(5.0);


  // Definici�n call backs de GLUT
  glutDisplayFunc(OnDibuja);
  glutKeyboardFunc(OnKeyboardDown);
  glutMouseFunc(OnMouseBtn);
  glutMotionFunc(OnMouseMove);
  glutPassiveMotionFunc(OnMotion);
  glutIdleFunc(myLogic);

  // Actualizar ruta del ejecutable
  map.getCmdPath(argv);
  // Carga la textura
  KLtexture= map.loadMap();

  // Carga las casillas con sus valores iniciales (no visitado)
  for(int i=0; i<23; i++){
    for(int j=0; j<23; j++){
      if(MAPA[i][j]<=0)
      Casillas[i][j]=Colors::VOID;
      else
      Casillas[i][j]=Colors::RED;
    }
  }

  Button::Buttons.push_back(Button(5,5, 100,25, "REINICIAR", [&](){
    oy=ox=11;
    for (int i = 0; i < 23; i++) {
      for (int j = 0; j < 23; j++) {
        visited[i][j] = false;
        if (Casillas[i][j] != Colors::VOID)
        Casillas[i][j] = Colors::RED;
      }
    }
  }));

  Button::Buttons.push_back(Button(5,40, 150,25, "GENERAR MAPA NUEVO", [&](){
    map.random(10);
    map.createMap();
    ox = oy = 11;
    KLtexture = map.loadMap();
    if (!solution.empty()) {
      solution.empty();
      solution = lab.solve(Point(ox,oy),&v);
    }
    for (int i = 0; i < 23; i++) {
      for (int j = 0; j < 23; j++) {
        visited[i][j] = false;
        if (Casillas[i][j] != Colors::VOID)
        Casillas[i][j] = Colors::RED;
      }
    }
  }));

  Button::Buttons.push_back(Button(225,5, 100,25, "VER SOLUCION", [&](){
    static bool status = false;
    if (status)
    solution.clear();
    else
    solution = lab.solve(Point(ox,oy),&v);
    status = !status;
  }));

  Button::Buttons.push_back(Button(335,5, 100,25, "VER ALGORITMO", [&](){

  }));
  Button::Buttons.push_back(Button(690,5, 100,25, "INSTRUCCIONES", [&](){

  }));

  auto search_ady =  [&](Direction c){
    std::list<Point> ady = lab.adyacent(Point(ox,oy));
    for (auto it = ady.begin(); it != ady.end(); ++it){
      if (it->dir == c) {
        // Mover muñeco
        ox = it->x;
        oy = it->y;
        visited[it->x][it->y] = true;
      }
    }
    for (int i = 0; i < 23; i++) {
      for (int j = 0; j < 23; j++) {
        if(visited[i][j])
        Casillas[i][j] = Colors::GREEN;

        if (Casillas[i][j] == Colors::BLUE)
        Casillas[i][j] = Colors::RED;
      }
    }
    ady = lab.adyacent(Point(ox,oy));
    for (auto it = ady.begin(); it != ady.end(); ++it){
      Casillas[it->x][it->y] = Colors::BLUE;
    }

  };

  Button::Buttons.push_back(Button(445,5, 120,25, "MODO INTERACTIVO", [&](){
    static bool interactive = true;
    if (interactive) {
      Button::Buttons.push_back(Button(700,100, 25,25, "N", [&](){
        search_ady(Direction::NORTH);
      }));
      Button::Buttons.push_back(Button(730,130, 25,25, "E", [&](){
        search_ady(Direction::EAST);
      }));
      Button::Buttons.push_back(Button(670,130, 25,25, "W", [&](){
        search_ady(Direction::WEST);
      }));
      Button::Buttons.push_back(Button(700,160, 25,25, "S", [&](){
        search_ady(Direction::SOUTH);
      }));
      Button::Buttons.push_back(Button(670,100, 25,25, "NW", [&](){
        search_ady(Direction::NORTHWEST);
      }));
      Button::Buttons.push_back(Button(730,160, 25,25, "SE", [&](){
        search_ady(Direction::SOUTHEAST);
      }));
      Button::Buttons.push_back(Button(670,160, 25,25, "SW", [&](){
        search_ady(Direction::SOUTHWEST);
      }));
      Button::Buttons.push_back(Button(730,100, 25,25, "NE", [&](){
        search_ady(Direction::SOUTHWEST);
      }));
    } else {
      Button::Buttons.erase(Button::Buttons.end()-=8, Button::Buttons.end());
    }
    interactive = !interactive;
  }));





  // bucle del programa
  glutMainLoop();

  return 0;
}
