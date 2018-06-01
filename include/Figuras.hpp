/*************************************************************
*
* ARCHIVO: Figuras.hpp
*
* DESCRIPCIÓN:
* 	Contiene la definición de la clase Figuras usada para
* 	dibujar los distintos elementos del juego.
*
* AUTOR: Alejandro Redondo Ayala
*
*************************************************************/

#ifndef FIGURAS_HPP
#define FIGURAS_HPP

/*
* Clase enumerada : Colors
* -----------------------------------------------------------
*
* Clase en la que se enumeran los colores de las casillas
*
*/

enum class Colors{VOID, RED, GREEN, BLUE};

/*
* Clase : Figuras
* -----------------------------------------------------------
*
*  Clase en la que se encuentran los elementos a dibujar
*
* -----------------------------------------------------------
* Atributo: Color
* -----------------------------------------------------------
*
* Se almacenan aquí 4 valores enteros los tres primeros
* correspondientes al código RGB del color deseado y el
* cuarto correspondiente a lo translúcido que es el
* objeto siendo 1 totalmente opaco y 0 transparente
*
* -----------------------------------------------------------
* Método: setColor
* -----------------------------------------------------------
*
* Fija los valores del atributo Color recibiendo como
* parámetro uno de los enumerados de la clase Colors
*
* -----------------------------------------------------------
* Método virtual puro: draw
* -----------------------------------------------------------
*
* Dibuja cada subclase según la propia implementación de
* draw en la clase derivada.
* Recibe como parámetro la casilla en la que se tiene que
* dibujar.
*
* -----------------------------------------------------------
*/

class Figuras{

public:

  float Color[4];
  void setColor(Colors);
  virtual void draw(int,int)=0;
};



/*
* Clase : Minero
* -----------------------------------------------------------
*
* Clase derivada de la clase Figuras en la que se define
* la figura que representará al minero en el juego.
*
* -----------------------------------------------------------
* Atributo: position
* -----------------------------------------------------------
*
* Vector de 6 elementos, los tres primeros indican posición
* respecto al eje x al eje y y al eje z, los 3 siguientes
* angulo de rotación respecto a esos ejes
*
* -----------------------------------------------------------
* Constructor: Minero
* -----------------------------------------------------------
*
*  Genera un objeto de la clase Minero
*
* -----------------------------------------------------------
* Método : draw
* -----------------------------------------------------------
*
* Apartir de la casilla en la que se desee dibujar dibuja un
* pico formado por cubos
*
* -----------------------------------------------------------
* Método: drawCube
* -----------------------------------------------------------
*
* Dibuja un cubo, teniendo como argumentos posición x,
* posición y, posición z, los tres siguientes código RGB del
* color del cubo, tamaño, y los dos siguientes argumentos
* corresponden a la casilla del mapa donde debe ser dibujado
* esta función es la emleada por draw para construir el pico
*
* -----------------------------------------------------------
*
*/

class Minero : public Figuras {
public:
  Minero();
  void draw(int,int);
  float position[6];
private:
  void drawCube(float , float , float , float , float , float , float ,int,int);
};

/*
* Clase : Cuadro
* -----------------------------------------------------------
*
* Clase derivada de la clase Figuras en la que se define
* la figura que representará los recuadros que marcan el
* estado de las casillas:
* Rojo: Casilla sin visitar.
* Azul: Casilla adyacente en el modo solución, y casilla
* adyacente y pendiente de visitar en modo algoritmo.
* Verde: Casilla visitada.
*
* -----------------------------------------------------------
* Constructor: Cuadro
* -----------------------------------------------------------
*
*  Genera un objeto de la clase Cuadro
*
* -----------------------------------------------------------
* Método : draw
* -----------------------------------------------------------
*
* Dibuja los indicadores de estado de las casillas teniendo
* como parámetro el número de casilla en la que se dibuja
* el recuadro.
*
* -----------------------------------------------------------
*
*/

class Cuadro : public Figuras{
public:
  Cuadro();
  void draw(int,int);
};

/*
* Clase : Plano
* -----------------------------------------------------------
*
* Clase derivada de la clase Figuras en la que se define
* un plano que no es visible para el usuario pero que es
* necesario para que la cámara se mueva con el ratón
*
* -----------------------------------------------------------
* Método : dibuja
* -----------------------------------------------------------
*
* Dibuja el plano
*
* -----------------------------------------------------------
*
*/
class Plano : public Figuras{
public:
  void draw(int ,int);
};

#endif
