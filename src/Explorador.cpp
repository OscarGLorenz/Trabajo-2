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
#elif __unix__
#include <GL/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK
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

Klondike lab(MAPA); // Mapa klondike con resolvedor
MapGenerator map(&lab); // Carga de mapas y generación de mapas aleatorios
std::list<Point> solution; // Casillas de la solución
std::vector<SearchResult> v; // Pasos del algoritmo

Camera camera; // Control del punto de vista con el ratón

GLuint KLtexture; // Textura del mapa

Minero jack; // Minero, personaje que visita las casillas
Cuadro cuadros[23][23]; // Recuadros para marcar las casillas de colores
Plano base; // Plano auxiliar para evitar comportamientos raros con la cámara
bool visited[23][23]; // Casillas visitadas, auxiliar para la lógica de la GUI
Arco arco; // Arcos que marcan el camino a la solución

int ox = 11, oy = 11; // Casilla del mapa donde está el minero
bool instructions = false; // Mostrar instrucciones

bool exit_flag = true; // Variable auxiliar para cortar los hilos adicionales

int adv = 0; // Variable de control para mostrar los arcos

unsigned int k =0; // Control del modo algoritmo
bool algoritmmode = true; // Modo algoritmo activado

// Función para mostrar texto en la GUI
void font(void *font,unsigned char *text,int x,int y) {
  glRasterPos2i(x, y); // Posicón del texto
  while( *text != '\0' ) { // Representar los caracteres
    glutBitmapCharacter( font, *text );
    ++text;
  }
}

// Función que actualiza la pantalla
void onDraw(void) {
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  map.displayMap(KLtexture); // Mostrar mapa

  base.draw(0,0); // Dibujar base
  jack.draw(ox,oy); // Dibujar minero

  // Si estamos en modo algoritmo
  if(k >= 0 && k < v.size() && algoritmmode) {
    adv=0; // No mostrar solución
    ox=v[k].from.x; // Obtener nueva casilla
    oy=v[k].from.y;
    cuadros[ox][oy].setColor(Colors::GREEN); //Marca las casillas visitadas en verde
    // Itera por las casillas adyacentes
    for (auto iblue = v[k].adjacent.begin(); iblue!= v[k].adjacent.end(); ++iblue){
      // Si la casilla no estaba en verde actualizar si esta dentro del mapa
      if (lab.getMap(Point(iblue->x,iblue->y)) > 0 &&
      cuadros[iblue->x][iblue->y].getColor() != Colors::GREEN)
      cuadros[iblue->x][iblue->y].setColor(Colors::BLUE);
    }
  }
  // Sacar al minero del mapa en el último paso
  if (k == v.size() && algoritmmode) {
    ox=solution.rbegin()->x;
    oy=solution.rbegin()->y;
  }

  // Dibujar los cuadros
  for(int i=0; i<23; i++)
    for(int j=0; j<23; j++)
      cuadros[i][j].draw(i,j);

  arco.drawArc(solution,adv); // Dibujar los arcos

  camera.look2D(); // Cambiar a visión 2D

  // Dibujar los botones
  for (auto &b : Button::Buttons)
    b.draw();

  // Mostrar instrucciones
  if(instructions){
    // Fondo
    glColor3f(0.85f,0.85f,0.85f);
    glBegin(GL_QUADS);
    glVertex2i(50, 80);
    glVertex2i(50, 700);
    glVertex2i(750, 700);
    glVertex2i(750, 80);
    glEnd();

    // Marco
    glLineWidth(4);
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2i(50, 80);
    glVertex2i(50, 700);
    glVertex2i(750, 700);
    glVertex2i(750, 80);
    glVertex2i(50, 80);
    glEnd();

    // Texto
    glColor3f(0.0f,0.0f,0.0f);
    const char *c[]={ "INSTRUCCIONES-GUIA DE TECLAS",
    "VER SOLUCION: Muestra la solucion del mapa",
    "VER ALGORITMO: Muestra de forma automatica el proceso que sigue el",
    "algoritmo para resolver el mapa.",
    "MODO INTERACTIVO: Permite al usuario jugar y moverse por el tablero",
    "usando los botones de la pantalla.",
    "REINICIAR: Devuelve al minero a la posicion inicial.",
    "GENERAR MAPA NUEVO: Genera de forma aleatoria un mapa.",
    "INSTRUCCIONES: Muestra este menu.",
    " ",
    "PUNTO DE VISTA",
    "TRASLACION: Boton izquierdo del raton, mantener pulsado y arrastrar.",
    "ROTACION: Boton derecho del raton, mantener pulsado y arratrar.",
    "ACERCAR O ALEJAR: Rueda del raton.", "ESC: Cierra el programa." };
    for(int i=0; i< 14; i++)
      font(GLUT_BITMAP_HELVETICA_18, (unsigned char*)c[i] ,100 ,110+30*i);
  }

  camera.look3D(); // Pasar a modo 3D

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();
}

// Al pulsar un botón del ratón
void onMouseBtn(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    for (auto &b : Button::Buttons) {
      if(b.press(x,y)) // Mirar si se hace clic en un botón
      return;
    }

    camera.moveMode(true,x,y); // Mover la cámara
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    for (auto &b : Button::Buttons)
    b.release(x,y); // Ver si se ha soltado un botón

    camera.moveMode(false,x,y); // Dejar de mover la cámara
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    camera.rotateMode(true,x,y); // Rotar la cámara
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
    camera.rotateMode(false,x,y); // Dejar de rotar la cámara
  } else if (button == 4) {
    camera.zoom(1.1,20); // Disminuir el zoom
  } else if (button == 3) {
    camera.zoom(0.9,1); // Aumentar el zoom
  }

}

// Al mover el ratón pulsando algo, actualizar la cámara
void onMouseMove(int x, int y) {
  camera.mouseMove(x,y);
}

// Al mover el ratón, marcar botones
void onMotion(int x, int y) {
  for (auto& b : Button::Buttons) {
    b.motion(x,y);
  }
}

// Al pulsar la tecla ESC, cortar hilos adicionales y salir
void onKeyboardDown(unsigned char key, int x, int y) {
  if (key == ESC)    {
    exit_flag = false;
    exit(1);
  }
}

// Lógica adicional del programa
void myLogic(){
  // Girar el pico en un sentido o en otro alternativamente
  static int status = 1;

  if(jack.position[4]>=20 && status ==1) status=0;
  if(jack.position[4]<=-20 && status == 0) status=1;
  switch(status){
    case 1:
    jack.position[4]+=2.0; break;
    case 0:
    jack.position[4]-=2.0; break;
  }
}


int main(int argc,char* argv[]) {

  // Inicializaciones openGL
  //Creacion y definicion de la ventana
  glutInit(&argc, argv);
  glutInitWindowSize(ANCHO,ALTO);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("BACK FROM THE KLONDIKE");
  glEnable(GL_LIGHT0);
  //TRANSPARENCIAS
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fondo blanco

  camera.setAspect(ANCHO,ALTO); // Ajusta el ancho y el alto de la cámara
  camera.setPrespective(40.0, ANCHO/ALTO, 0.1, 50); // Ajusta la prespectiva
  camera.setPosition(0,-10,10); // Posición inicial de la cámara
  camera.setLimits(5.0); // Límite de movilidad de la cámara


  // Definición call backs de GLUT
  glutDisplayFunc(onDraw);
  glutKeyboardFunc(onKeyboardDown);
  glutMouseFunc(onMouseBtn);
  glutMotionFunc(onMouseMove);
  glutPassiveMotionFunc(onMotion);
  glutIdleFunc(myLogic);

  map.getCmdPath(argv);  // Actualizar ruta del ejecutable
  KLtexture= map.loadMap();  // Carga la textura

  // Carga las casillas con sus valores iniciales (no visitado)
  for(int i=0; i<23; i++){
    for(int j=0; j<23; j++){
      if(MAPA[i][j]<=0)
      cuadros[i][j].setColor(Colors::VOID);
      else
      cuadros[i][j].setColor(Colors::RED);
    }
  }

  // Resetea la GUI
  auto reset = [&]() {
    oy=ox=11; // Mueve el minero al centro
    for (int i = 0; i < 23; i++) {
      for (int j = 0; j < 23; j++) {
        visited[i][j] = false;
        if (cuadros[i][j].getColor() != Colors::VOID)
        cuadros[i][j].setColor(Colors::RED);
      }
    }
    solution.clear();
    adv = 0;
    k = 0;
    algoritmmode = false;
  };

  // Botón de reinicio, limpia los cuadros y elimina la solución
  Button::Buttons.push_back(Button(5,5, 100,25, "REINICIAR",reset));

  // Botón de generar nuevo mapa aleatorio
  Button::Buttons.push_back(Button(5,40, 150,25, "GENERAR MAPA NUEVO", [&](){
    map.random(10); // Genera un mapa con 10 movimientos para salir
    map.createMap(); // Crea la imagen del mapa
    KLtexture = map.loadMap();

    reset();
  }));

  // Muestra la solución al pulsar este botón
  Button::Buttons.push_back(Button(225,5, 100,25, "VER SOLUCION", [&](){
    static bool status = false;
    if (status)
    solution.clear();
    else {
      solution = lab.solve(Point(ox,oy),&v); // Buscar la solución
      adv = 0;
    }
    status = !status;
  }));

  // Botón para visualizar el desarrollo del algoritmo
  Button::Buttons.push_back(Button(335,5, 100,25, "VER ALGORITMO",[&](){
    algoritmmode= !algoritmmode;
    if(algoritmmode){
      solution = lab.solve(Point(ox,oy),&v);
      k=0;
    } else {
      reset();
    }
  }));

  // Botón para mostrar las instrucciones
  Button::Buttons.push_back(Button(690,5, 100,25, "INSTRUCCIONES", [&](){
    instructions= !instructions;
  }));

  // Función auxiliar para moverse por el mapa en una dirección en interactivo
  auto search_ady =  [&](Direction c){
    // Obtener casillas adyacentes
    std::list<Point> ady = lab.adjacent(Point(ox,oy));
    for (auto it = ady.begin(); it != ady.end(); ++it){
      if (it->dir == c) { // Si a alguna de las casillas se puede llegar
        // Mover muñeco
        ox = it->x;
        oy = it->y;
        visited[it->x][it->y] = true;
      }
    }
    for (int i = 0; i < 23; i++) {
      for (int j = 0; j < 23; j++) {
        if(visited[i][j]) // Si una casilla ha sido visitada poner en verde
        cuadros[i][j].setColor(Colors::GREEN);

        if (cuadros[i][j].getColor() == Colors::BLUE) // Si era azul ahora rojo
        cuadros[i][j].setColor(Colors::RED);
      }
    }
    ady = lab.adjacent(Point(ox,oy)); // Las casillas adyacentes en azul
    for (auto it = ady.begin(); it != ady.end(); ++it){
      cuadros[it->x][it->y].setColor(Colors::BLUE);
    }

  };

  // Activar modo interactivo, mostrar los botones de navegación
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
        search_ady(Direction::NORTHEAST);
      }));
    } else {
      Button::Buttons.erase(Button::Buttons.end()-=8, Button::Buttons.end());
    }
    interactive = !interactive;
  }));

  // Hilo para controlar los arcos
  std::thread advance([&](){
    while(exit_flag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      adv++;
    }
  });
  // Hilo para controlar el avance del algoritmo
  std::thread extra([&](){
    while(exit_flag) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      k++;
    }
  });

  reset();
  // bucle del programa
  glutMainLoop();

  return 0;
}
