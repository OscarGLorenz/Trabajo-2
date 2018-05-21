 #include<iostream>
 #include"figuras.hpp"
 #include<GL/glut.h>

  /*  float color[3]={1.0,0.545,0.0};
        float position[6]={-0.180,-0.205,0.0,0.0,0.0,0.0}; */
        
  void Figuras::SetColor(Colors col){
      switch(col){
      case Colors::VOID:
         Color[0]=0.0f;
         Color[1]=0.0f;
         Color[2]=0.0f;
         Color[4]=0.0f;
         break;
        case Colors::RED:
         Color[0]=1.0f;
         Color[1]=0.0f;
         Color[2]=0.0f;
         
         break;
         case Colors::GREEN:
         Color[0]=0.0f;
         Color[1]=1.0f;
         Color[2]=0.0f;
        
         
         break;
         case Colors::BLUE:
         Color[0]=0.0f;
         Color[1]=0.0f;
         Color[2]=1.0f;
         
         break;
         case Colors::ORANGED:
         Color[0]=1.0f;
         Color[1]=0.545f;
         Color[2]=0.0f;
         
         break;
  
  }
  }
        
  Minero::Minero(){
    radio=0.2;
    coneHeight=0.3;
    position[1]=-0.180;
    position[2]=-0.205;
    position[3]=0.0;
    position[4]=0.0;
    position[5]=0.0;
    position[6]=0.0;
   
  }
  
  void Minero::SetColor (float R, float G, float B){
     Color[0]=R;
     Color[1]=G;
     Color[2]=B;
  
  
  }
  
  void Minero:: draw(int x,int y){
          
     //dibujo el cuerpo del minero
     
     glPushMatrix();
     //situar posicion para dibujar
     glTranslatef(-0.180f+(y-11.0)*0.422,-0.205f+(11.0-x)*0.422,position[2]);
     glRotatef(position[3],1,0,0);
     glRotatef(position[4],0,1,0);
     glRotatef(position[5],0,0,1);
     //selección color de dibujo
     glColor3f(Color[0],Color[1],Color[2]);
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
     Color[0]=0.0f;
     Color[1]=0.0f;
     Color[2]=0.0f;
         }
    
  void Cuadro:: draw(int x, int y){
  
     glDisable(GL_LIGHTING);
     glPushMatrix();
    
     glColor4f(Color[0],Color[1],Color[2],0.5f);
          
     glBegin(GL_POLYGON);
    
     glVertex3f(0.035f+(y-11.0)*0.422, 0.0f+(11.0-x)*0.422,0.01);
     glVertex3f(0.035f+(y-11.0)*0.422, -0.415f+(11.0-x)*0.422,0.01);
     glVertex3f(-0.380f+(y-11.0)*0.422, -0.415f+(11.0-x)*0.422,0.01);
     glVertex3f(-0.380f+(y-11.0)*0.422, 0.0f +(11.0-x)*0.422,0.01);
     glEnd();
     glPopMatrix();
     glEnable(GL_LIGHTING);
  
  }
   
  
  
