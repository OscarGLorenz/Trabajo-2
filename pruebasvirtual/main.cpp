#include<iostream>
#include<math.h>
#include<GL/glut.h>
#include "figuras.hpp"
#include "map.hpp"
//#include<stdlib.h>
#define  ANCHO 800
#define ALTO 800
#include <fstream>
#include <vector>
#include <list>
#include <limits>     


//Pruebas mov

#include "src/Klondike.hpp"
#include "Mapa.h"
//Pruebas mov
//LIBRERIAS TIEMPO
 #include <chrono>
#include <thread>
//LIBRERIAS TIEMPO
 Minero Jack;
 //SE DEFINEN LOS CUADROS DE COLORES (OBJETOS)
  //Cuadro Rojo;
  Cuadro Azul;
 GLuint KLtexture;

  
//PRUEBAS MOV
// Casilla del mapa
int ox = 11, oy = 11,oxblue,oyblue;
unsigned int k=-1;
int i,j;


//PRUEBAS MOV

 Klondike lab(MAPA);
 std::vector<SearchResult> v;
 //OBTENEMOS LA LISTA CON LA SOLUCIÓN
 std::list<Point> sol = lab.solve(Point((SIZE-1)/2,(SIZE-1)/2),&v);
 //SE DEFINE ITERADOR PARA OPERAR CON LA LISTA
 std::list<Point>::const_iterator it= sol.begin();
 
 /*****************MATRIZ DE DIBUJAR CASILLAS********************/
 
  Colors Casillas[23][23];
 
 
 /*
 //BORRAR
 //OBTENCION DE LOS ADYACENTES
 std::list<Point> ady = lab.adyacent(Point (ox,oy));
 
 std::list<Point>::const_iterator iblue;
 
 */
 

 float cam_pos[3]={-0, -10, 10};

 void Keyboard(int key, int x, int y);
 void Keys(unsigned char tecla,int x,int y);
 
 
 void OnDibuja(void){
  

  //Borrado de la pantalla	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    //SE DIBUJA EL MAPA
   
    
  /************************************************************************/
  //PRUEBAS DE CUADROS DE COLORES
   /*  glDisable(GL_LIGHTING);
   
   //prueba cuadrado
   glPushMatrix();
   glColor4f(1.0f,0.0f,0.0f,0.5f);
    
    glBegin(GL_POLYGON);
    
    glVertex3f(0.035f+(11-11.0)*0.422, 0.0f+(11.0-11)*0.422,0.05);
    glVertex3f(0.035f+(11-11.0)*0.422, -0.415f+(11.0-11)*0.422,0.05);
    glVertex3f(-0.38f+(11-11.0)*0.422, -0.415f+(11.0-11)*0.422,0.05);
    glVertex3f(-0.38f+(11-11.0)*0.422, 0.0f +(11.0-11)*0.422,0.05);
    glEnd();
    glPopMatrix(); */
    
    /*
    for(i=0;i<22;i++){
       for(j=0;j<22;++j){
    glPushMatrix();
   glColor4f(0.0f,1.0f,0.0f,0.5f);
    
    glBegin(GL_POLYGON);
    
    glVertex3f(0.035f+(i-11.0)*0.422, 0.0f+(11.0-j)*0.422,0.01);
    glVertex3f(0.035f+(i-11.0)*0.422, -0.415f+(11.0-j)*0.422,0.01);
    glVertex3f(-0.380f+(i-11.0)*0.422, -0.415f+(11.0-j)*0.422,0.01);
    glVertex3f(-0.380f+(i-11.0)*0.422, 0.0f +(11.0-j)*0.422,0.01);
    glEnd();
    glPopMatrix();
     
    }
    }
  //prueba cuadrado*/
  //glEnable(GL_LIGHTING);
  /******************************no*********************************************/
  
  //SE DIBUJA EL CUADRO ROJO
  //Rojo.drawCuadro(10,10,Colors::RED);
  
 
  
  //SE DIBUJAN LOS CUADROS ADYACENTES
  /*BORRAR
  std::list<Point> ady = lab.adyacent(Point (ox,oy));
  std::list<Point>::const_iterator iblue;
    for (iblue = ady.begin(); iblue!= ady.end(); ++iblue){
  Azul.drawCuadro(iblue->x,iblue->y,Colors::BLUE);
    
    }*/
  //SE DIBUJA EL MINERO
  
  
  

 /* PRUEBA DE DIBUJO A PARTIR DE SearchResult
  ox=v[k].from.x;
  oy=v[k].from.y;
  std::list<Point>::const_iterator iblue;
    for (iblue = v[k].adyacent.begin(); iblue!= v[k].adyacent.end(); ++iblue){
  Azul.drawCuadro(iblue->x,iblue->y,Colors::BLUE);
  
  
  }*/
  
  /***********PRUEBA DIBUJO DE CASILLAS A PARTIR DE MATRIZ************/
   displaytext(KLtexture);
 
  for(i=0;i<23;i++){
     for(j=0;j<23;j++){
         Azul.setColor(Casillas[i][j]);
         Azul.draw(i,j);
     }
  }
  Jack.setColor(Colors::ORANGED);
  Jack.draw(ox,oy);
   
  

  //PRUEBAS MOV
 
  
  
  
  
  
  //PRUEBAS MOV
  //PRUEBAS VECTOR DE SOLUCIONES
  
  
  
  /***************************************************************************
  PRUEBA
   
    //Write Footnote 
  glColor3f (1.0F, 0.0F, 0.0F); 
  
  glRasterPos3f (-4.5, -5.0,1.0); 
  glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 54); 
  
   
   
   
   PRUEBA
   ****************************************************************************/
   
  glLoadIdentity();
  // posicciona el punto de vista 
  gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],  // posicion del  ojo  
	    0.0, 0, 0.0,		        // hacia que punto mira  
	    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();//se le indica que redibuje la pantalla
  
  }

 //void myLogic();
 
int main(int argc,char* argv[]){


  for(i=0;i<23;i++){
     for(j=0;j<23;j++){
        if(MAPA[i][j]<=0){
         Casillas[i][j]=Colors::VOID;
         }
        else{
        Casillas[i][j]=Colors::RED;
        }
     }
  } 



  /***********************************************************************
  //Resolver mapa
 
  for(i=0;i<285;i++){
   ox=v[i].from.x;
  oy=v[i].from.y;
  
  std::cout<< ox<< "   "<< oy<< std::endl;
  }*/
  /*
  std::list<Point>::const_iterator it;
  for (it = sol.begin(),i=0; it != sol.end(); ++it,i++){
    a[i].x=it->x;
    a[i].y= it->y; 
    
   // std::this_thread::sleep_for(std::chrono::seconds(2));
  }*************************************************************************/
 
  
  
  
  
  // Inicializaciones

  //Creacion y definicion de la ventana
  glutInit(&argc, argv);
  glutInitWindowSize(ANCHO,ALTO);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Ejemplo minero");

  //Habilitar las luces, la renderizacion y el color de los materiales
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  //TRANSPARENCIAS
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
  //definir la proyeccion
  glMatrixMode(GL_PROJECTION);
  gluPerspective( 40.0, ANCHO/ALTO, 0.1, 50);
  
  //color del fondo
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  

  // define call backs de GLUT
   glMatrixMode(GL_MODELVIEW);	
      
  glutDisplayFunc(OnDibuja);
  //glutIdleFunc(myLogic);
  
  // La siguiente función sirve para definir teclas concretas del teclado
  glutSpecialFunc(Keyboard);
  //La siguienter función sirve para definir el resto de teclas 
  glutKeyboardFunc(Keys);
    
	
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
  void Keys( unsigned char tecla, int x, int y){
     switch(tecla){
        case 13:
           k++;
         /*  if(it++,it != sol.end()){
            
            ox=it->x;
            oy= it->y; */
            
        /***********PRUEBA DIBUJO CASILLAS CON MATRIZ************/
          ox=v[k].from.x;
          oy=v[k].from.y;
          Casillas[ox][oy]= Colors::GREEN; //Marca las casillas visitadas en verde
          std::list<Point>::const_iterator iblue;
         for (iblue = v[k].adyacent.begin(); iblue!= v[k].adyacent.end(); ++iblue){
              Casillas[iblue->x][iblue->y] = Colors::BLUE;
           }
        /***********PRUEBA DIBUJO CASILLAS CON MATRIZ************/
           
          
           break;
     
     }
     }
    
 /* void myLogic(){
    
     
  
  
  std::list<Point> ady = lab.adyacent(Point (ox,oy));
  std::list<Point>::const_iterator iblue;
  for (iblue = ady.begin(); iblue!= ady.end(); ++iblue){
    oxblue=iblue->x;
    oyblue=iblue->y;
    std::cout<< iblue->x << "   "<< iblue->y << std::endl;
    }
  
  }*/
  
  
  
