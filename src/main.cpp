#include <Arduino.h>
#include <LiquidCrystal.h>
#define btnIN A0 //Entrada analogica
#define LED 10   //Salida digital

/////////////////variables menu////////////
int menuLCD;
int salida;
int fotocelda;
String AnuncioMenu;

void Estado();
void Anuncio();
void Menu();
 
LiquidCrystal lcd(12,11,5,4,3,2);
/////////////////////////////////////////

enum funMode
{
  OFF,
  BLINK1,
  BLINK2,
  BLINK3,
  BLINK4,
  NBSTATE
};

int L_STD = LOW;
unsigned long pre_Millis, pas_Millis;
unsigned long lastDeTime = 0;       //Ultimo tiempo definido como 0 o inicio (rebote)
int LstBSTD = HIGH;                 //Ultimo estado pre-definido es Alto
int btnSTD = 0;                     //Estado presente
int funcSTD = 0;                    //Estados para de la funcion de seleccion de Menu
int BP1 = 50, BP2 = 100, BP3 = 200; //Blink intermidiate time
int interB = 100;                  //Blink intermidiate time general

int brillo = 0; 
int fadeAmount = 5; 

unsigned long tiempo;
unsigned long tiempoX=0;
unsigned long demora=100;

void setup()
{
  pinMode(btnIN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  ///////////MENU/////////
  pinMode(13, OUTPUT); //Led como salida
  pinMode(6,OUTPUT); //Buzzer como salida
  lcd.begin(16,2); //confiramos el tamaño de la LDC
  digitalWrite(15,HIGH) //Resistencia PullUp al pulsador
}

void loop()
{
  func1(); //Button detection
  func2(); //Menu de seleccion
  fade();

  //////////MENU//////////////
  menu = analogRead (0); //Guardamos la posicion de potenciometro
  menu = map(menu,0,1023,0,3) //escalamos el valor para que sea Menu
  
}
/////////////////////////////////////////////
void func1()
{
  int reading = digitalRead(btnIN);
  if (reading != LstBSTD)
  {                        //Ultima estado definido del buton
    lastDeTime = millis(); //lastDeubonceTime
    if ((millis() - lastDeTime) > 100)
    {
      if (reading != btnSTD)
      {
        btnSTD = reading;
      }
      if (btnSTD == HIGH)
      {
        funcSTD += 1;
      }
    }
  }
  LstBSTD = reading;
  funcSTD = funcSTD % NBSTATE;
}
/////////////////////////////////////
void func2()
{
  switch (funcSTD)
  {
  case OFF:
    //FuncionOFF - LED
    digitalWrite(LED, LOW);
    break;
  case BLINK1:
    //FuncionBlink1 - LED
    funcB1(BP1);
    break;
  case BLINK2:
    //FuncionBlink2 - LED
    funcB2(BP2);
    break;
  case BLINK3:
  //FuncionBlink3 - LED
    funcB3(BP3);
    break;
  //FuncionBlink4 - LED
  case BLINK4:
    funcB4(interB);
    break;
  }
}
void funcB1(int BP1)
{
  delay(BP1);
  digitalWrite(LED, LOW);
  delay(BP1);
  digitalWrite(LED, HIGH);
}
void funcB2(int BP2)
{
  delay(BP2);
  digitalWrite(LED, LOW);
  delay(BP2);
  digitalWrite(LED, HIGH);
}
void funcB3(int BP3)
{
  delay(BP3);
  digitalWrite(LED, LOW);
  delay(BP3);
  digitalWrite(LED, HIGH);
}
void funcB4(int interB) {
  pre_Millis = millis();
  if ((pre_Millis - pas_Millis) >= interB) {
    pas_Millis = pre_Millis;
    if(L_STD = LOW) {
      L_STD = HIGH;
    } else {
      L_STD = LOW;
    }
  }

}

void fade(){
    tiempo = millis ();
  if(tiempo>tiempoX+demora){
  tiempoX=tiempo;
  analogWrite (LED, brillo);
 brillo = brillo + fadeAmount; 
 if (brillo == 0 || brillo == 255) {fadeAmount = -fadeAmount; }
 }
    }