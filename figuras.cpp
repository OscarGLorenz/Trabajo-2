 #include<iostream>
 #include"figuras.hpp"
 #include<GL/glut.h>

    float color[3]={1.0,0.545,0.0};
        float position[6]={-0.180,-0.205,0.0,0.0,0.0,0.0};
  Minero::Minero(){
    radio=0.2;
    coneHeight=0.3;
  
  }
  
  void Minero:: drawMinero(float x,float y){
          
     //dibujo el cuerpo del minero
     
     glPushMatrix();
     //situar posicion para dibujar
     glTranslatef(x,y,position[2]);
     glRotatef(position[3],1,0,0);
     glRotatef(position[4],0,1,0);
     glRotatef(position[5],0,0,1);
     //selección color de dibujo
     glColor3f(color[0],color[1],color[2]);
     //dibujo del cuerpo
     
     glutSolidCone(radio,coneHeight,20,20);
     glPopMatrix();
  
     
     //dibujo de la cabeza
     glPushMatrix();
     glTranslatef(x,y,position[2]+coneHeight+(radio/2));
     glColor4f(0.0F,1.0F,0.0F,1.0F);
     //no cambio la rotación pues es la misma
     glutSolidSphere(radio,20,20);
     
     glPopMatrix();
      
   
}

  Cuadro::Cuadro(){
     x=0;
     y=0;    }
    
  void Cuadro:: drawCuadro(int x, int y, Colors col){
  
     glDisable(GL_LIGHTING);
     glPushMatrix();
     switch(col){
        case Colors::RED:
         glColor4f(1.0f,0.0f,0.0f,0.5f);
         break;
         case Colors::GREEN:
         glColor4f(0.0f,1.0f,0.0f,0.5f);
         break;
         case Colors::BLUE:
         glColor4f(0.0f,0.0f,1.0f,0.5f);
         break;
     
     
     }
       
    glBegin(GL_POLYGON);
    
    glVertex3f(0.035f+(y-11.0)*0.422, 0.0f+(11.0-x)*0.422,0.01);
    glVertex3f(0.035f+(y-11.0)*0.422, -0.415f+(11.0-x)*0.422,0.01);
    glVertex3f(-0.380f+(y-11.0)*0.422, -0.415f+(11.0-x)*0.422,0.01);
    glVertex3f(-0.380f+(y-11.0)*0.422, 0.0f +(11.0-x)*0.422,0.01);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
  
  }
   
  
  
