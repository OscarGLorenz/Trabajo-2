  enum class Colors{
     VOID,RED, GREEN , BLUE, ORANGED};
     
     
  class Figuras{
    
     public:
     
        float Color[4];
        void setColor(Colors);  
        virtual void draw(int,int)=0;
             


   };
   
   
   
   
 class Minero : public Figuras {
     public:
        Minero();
        //void setColor(Colors); 
        void draw(int,int);
        //float getRadio();
     private:
        double radio;
        float coneHeight;
        float position[6];
     
       
                
  };
  
  
     
     
  class Cuadro : public Figuras{
     public:
     Cuadro();
     //void setColor(Colors);  
     void draw(int,int);
        
         
  
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
