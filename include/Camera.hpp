#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

// Limita una variable entre un máximo y un mínimo. Arduino
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class Camera {
public:
  void look3D();
  void look2D();

  void setAspect(int witdh, int height);
  void setPosition(double x, double y, double z);
  void setTraslation(double x, double y, double z);
  void setPrespective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

  void moveMode(bool enable, int x, int y);
  void rotateMode(bool enable, int x, int y);
  void zoom(double zoom, double limit);

  void mouseMove(int x, int y);

private:
  void screenToWorld(int x, int y, GLdouble *outX, GLdouble *outY, GLdouble *outZ);

  GLdouble tras[3]; // Traslación actual
  GLdouble pos[3]; // Posición actual
  GLdouble oldPos[3]; // Posición anterior
  GLdouble oldTras[3]; // Traslación anterior
  int oldX =0, oldY =0; // Posición del cursor anterior
  bool rotate = false, move= false;

  GLdouble prespective[4];

  GLdouble width, height;
};

#endif
