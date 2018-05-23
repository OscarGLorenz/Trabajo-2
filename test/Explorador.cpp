/* Desplazarse por el mapa con:
  QWE
  A D
  ZXC

  ESC: Salir
*/

#include "GL/glut.h"

#include "Klondike.hpp"
#include "Mapa.h"

#include <fstream>
#include <vector>
#include <cmath>

#define  ANCHO 800
#define ALTO 800
#define W 900.0
#define H 1074.0

#define ESC 27
Klondike lab(MAPA);

// Texture datas structure
GLuint KLtexture;

// Casilla del mapa
int ox = 11, oy = 11;
double cam_pos[6]={0.0, -10.0f, 10.0f};
double zcam_pos[6]={0.0, -10.0f, 10.0f};

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

 //
 //   float r = sqrt(zcam_pos[0]*zcam_pos[0]+zcam_pos[1]*zcam_pos[1]);
 //   float a = zcam_pos[1]/r;
 //   float b = -zcam_pos[0]/r;
 //
 // cam_pos[0] = zcam_pos[0]*(cos(be)+a*a*(1-cos(be))) + zcam_pos[1]*a*b*(1-cos(be)) + zcam_pos[2]*b*sin(be);
 // cam_pos[1] =  zcam_pos[0]*a*b*(1-cos(be)) + zcam_pos[1]*(cos(be)+b*b*(1-cos(be))) - zcam_pos[2]*a*sin(be);
 // cam_pos[2] = zcam_pos[0]*- b*sin(be) + zcam_pos[1]*a*sin(be) + zcam_pos[2]*cos(be);

  // Buscar si es un movimiento posible
  for (std::list<Point>::const_iterator it = ady.begin(); it != ady.end(); ++it){
	   if (it->dir == c) {
      // Mover muñeco
		  ox = it->x;
		  oy = it->y;
	 }
  }
}

void drawSnowMan(float x, float y, float z) {
  glColor3f(1.0f, 0 , 1.0f); // Color RGB
  glEnable(GL_LIGHTING);
  glPushMatrix(); // Crear matriz
  glTranslatef(x,y,z); // Mover muñeco
  glutSolidSphere(0.15f,20,20); // Dibujar esfera
  glPopMatrix(); // Eliminar matriz
  glDisable(GL_LIGHTING);

}

// Carga la textura
GLuint LoadTexture( int width, int height, const char * filename ) {
  GLuint texture;

  std::vector<char> data(3*W*H); // Mapa de bits
  std::fstream file; // Archivo
  file.open ("graphics/klondike.bmp", std::fstream::in | std::fstream::binary);

  file.seekg(139); // Saltar cabecera
  for(int j=0; j<height; j++) {
    for(int i=0; i<width; i++) {
      // Copiar colores BGR
      data[(i+j*width)*3+2] = file.get();
      data[(i+j*width)*3+1] = file.get();
      data[(i+j*width)*3+0] = file.get();
      file.seekg((4-(width*3)%4)%4+1, file.cur);
    }
  }

  file.close(); // Cerrar

  // **************Magia*****************
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data.data());
  // **************Magia*****************

  return texture;
}

// Muestra el mapa
void displaytext(GLuint Thetexture) {
  // **************Magia*****************
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, Thetexture);
   glBegin(GL_QUADS);
   // **************Magia*****************

   // Esquinas del mapa
   glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0*H/W, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0*H/W, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0*H/W, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0*H/W, 0.0);

   glEnd();
   glFlush();
   glDisable(GL_TEXTURE_2D);
}


bool zoom = false, rotate = false,move=false; int mx, my;
double sx, sy, sz;
void drawCube(float x, float y, float z, float R, float G, float B, float size) {
x*=size;
y*=size;
z*=size;
//Multi-colored side - FRONT
glBegin(GL_POLYGON);
 glColor3f(R,G,B);
glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );      // P1 is red
glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );      // P2 is green
glVertex3f( x-size*0.5,  y+size*0.5, z-size*0.5 );      // P3 is blue
glVertex3f( x-size*0.5, y-size*0.5, z-size*0.5 );      // P4 is purple

glEnd();

glBegin(GL_POLYGON);
glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5,  y+size*0.5, z+size*0.5 );
glVertex3f( x-size*0.5,  y+size*0.5, z+size*0.5 );
glVertex3f( x-size*0.5, y-size*0.5, z+size*0.5 );
glEnd();

// Purple side - RIGHT
glBegin(GL_POLYGON);
glVertex3f( x+size*0.5, y-size*0.5, z-size*0.5 );
glVertex3f( x+size*0.5,  y+size*0.5, z-size*0.5 );
glVertex3f( x+size*0.5,  y+size*0.5,  z+size*0.5 );
glVertex3f( x+size*0.5, y-size*0.5,  z+size*0.5 );
glEnd();

// Green side - LEFT
glBegin(GL_POLYGON);
glVertex3f( x-size*0.5, y-size*0.5,  z+size*0.5 );
glVertex3f( x-size*0.5,  y+size*0.5,  z+size*0.5 );
glVertex3f( x-size*0.5,  y+size*0.5, z-size*0.5 );
glVertex3f( x-size*0.5, y-size*0.5, z-size*0.5 );
glEnd();

// Blue side - TOP
glBegin(GL_POLYGON);
glVertex3f(  x+size*0.5,  y+size*0.5,  z+size*0.5 );
glVertex3f(  x+size*0.5,  y+size*0.5, z-size*0.5 );
glVertex3f( x-size*0.5,  y+size*0.5,z-size*0.5 );
glVertex3f( x-size*0.5,  y+size*0.5,  z+size*0.5 );
glEnd();

// Red side - BOTTOM
glBegin(GL_POLYGON);
glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );
glVertex3f(  x+size*0.5, y-size*0.5, z+ size*0.5 );
glVertex3f( x-size*0.5,y-size*0.5,  z+size*0.5 );
glVertex3f( x-size*0.5, y-size*0.5, z-size*0.5 );
glEnd();
glFlush();
glColor3f(0,0,0 );
glLineWidth(1.5);
glBegin(GL_LINES);
glVertex3f(  x+size*0.5, y+size*0.5, z+size*0.5 );
glVertex3f(  x-size*0.5, y+size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5, y+size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5, y+size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );

glVertex3f(  x-size*0.5, y-size*0.5, z-size*0.5 );
glVertex3f(  x-size*0.5, y-size*0.5, z+size*0.5 );
glVertex3f(  x-size*0.5, y-size*0.5, z-size*0.5 );
glVertex3f(  x-size*0.5, y+size*0.5, z-size*0.5 );
glVertex3f(  x-size*0.5, y-size*0.5, z-size*0.5 );
glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );

glVertex3f(  x-size*0.5, y+size*0.5, z+size*0.5 );
glVertex3f(  x-size*0.5, y+size*0.5, z-size*0.5 );
glVertex3f(  x-size*0.5, y+size*0.5, z+size*0.5 );
glVertex3f(  x-size*0.5, y-size*0.5, z+size*0.5 );

glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );
glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );
glVertex3f(  x+size*0.5, y+size*0.5, z-size*0.5 );
glVertex3f(  x-size*0.5, y+size*0.5, z-size*0.5 );

glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
glVertex3f(  x-size*0.5, y-size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5, y-size*0.5, z+size*0.5 );
glVertex3f(  x+size*0.5, y-size*0.5, z-size*0.5 );
glEnd();


glFlush();

}
// Funci�n que actualiza la pantalla
void OnDibuja(void) {
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // proyeccion de las figuras

  displaytext(KLtexture);
  drawCube(0,0,0, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(1,0,0, 0.3f, 0.15f, 0.05f, 0.05);

  drawCube(0,0,1, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(1,0,1, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(2,0,1, 0.3f, 0.15f, 0.05f, 0.05);

  drawCube(1,0,2, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(2,0,2, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(3,0,2, 0.3f, 0.15f, 0.05f, 0.05);

  drawCube(2,0,3, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(3,0,3, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(4,0,3, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(11,0,3,0.055f, 0.247f, 0.212f, 0.05);



  drawCube(3,0,4, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(4,0,4, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(5,0,4, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(10,0,4,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(11,0,4,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(12,0,4,0.055f, 0.247f, 0.212f, 0.05);

  drawCube(4,0,5, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(5,0,5, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(6,0,5, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(10,0,5,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(11,0,5,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(12,0,5,0.055f, 0.247f, 0.212f, 0.05);

  drawCube(5,0,6, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(6,0,6, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(7,0,6, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(10,0,6,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(11,0,6,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(12,0,6,0.055f, 0.247f, 0.212f, 0.05);

  drawCube(6,0,7, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(7,0,7, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(8,0,7, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(10,0,7,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(11,0,7,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(12,0,7,0.055f, 0.247f, 0.212f, 0.05);

  drawCube(7,0,8, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(8,0,8, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(9,0,8, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(10,0,8,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(11,0,8,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(12,0,8,0.055f, 0.247f, 0.212f, 0.05);

  drawCube(8,0,9, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(9,0,9,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(10,0,9,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(11,0,9,0.055f, 0.247f, 0.212f, 0.05);

  drawCube(4,0,10,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(5,0,10,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(6,0,10,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(7,0,10,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(8,0,10,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(9,0,10,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(10,0,10, 0.5f, 0.35f, 0.05f, 0.05);
  drawCube(11,0,10, 0.3f, 0.15f, 0.05f, 0.05);

  drawCube(3,0,11,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(4,0,11,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(5,0,11,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(6,0,11,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(7,0,11,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(8,0,11,0.2f, 0.922f, 0.796f, 0.05);
  drawCube(9,0,11,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(10,0,11, 0.3f, 0.15f, 0.05f, 0.05);
  drawCube(11,0,11, 0.5f, 0.35f, 0.05f, 0.05);

  drawCube(4,0,12,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(5,0,12,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(6,0,12,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(7,0,12,0.055f, 0.247f, 0.212f, 0.05);
  drawCube(8,0,12,0.055f, 0.247f, 0.212f, 0.05);



  drawSnowMan(-0.144f+(oy-11.0)*0.422,-0.190f+(11.0-ox)*0.422,0);

  glBegin(GL_LINE_STRIP);
  glLineWidth(2);

  glColor3f(1.0f,0.0f,0.0f);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(10.0f,0.0f,0.0f);
  glColor3f(0.0f,1.0f,0.0f);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,10.0f,0.0f);
  glColor3f(0.0f,0.0f,1.0f);
  glVertex3f(0.0f,0.0f,0.0f);
  glVertex3f(0.0f,0.0f,10.0f);
  glEnd();

  glLoadIdentity();

  // posicciona el punto de vista
  gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],  // posicion del  ojo
	  cam_pos[3], cam_pos[4],cam_pos[5],		        // hacia que punto mira
	    0.0, 0.0, 1.0);         // vector "UP"  (vertical positivo)

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();//se le indica que redibuje la pantalla
}

void OnMouseBtn(int button, int state, int x, int y) {

   if (state != GLUT_UP) {
     if (button == 3) {
       zcam_pos[0] = cam_pos[0] *= (1+0.1);
			 zcam_pos[1] =cam_pos[1] *= (1+0.1);
			 zcam_pos[2] =cam_pos[2] *= (1+0.1);
     } else if (button == 4)  {
       zcam_pos[0] =cam_pos[0] *= (1-0.1);
       zcam_pos[1] =cam_pos[1] *= (1-0.1);
      zcam_pos[2] = cam_pos[2] *= (1-0.1);
     }
   }
   if (button == 2 && state == 0) {
 		rotate = true;
 		mx = x;
 		my = y;
 	}
  if (button == 2 && state == 1) {
 		zcam_pos[0] = cam_pos[0];
 		zcam_pos[1] = cam_pos[1];
 		zcam_pos[2] = cam_pos[2];
 		rotate = false;
 	}
  if (button == 0 && state == 0 && 0) {
    double ox, oy, oz;
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;
    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
    glutPostRedisplay();

    zcam_pos[3] = cam_pos[3] = ox;
    zcam_pos[4] =cam_pos[4] = oy;
    zcam_pos[5] =cam_pos[5] = oz;
  }


}

void OnMouseMove(int x, int y) {
  if (rotate) {
      float al = (float) (mx-x)/200.0;
       float be = (float) (my-y)/200.0;
       float r = sqrt(zcam_pos[0]*zcam_pos[0]+zcam_pos[1]*zcam_pos[1]);
       float a = (float)zcam_pos[1]/r;
       float b = -(float)zcam_pos[0]/r;

       cam_pos[0] = (zcam_pos[0])*(cos(be)+a*a*(1-cos(be))) + (zcam_pos[1])*a*b*(1-cos(be))           + (zcam_pos[2])*b*sin(be);
       cam_pos[1] = (zcam_pos[0])*a*b*(1-cos(be))           + (zcam_pos[1])*(cos(be)+b*b*(1-cos(be))) - (zcam_pos[2])*a*sin(be);
       cam_pos[2] = (zcam_pos[0])*- b*sin(be)               + (zcam_pos[1])*a*sin(be)                 + (zcam_pos[2])*cos(be);

       cam_pos[0] = cam_pos[0]*cos(al)+cam_pos[1]*-sin(al);
       cam_pos[1] = cam_pos[0]*sin(al)+cam_pos[1]*cos(al);

  }
  if (move) {


  }
}

int main(int argc,char* argv[]) {

  // Inicializaci�n GLUT
  glutInit(&argc, argv);

  // Inicializaciones openGL
  //Creacion y definicion de la ventana

  glutInitWindowSize(ANCHO,ALTO);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Ejemplo GLUT");

 //glutLeaveGameMode();

  //Habilitar las luces, la renderizacion y el color de los materiales
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
	//glutFullScreen();

  //definir la proyeccion
  glMatrixMode(GL_PROJECTION);
  gluPerspective( 40.0, 1027.0/768.0, 0.1, 50);

  // color del fondo
  glClearColor(0.5f, 0.91f, 0.9f, 0.5f);

  // Define el punto de vista
  glMatrixMode(GL_MODELVIEW);

  // Definici�n call backs de GLUT
  glutDisplayFunc(OnDibuja);
  glutKeyboardFunc(OnKeyboardDown);
 glutMouseFunc(OnMouseBtn);
 glutMotionFunc(OnMouseMove);
  // Carga la textura
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  KLtexture= LoadTexture(900,1074, "graphics/klondike.bmp" );
  glBindTexture (GL_TEXTURE_2D, KLtexture);

  // bucle del programa
  glutMainLoop();

  return 0;
}
