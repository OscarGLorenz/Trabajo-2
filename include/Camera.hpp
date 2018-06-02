/******************************************************************************
* ARCHIVO :        Camera.hpp
*
* DESCRIPCIÓN :
*       Clase para manipular la cámara con el ratón, girar, moverse y zoom
*       Esta clase para entenderla correctamente hay que ver como se utiliza
*       en el main
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glut.h"
#endif

// Limita una variable entre un máximo y un mínimo. Arduino
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

class Camera {
public:
  /*
   * Función: look3D
   * --------------------------------------------------------
   *   Pasa a modo 3D, con la prespectiva y posición definida en la clase
   */
  void look3D();
  /*
   * Función: look2D
   * --------------------------------------------------------
   *   Pasa a modo 2D, para dibujar objetos como botones
   */
  void look2D();

  /*
   * Función: setAspect
   * --------------------------------------------------------
   *   Cambia el ancho y el alto de la pantalla
   */
  void setAspect(int width, int height);

  /*
   * Función: setPosition
   * --------------------------------------------------------
   *   Cambia la posición relativa de la cámara
   */
  void setPosition(double x, double y, double z);

  /*
   * Función: setTraslation
   * --------------------------------------------------------
   *   Traslada el punto de visión de la cámara
   */
  void setTraslation(double x, double y, double z);

  /*
   * Función: setPrespective
   * --------------------------------------------------------
   *   Cambia la prespectiva, mismos argumentos que los de GL
   */
  void setPrespective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

  /*
   * Función: moveMode
   * --------------------------------------------------------
   *   Activa el modo de mover la cámara, x e y coordenadas del cursor
   */
  void moveMode(bool enable, int x, int y);

  /*
   * Función: rotateMode
   * --------------------------------------------------------
   *   Activa el modo de rotar la cámara, x e y coordenadas del cursor
   */
  void rotateMode(bool enable, int x, int y);

  /*
   * Función: zoom
   * --------------------------------------------------------
   *   Hacer zoom a la cámara, con el límite dado
   */
  void zoom(double zoom, double limit);

  /*
   * Función: mouseMove
   * --------------------------------------------------------
   *   Actualiza la posición del cursor
   */
  void mouseMove(int x, int y);

  /*
   * Función: mouseMove
   * --------------------------------------------------------
   *   Actualiza la posición del cursor y ejecuta las transformaciones
   */
  void setLimits(double limit);

private:
  /*
   * Función: screenToWorld
   * --------------------------------------------------------
   *   Convierte las coordenadas del cursor a coordenadas en el mundo 3D
   */
  void screenToWorld(int x, int y, GLdouble *outX, GLdouble *outY, GLdouble *outZ);

  GLdouble prespective[4];        // Prepectiva actual
  GLdouble width, height;         // Ancho y alto
  GLdouble tras[3];               // Traslación actual
  GLdouble pos[3];                // Posición actual
  GLdouble oldPos[3];             // Posición anterior
  GLdouble oldTras[3];            // Traslación anterior
  int oldX =0, oldY =0;           // Posición del cursor anterior
  double limitMove;               // Límite del mapa
  bool rotate = false, move= false;

};

#endif
