/* Desplazarse por el mapa con:
  QWE
  A D
  ZXC

  ESC: Salir
*/

#include "GL/glut.h"

#include "src/Klondike.hpp"
#include "src/Mapa.h"

#include <fstream>
#include <vector>

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
      // Mover mu√±eco
		  ox = it->x;
		  oy = it->y;
	 }
  }
}

void drawSnowMan(float x, float y, float z) {
  glColor3f(1.0f, 0 , 1.0f); // Color RGB
  glPushMatrix(); // Crear matriz
  glTranslatef(x,y,z); // Mover mu√±eco
  glutSolidSphere(0.15f,20,20); // Dibujar esfera
  glPopMatrix(); // Eliminar matriz
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

// FunciÛn que actualiza la pantalla
void OnDibuja(void) {
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // proyeccion de las figuras
  displaytext(KLtexture);
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

int main(int argc,char* argv[]) {

  // InicializaciÛn GLUT
  glutInit(&argc, argv);

  // Inicializaciones openGL
  //Creacion y definicion de la ventana

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
  glClearColor(0.5f, 0.91f, 0.9f, 0.5f);

  // Define el punto de vista
  glMatrixMode(GL_MODELVIEW);

  // DefiniciÛn call backs de GLUT
  glutDisplayFunc(OnDibuja);
  glutKeyboardFunc(OnKeyboardDown);

  // Carga la textura
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  KLtexture= LoadTexture(900,1074, "graphics/klondike.bmp" );
  glBindTexture (GL_TEXTURE_2D, KLtexture);

  // bucle del programa
  glutMainLoop();

  return 0;
}
