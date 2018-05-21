  enum class Colors{
     VOID,RED, GREEN , BLUE, ORANGED};
     
     
  class Figuras{
    
     public:
     
        float Color[3];
        virtual void setColor(Colors);  
        virtual void draw(int,int)=0;
             


   };
   
   
   
   
 class Minero : public Figuras {
     public:
     Minero();
      virtual void setColor(Colors);  
      virtual void draw(int,int)=0;
     private:
        float radio;
        float coneHeight;
        float position[6];
     
       
                
  };
  
  
     
     
  class Cuadro : public Figuras{
     public:
     Cuadro();
      virtual void setColor(Colors);  
      virtual void draw(int,int)=0;
        
         
  
  };

/* BORRAR




 class Minero {
     private:
        
        float radio;
        float coneHeight;
     public:
        Minero();
        void drawMinero(float x, float y);
        //void setPos(float x. float y);
        
  };
  
  
     
     
  class Cuadro{
     public:
        Cuadro();
        void drawCuadro(int x, int y, Colors col);
        int x;
        int y;
     
     
     
     
  
  
  };*/
