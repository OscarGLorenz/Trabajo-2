/******************************************************************************
* ARCHIVO :        Button.hpp
*
* DESCRIPCIÓN :
*       Clase para crear botones y asignares funciones
*
*       Todo el código para dibujar los botones en sus diferentes posciones y
*       comprobar sus estados y el ratón han sido extraidos de los ejemplos de
*       esta página:
*       https://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html
*
*       Por nuestra parte hemos encapsulado todo su código en una clase y
*       mejorado los callback usando std::function
*
* AUTOR :    Óscar García Lorenz
******************************************************************************/

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <functional>
#include <vector>

class Button {
public:
  /*
   * Constructor
   * --------------------------------------------------------
   *   Crea un nuevo botón
   *
   *   ox, oy: esquina superior izquierda del botón en la pantalla
   *   width: height: ancho y alto del botón
   *   str: texto a escribir en el botón
   *   call: callback cuando se haga clic en el botón
   */
  Button (int ox, int oy, int width, int height, std::string str, std::function<void()> call);

  /*
   * Función: clickTest
   * --------------------------------------------------------
   *   Comprueba si el cursor está dentro de botón
   *
   *   cx, cy: coordenadas del cursor
   */
  bool clickTest(int cx,int cy);

  /*
   * Función: press
   * --------------------------------------------------------
   *   Ejecuta la lógica del botón al pulsar el ratón
   *
   *   x, y: coordenadas del cursor
   */
  bool press(int x,int y);

  /*
   * Función: release
   * --------------------------------------------------------
   *   Ejecuta la lógica del botón al soltar un botón del ratón
   *
   *   x, y: coordenadas del cursor
   */
  void release(int x,int y);

  /*
   * Función: passive
   * --------------------------------------------------------
   *   Ejecuta la lógica del botón si se pasa por encima
   *
   *   x, y: coordenadas del cursor
   */
  void passive(int x,int y);

  /*
   * Función: draw
   * --------------------------------------------------------
   *   Dibuja el botón
   */
  void draw();

  /*
   * Función: motion
   * --------------------------------------------------------
   *   Ejecuta la lógica del botón si se pasa por encima pulsando
   *
   *   x, y: coordenadas del cursor
   */
  void motion(int x, int y);

  // Vector estático con todos los botones de la aplicación
  static std::vector<Button> Buttons;

private:
  int oldX, oldY;     // Antiguas coordenadas del cursor
  int x, y;						// Coordenadas del  botón
  int w, h;						// Ancho y alto del botón
  std::string label;	// Texto del botón
  std::function<void()> callback; // Callback al pulsar
  int	state;					// Estado, presionado=true
  int	highlighted;	  // Marcado, cursor encima=true

  /*
   * Función: font
   * --------------------------------------------------------
   *   Ejecuta la lógica del botón si se pasa por encima pulsando
   *
   *   x, y: coordenadas del texto
   *   font: fuente a usar
   *   text: cadena de caracteres a mostrar
   */
  void font(void *font,unsigned char *text,int x,int y);

};
#endif
