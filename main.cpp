#include<iostream>
#include<math.h>
#include<GL/glut.h>
#include "figuras.hpp"
#include "map.hpp"
#include<stdlib.h>
#define  ANCHO 800
#define ALTO 800
#include <fstream>
#include <vector>
#include <list>
#include <limits>     

 Minero Jack;
 GLuint KLtexture;

 float cam_pos[6]={0, -10, 10};
 
 void Keyboard(int key, int x, int y);
 
 
 void OnDibuja(void){
  

  //Borrado de la pantalla	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  
   
  
  
  
  Jack.drawMinero();
  
  displaytext(KLtexture);
 
  glLoadIdentity();
  // posicciona el punto de vista 
  gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],  // posicion del  ojo  
	    0.0, 0, 0.0,		        // hacia que punto mira  
	    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();//se le indica que redibuje la pantalla
  /**************************************************************/
  }

int main(int argc,char* argv[]){

  
  // Inicializaciones

  //Creacion y definicion de la ventana
  glutInit(&argc, argv);
  glutInitWindowSize(ANCHO,ALTO);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Ejemplo minero");

  //Habilitar las luces, la renderizacion y el color de los materiales
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
	
  //definir la proyeccion
  glMatrixMode(GL_PROJECTION);
  gluPerspective( 40.0, ANCHO/ALTO, 0.1, 50);
  
  //color del fondo
  glClearColor(1.0f, 0.31f, 0.9f, 1.0f);  

  // define call backs de GLUT
   glMatrixMode(GL_MODELVIEW);	
  glutDisplayFunc(OnDibuja);
  
  // La siguiente función sirve para definir teclas concretas del teclado
  glutSpecialFunc(Keyboard);
  
    
	
  //Para definir el punto de vista
 
  
 
  // posicciona el punto de vista 
  gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],  // posicion del  ojo  
	    0.0, 0, 0.0,		        // hacia que punto mira  
	    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)
	   
  // Carga la textura
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  KLtexture= LoadTexture(900,1074, "klondike.bmp" ); 
  glBindTexture (GL_TEXTURE_2D, KLtexture);

  // bucle del programa
  glutMainLoop();
  
  return 0;

}

  void Keyboard(int key, int x, int y){
     switch(key){
        case GLUT_KEY_DOWN:
           cam_pos[0]++; break;
        case GLUT_KEY_UP:
           cam_pos[0]--;break;
        case GLUT_KEY_RIGHT:
           cam_pos[1]++; break;
        case GLUT_KEY_LEFT:
           cam_pos[1]--;break;
     
     }
     }
  
