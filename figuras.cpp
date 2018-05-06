 #include<iostream>
 #include"figuras.hpp"
 #include<GL/glut.h>

    float color[3]={1.0,0.545,0.0};
        float position[6]={-0.144,-0.190,0.0,0.0,0.0,0.0};
  Minero::Minero(){
    radio=0.2;
    coneHeight=0.3;
  
  }
  
  void Minero:: drawMinero(){
          
     //dibujo el cuerpo del minero
     
     glPushMatrix();
     //situar posicion para dibujar
     glTranslatef(position[0],position[1],position[2]);
     glRotatef(position[3],1,0,0);
     glRotatef(position[4],0,1,0);
     glRotatef(position[5],0,0,1);
     //selección color de dibujo
     glColor3f(color[0],color[1],color[2]);
     //dibujo del cuerpo
     
     glutSolidCone(radio,coneHeight,20,20);
     
  
     
     //dibujo de la cabeza
     
     glTranslatef(position[0],position[1],position[2]+coneHeight+(radio/2));
     glColor3f(1.0,0,0);
     //no cambio la rotación pues es la misma
     glutSolidSphere(radio,10,10);
     
     glPopMatrix();
      
   
}
  
  
