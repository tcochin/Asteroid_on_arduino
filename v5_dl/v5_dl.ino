#include <Wire.h>
#include <Arduino.h>
#include "ht1632.h"
#include <avr/pgmspace.h>
#include <TimerOne.h>

#define BUZZER 3

void setup() {
  // put your setup code here, to run once:
  ht1632_setup();                       // initialise the wire library and hardware  
  Serial.begin(9600);
  Wire.begin();
  setup7Seg();    
  cls();
  pinMode(BUZZER, OUTPUT);
 
  Timer1.attachInterrupt(fnPeriodique,25000);//min 25ms
}


word compte=0;
word vitesse=300;
typedef struct{
  byte posX;
  byte posY;
  byte lastX;
  byte lastY;
  int deltaX;
  int deltaY;
  byte etat;
  byte typeAsteroid;  //on défini quelques types d'astéroid possible plutot qu'une génération aléatoire
  }T_asteroid;
int const nombreAsteroid=10;
T_asteroid tableauAsteroid[nombreAsteroid+1];
int MAX_COUNT_ASTEROIDS = sizeof(tableauAsteroid)/2;


  

typedef struct{
  byte posX;
  byte posY;
  byte lastX;
  byte lastY;
  byte orientation;
  byte lastOrientation;
  }T_vaisseau;

byte changementOrientationPosition=0;

#define N 1
#define NE 2
#define E 3
#define SE 4
#define S 5
#define SO 6
#define O 7
#define NO 8

#define Y_pin 1   // joystick
#define X_pin 0

T_vaisseau vaisseau1={15,8,15,8,N
};

//---------------------------------------gestion vaisseau-------------------------------------
//---------------------------------------effacement-------------------------------------------
void effacerPuisAfficherVaisseau(){
  //partie eteignage de LED

    if(vaisseau1.lastOrientation==N or vaisseau1.lastOrientation==S){    //effacement NORD ET SUD
      ht1632_plot(vaisseau1.lastX-1,vaisseau1.lastY,LOW);
      ht1632_plot(vaisseau1.lastX+1,vaisseau1.lastY,LOW);
      if(vaisseau1.lastOrientation==N){
        ht1632_plot(vaisseau1.lastX,vaisseau1.lastY-1,LOW);
        }
      if(vaisseau1.lastOrientation==S){
        }  
         ht1632_plot(vaisseau1.lastX,vaisseau1.lastY+1,LOW);

      }


    if(vaisseau1.lastOrientation==O or vaisseau1.lastOrientation==E){    //effacement OUEST ET EST
      ht1632_plot(vaisseau1.lastX,vaisseau1.lastY-1,LOW);
      ht1632_plot(vaisseau1.lastX,vaisseau1.lastY+1,LOW);
      if(vaisseau1.lastOrientation==O){
        ht1632_plot(vaisseau1.lastX-1,vaisseau1.lastY,LOW);
        }
      if(vaisseau1.lastOrientation==E){
        ht1632_plot(vaisseau1.lastX+1,vaisseau1.lastY,LOW);
        }  
      }

    if(vaisseau1.lastOrientation==NO or vaisseau1.lastOrientation==NE){   //effacement de LED N-OUEST ET N-EST
     
      ht1632_plot(vaisseau1.lastX,vaisseau1.lastY,LOW);
      ht1632_plot(vaisseau1.lastX,vaisseau1.lastY+1,LOW);
      if(vaisseau1.lastOrientation==NO){   //affihcage vaiseeau N-Ouest
      ht1632_plot(vaisseau1.lastX+1,vaisseau1.lastY,LOW);
        }
      if(vaisseau1.lastOrientation==NE){       //affichage vaisseau N-Est
        ht1632_plot(vaisseau1.lastX-1,vaisseau1.lastY,LOW);
        }  
      }
    
     if(vaisseau1.lastOrientation==SO or vaisseau1.lastOrientation==SE){   //effacement de LED N-OUEST ET N-EST
     
      ht1632_plot(vaisseau1.lastX,vaisseau1.lastY,LOW);
      ht1632_plot(vaisseau1.lastX,vaisseau1.lastY-1,LOW);
      if(vaisseau1.lastOrientation==SO){   //affihcage vaiseeau N-Ouest
      ht1632_plot(vaisseau1.lastX+1,vaisseau1.lastY,LOW);
        }
      if(vaisseau1.lastOrientation==SE){       //affichage vaisseau N-Est
        ht1632_plot(vaisseau1.lastX-1,vaisseau1.lastY,LOW);
        }  
      }

  //---------------------------------------affichage-------------------------------------------

    if(vaisseau1.orientation==N or vaisseau1.orientation==S){   //partie alumage de LED NORD ET SUD
      ht1632_plot(vaisseau1.posX-1,vaisseau1.posY,GREEN);
      ht1632_plot(vaisseau1.posX+1,vaisseau1.posY,GREEN);
      if(vaisseau1.orientation==N){   //affihcage vaiseeau Nord
        ht1632_plot(vaisseau1.posX,vaisseau1.posY-1,GREEN);
        }
      if(vaisseau1.orientation==S){       //affichage vaisseau Nord
         ht1632_plot(vaisseau1.posX,vaisseau1.posY+1,GREEN);

        }  
      }

      
     if(vaisseau1.orientation==O or vaisseau1.orientation==E){   //partie alumage de LED OUEST ET EST
      ht1632_plot(vaisseau1.posX,vaisseau1.posY-1,GREEN);
      ht1632_plot(vaisseau1.posX,vaisseau1.posY+1,GREEN);     
      if(vaisseau1.orientation==O){   //affihcage vaiseeau Ouest
          ht1632_plot(vaisseau1.posX-1,vaisseau1.posY,GREEN);

        }
      if(vaisseau1.orientation==E){       //affichage vaisseau Est
           ht1632_plot(vaisseau1.posX+1,vaisseau1.posY,GREEN);
        }  
      }

      if(vaisseau1.orientation==NO or vaisseau1.orientation==NE){   //partie alumage de LED N-OUEST ET N-EST
      ht1632_plot(vaisseau1.posX,vaisseau1.posY,GREEN);
      ht1632_plot(vaisseau1.posX,vaisseau1.posY+1,GREEN);
      if(vaisseau1.orientation==NO){   //affihcage vaiseeau N-Ouest
      ht1632_plot(vaisseau1.posX+1,vaisseau1.posY,GREEN);
        }
      if(vaisseau1.orientation==NE){       //affichage vaisseau N-Est
    ht1632_plot(vaisseau1.posX-1,vaisseau1.posY,GREEN);
        }  
      }

      if(vaisseau1.orientation==SO or vaisseau1.orientation==SE){   //partie alumage de LED S-OUEST ET S-EST
      ht1632_plot(vaisseau1.posX,vaisseau1.posY,GREEN);
      ht1632_plot(vaisseau1.posX,vaisseau1.posY-1,GREEN);
      if(vaisseau1.orientation==SO){   //affihcage vaiseeau S-Ouest
      ht1632_plot(vaisseau1.posX+1,vaisseau1.posY,GREEN);
        }
      if(vaisseau1.orientation==SE){       //affichage vaisseau N-Est
    ht1632_plot(vaisseau1.posX-1,vaisseau1.posY,GREEN);
        }  
      }

      
  }


  

void deplacerVaisseau(word posControleurX,word posControleurY){
 


 if(posControleurY>600 and posControleurY!=0){//partie Nord
  if(posControleurX<=420){//nord est
  //  Serial.println("nord-est");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=NE;
    vaisseau1.posX=vaisseau1.posX+1;
    if(vaisseau1.posY<=0 and vaisseau1.posX>=31){//le vaisseau est dans le coin en haut à droite
      vaisseau1.posY=15;
      vaisseau1.posX=0;
      }
    else if(vaisseau1.posY<=0){// si le vaisseau est tout en haut
      vaisseau1.posY=15; 
      }
    else{
     // Serial.println("nord");
      vaisseau1.posY=vaisseau1.posY-1;
      }
     }
    else if(580<=posControleurX){//nord-ouest
 //  Serial.println("nord-ouest");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=NO;
    vaisseau1.posX=vaisseau1.posX-1;
    if(vaisseau1.posY<=0){// si le vaisseau est tout en haut
      vaisseau1.posY=15; 
      }
    else{
      vaisseau1.posY=vaisseau1.posY-1;
      }
    }
  else{//plein nord
      vaisseau1.lastOrientation=vaisseau1.orientation;
      vaisseau1.lastX=vaisseau1.posX;
      vaisseau1.lastY=vaisseau1.posY;
      vaisseau1.orientation=N;
    if(vaisseau1.posY<=0){// si le vaisseau est tout en haut
      vaisseau1.posY=15; 
      }
    else{
     // Serial.println("nord");
      vaisseau1.posY=vaisseau1.posY-1;
      }
    }
   
      
  }
  else if(posControleurY<400 and posControleurY!=0){//partie Sud
  if(posControleurX<=420){//sud est
  //  Serial.println("sud-est");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=SE;
    vaisseau1.posX=vaisseau1.posX+1;
    vaisseau1.posY=vaisseau1.posY+1;
    if(vaisseau1.posY>=15){// si le vaisseau est tout en bas
      vaisseau1.posY=0; 
      }
    else{
      vaisseau1.posY=vaisseau1.posY+1;
      }
     }
    else if(580<=posControleurX){//sud-ouest
 //  Serial.println("sud-ouest");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=SO;
    vaisseau1.posX=vaisseau1.posX-1;
    if(vaisseau1.posY>=15){// si le vaisseau est tout en bas
      vaisseau1.posY=0; 
      }
    else{
      vaisseau1.posY=vaisseau1.posY+1;
      }
     }
  else{//plein sud
   // Serial.println("sud");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=S;
    if(vaisseau1.posY>=15){// si le vaisseau est tout en bas
      vaisseau1.posY=0; 
      }
    else{
      vaisseau1.posY=vaisseau1.posY+1;
      }
    }       
  }
 
  
else if(posControleurX<420 and posControleurY!=0){  //est
  //Serial.println("est");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=E;
    if(vaisseau1.posX>=31){// si le vaisseau est tout a droite
      vaisseau1.posX=0; 
      }
    else{
       vaisseau1.posX=vaisseau1.posX+1;
      }
    }
else if(posControleurX>600 and posControleurY!=0){  //ouest
 // Serial.println("ouest");
    vaisseau1.lastOrientation=vaisseau1.orientation;
    vaisseau1.lastX=vaisseau1.posX;
    vaisseau1.lastY=vaisseau1.posY;
    vaisseau1.orientation=O;
if(vaisseau1.posX<=0){// si le vaisseau est tout a gauche
      vaisseau1.posX=31; 
      }
    else{
       vaisseau1.posX=vaisseau1.posX-1;
      }   
     }      
  
  /*
  Serial.println("Position controleur=");
  Serial.print(posControleurX);
  Serial.println(":");
  Serial.println(posControleurY);*/
  }

//---------------------------------------fin gestion vaisseau-------------------------------------

  //---------------------------------------Debut asteroids-------------------------------------------

void effacerAfficherAsteroid(byte i){
 if (tableauAsteroid[i].typeAsteroid == 0) {
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY,ORANGE);
  } else if (tableauAsteroid[i].typeAsteroid == 1) {
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX-1,tableauAsteroid[i].posY,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY+1,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX+1,tableauAsteroid[i].posY,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY-1,ORANGE);
  } else if (tableauAsteroid[i].typeAsteroid == 2) {
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX-1,tableauAsteroid[i].posY-1,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY+1,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX+1,tableauAsteroid[i].posY,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY-1,ORANGE);
  } else {
    ht1632_plot(tableauAsteroid[i].posX,tableauAsteroid[i].posY,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX-1,tableauAsteroid[i].posY-1,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX+1,tableauAsteroid[i].posY+1,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX+1,tableauAsteroid[i].posY-1,ORANGE);
    ht1632_plot(tableauAsteroid[i].posX-1,tableauAsteroid[i].posY+1,ORANGE);
  }
//ht1632_plot(v.posX-1,vaisseau1.posY,ORANGE);


}

  
void afficherPosition() {
  ht1632_plot(vaisseau1.posX,vaisseau1.posY,RED);
  ht1632_plot(vaisseau1.lastX,vaisseau1.lastY,LOW);
  }

  //---------------------------------------Fin asteroid-------------------------------------------
 void compteur (word vitesse){
    if(compte%vitesse==0){//permet gérer la vitesse
     deplacerVaisseau(analogRead(X_pin),analogRead(Y_pin));
    /* Serial.print(analogRead(X_pin));
     Serial.print(":");
     Serial.println(analogRead(Y_pin));*/
       //Serial.println(vaisseau1.lastOrientation);
      /* Serial.println("position vaisseau");
       Serial.print(vaisseau1.posX);
       Serial.print(":");
       Serial.println(vaisseau1.posY);
       Serial.println("ancienne position vaisseau");
       Serial.print(vaisseau1.lastX);
       Serial.print(":");
       Serial.println(vaisseau1.lastY);*/
         /*if(compte%(vitesse*50)==0){//augmenter la vitesse tous les 100 tours
    vitesse=vitesse-50;
    }  */
      }
     if(compte%(3*vitesse)==0){
//      AfficherAsteroid();
      
      
     }


 }
     



void loop() {
  // mettre l'affichage ici
  effacerPuisAfficherVaisseau();   
  /*tableauAsteroid[0].posX = 5;  asteroid
  tableauAsteroid[0].posY = 5;
  tableauAsteroid[0].typeAsteroid = 0;

  tableauAsteroid[1].posX = 10;
  tableauAsteroid[1].posY = 5;
  tableauAsteroid[1].typeAsteroid = 1;

  tableauAsteroid[2].posX = 20;
  tableauAsteroid[2].posY = 5;
  tableauAsteroid[2].typeAsteroid = 2;

  tableauAsteroid[3].posX = 10;
  tableauAsteroid[3].posY = 10;
  tableauAsteroid[3].typeAsteroid = 3;
  effacerAfficherAsteroid(0);
  effacerAfficherAsteroid(1);
  effacerAfficherAsteroid(2);
  effacerAfficherAsteroid(3);*/


  
}

void fnPeriodique(void) {
  compteur(vitesse);  
  compte++;

  }
