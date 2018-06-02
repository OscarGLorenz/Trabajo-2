  /*************************************************************
  * 
  * ARCHIVO : main.cpp
  * 
  * DESCRIPCIÓN: 
  * 	Contiene el main del programa, a lo largo de todo el 
  * 	fichero se hacen llamadas a funciones de OpenGl 
  *     y se definen funciones Callbacks
  *
  * AUTORES:
  * 
  * 	Alejandro Redondo Ayala
  *	   
  *     Óscar García Lorenz
  *	   Implementación de las funciones que permiten cambiar
  * 	   el punto de vista con el ratón.
  *
  *************************************************************/




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
  
  
  
  
  
  /**************COSAS PARA LA CAMARA**************************/
  #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

   GLdouble tras[3] = {0,0,0}; // Traslación actual
   GLdouble pos[3] = {0,-10,10}; // Posición actual
   GLdouble oldPos[3] = {0,-10,10}; // Posición anterior
   GLdouble oldTras[3] = {0,0,0}; // Traslación anterior
   int oldX = ANCHO/2, oldY = ALTO/2; // Posición del cursor anterior

   // Modos rotación y movimientos con el ratón
   bool rotate = false, move= false;
   /**************COSAS PARA LA CAMARA**************************/
  
  
  
  
  
  
  Minero Jack; 
   
  Cuadro Azul; 
  GLuint KLtexture;
  Plano Base;
  
  //PRUEBAS MOV
  // Casilla del mapa
  int ox = 11, oy = 11,oxblue,oyblue;
  unsigned int k=-1;
  int i,j;


  //PRUEBAS MOV

  Klondike lab(MAPA);
  std::vector<SearchResult> v;
  //OBTENEMOS LA LISTA CON LA SOLUCIÓN
  std::list<Point> solution = lab.solve(Point((SIZE-1)/2,(SIZE-1)/2),&v);
  //SE DEFINE ITERADOR PARA OPERAR CON LA LISTA
  std::list<Point>::const_iterator it= solution.begin();
 
  /*****************MATRIZ DE DIBUJAR CASILLAS********************/
 
  Colors Casillas[23][23];
 
  /******* MODO SOLUCION ****/
  
  /*
  //OBTENCION DE LOS ADYACENTES
  std::list<Point> ady = lab.adyacent(Point (ox,oy));
 
  std::list<Point>::const_iterator iblue;
  */
  
  /******* MODO SOLUCION ******/

  float cam_pos[3]={-0, -5, 5};

  void Keyboard(int key, int x, int y);
  void Keys(unsigned char tecla,int x,int y);
 
 
  void OnDibuja(void){
  

     //Borrado de la pantalla	
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
    //SE DIBUJA EL MAPA
   
     displaytext(KLtexture);
     Base.draw(ox,oy);
  
 
  
  //SE DIBUJAN LOS CUADROS ADYACENTES
  
  
  
    
  //SE DIBUJA EL MINERO
  
  
  
  
 /* PRUEBA DE DIBUJO A PARTIR DE SearchResult
  ox=v[k].from.x;
  oy=v[k].from.y;
  std::list<Point>::const_iterator iblue;
    for (iblue = v[k].adyacent.begin(); iblue!= v[k].adyacent.end(); ++iblue){
  Azul.drawCuadro(iblue->x,iblue->y,Colors::BLUE);
  
  
  }*/
  
  
    
  
     for(i=0;i<23;i++){
        for(j=0;j<23;j++){
           Azul.setColor(Casillas[i][j]);
           Azul.draw(i,j);
        }
     }
   
     
 
  
  
     Jack.draw(ox,oy);
     
     
     /* ARCOS FINALES 
     glDisable(GL_LIGHTING);
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
     glEnable(GL_LIGHTING);
     
  */
 
  
       
       
       
       
       
     glLoadIdentity();
     // posicciona el punto de vista 
     
     
     
      gluLookAt(pos[0]+tras[0],pos[1]+tras[1],pos[2]+tras[2],
                tras[0],tras[1],tras[2],          // hacia que punto mira
                0.0, 0.0, 1.0);         // vector "UP"  (vertical positivo)
   

     //Al final, cambiar el buffer
     glutSwapBuffers();
     glutPostRedisplay();//se le indica que redibuje la pantalla
  
  }
  
  
  
  
  
  /**************COSAS PARA LA CAMARA**************************/
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

  
  /**************COSAS PARA LA CAMARA**************************/


  void myLogic();
 
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
     glutIdleFunc(myLogic);
  
     // La siguiente función sirve para definir teclas concretas del teclado
     glutSpecialFunc(Keyboard);
     //La siguienter función sirve para definir el resto de teclas 
     glutKeyboardFunc(Keys);
     glutMouseFunc(OnMouseBtn);
     glutMotionFunc(OnMouseMove);

    
	
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
        case GLUT_KEY_RIGHT:
           cam_pos[0]++; break;
        case GLUT_KEY_LEFT:
           cam_pos[0]--; break;
        case GLUT_KEY_UP:
           cam_pos[1]++; break;
        case GLUT_KEY_DOWN:
           cam_pos[1]--;break;
           
     
     }
  }
  
  void Keys( unsigned char tecla, int x, int y){
     switch(tecla){
        case 13:
           k++;
           
           /****** MODO SOLUCION ******/
          
          /*
           std::list<Point> ady = lab.adyacent(Point (ox,oy));
              std::list<Point>::const_iterator iblue;
             
              for (iblue = ady.begin(); iblue!= ady.end(); ++iblue){
                 Casillas[iblue->x][iblue->y] = Colors::RED;
               
          }
           if(it++,it != sol.end()){
            
            ox=it->x;
            oy= it->y; 
             }
              ady = lab.adyacent(Point (ox,oy));
             
             
              for (iblue = ady.begin(); iblue!= ady.end(); ++iblue){
                 Casillas[iblue->x][iblue->y] = Colors::BLUE;
                 
               
          }
         */
            
           /****** MODO SOLUCION ******/
           
           
           /****** MODO ALGORITMO******/
        /***********PRUEBA DIBUJO CASILLAS CON MATRIZ************/
          
          ox=v[k].from.x;
          oy=v[k].from.y;
          Casillas[ox][oy]= Colors::GREEN; //Marca las casillas visitadas en verde
          std::list<Point>::const_iterator iblue;
         for (iblue = v[k].adyacent.begin(); iblue!= v[k].adyacent.end(); ++iblue){
              Casillas[iblue->x][iblue->y] = Colors::BLUE;
           }
           
        /***********PRUEBA DIBUJO CASILLAS CON MATRIZ************/
           /****** MODO ALGORITMO ******/  
          
        break;
     
     }
  }
  
    
 int status = 1;
  void myLogic(){
  
  if(Jack.position[4]>=20 && status ==1) status=0;
  if(Jack.position[4]<=-20 && status == 0) status=1;
   switch(status){
      case 1:
          Jack.position[4]+=2.0; break;
      case 0:
          Jack.position[4]-=2.0; break;    
   
   
   }
     
   
                  
     
  }
 


  
  
