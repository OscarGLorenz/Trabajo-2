/******************************************************************************
* ARCHIVO :        Camera.hpp
*
* DESCRIPCIÓN :
*       Clase para manipular la cámara con el ratón, girar, moverse y zoom
*
* NOTA:
*       Documentación de uso de las funciones en la cabecera (include/)
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/


#include "Camera.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

void Camera::setAspect(int w, int h) {
  width = w;
  height = h;
}
void Camera::setPrespective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
  prespective[0] = fovy;
  prespective[1] = aspect;
  prespective[2] = zNear;
  prespective[3] = zFar;
}

void Camera::setPosition(double x, double y, double z) {
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
}

void Camera::setTraslation(double x, double y, double z) {
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
}

void Camera::zoom(double zoom, double limit) {
  // Multiplica la posición solo si no se pasa de los límites
  if((zoom > 1.0 && zoom*sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]) <= limit) ||
  (zoom < 1.0 && zoom*sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]) >= limit)) {
    oldPos[0] = pos[0] *= zoom;
    oldPos[1] = pos[1] *= zoom;
    oldPos[2] = pos[2] *= zoom;
  }
}

void Camera::moveMode(bool enable, int x, int y) {
  move = enable;
  if (enable) {
    screenToWorld(x, y, &oldTras[0], &oldTras[1], &oldTras[2]);
    // Limitar para no salirse del mapa
    oldTras[0] = constrain(oldTras[0],-limitMove,limitMove);
    oldTras[1] = constrain(oldTras[1],-limitMove,limitMove);
    oldTras[2] = 0;
  }
  oldX = x;
  oldY = y;
}

void Camera::rotateMode(bool enable, int x, int y){
  rotate = enable;
  if (enable) {
    oldPos[0] = pos[0];
    oldPos[1] = pos[1];
    oldPos[2] = pos[2];
  }
  oldX = x;
  oldY = y;
}

void Camera::mouseMove(int x, int y) {
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

void Camera::look3D() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 40.0, width/height, 0.1, 50);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Posiciona el punto de vista
  gluLookAt(pos[0]+tras[0],pos[1]+tras[1],pos[2]+tras[2],tras[0],tras[1],tras[2], 0.0, 0.0, 1.0);
}

void Camera::look2D() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,width,height,0,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Transforma la posición del cursos en la pantalla a la posición en el mundo
void Camera::screenToWorld(int x, int y, GLdouble *outX, GLdouble *outY, GLdouble *outZ) {
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

void Camera::setLimits(double limit) {
  limitMove = limit;
}
