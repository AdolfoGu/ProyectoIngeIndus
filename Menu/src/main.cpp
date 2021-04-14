#include <Arduino.h>
#include <LiquidCrystal.h>

// Constantes:
#define pinLedB 9	// Led Azul y Amarillo, ambos en un pin PWM.
#define pinLedY 10
#define aum 50		// El aumento de brillo por cada pulsacion.

// Variables globales:
int ledY_I			= 0;	// Nivel de iluminación de los LEDs.
int ledB_I			= 0;
boolean ledY_E		= false;	// Nos indicaran si los leds estan encendidos o apagados.
boolean ledB_E		= false;
byte arrowDown[8]	= {B00000, B01110, B01110, B01110, B11111, B01110, B00100};	// Caracteres personalizados para el LCD
byte cursorLCD[8]	= {B00000, B00000, B00110, B01111, B01111, B00110, B00000};
byte ledOn[8]		= {B00000, B01110, B10001, B10001, B01010, B01110, B01110};
byte white[8]		= {B11111, B11111, B11111, B11111, B11111, B11111, B11111};


// Se instancia el LCD con los pines indicados:
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



void setup()
{
	// Pantalla de 20 caracteres y 4 líneas:
	lcd.begin(20, 4);

	// Se añaden los caracteres especiales al objeto LCD:
	lcd.createChar(0, cursorLCD);
	lcd.createChar(1, arrowDown);
	lcd.createChar(2, ledOn);
	lcd.createChar(3, white);

	pinMode(pinLedY, OUTPUT);		// Se configuran los pines de los LEDs como salidas y se ponen a 0.
	pinMode(pinLedB, OUTPUT);
	analogWrite(pinLedY,ledY_I);
	analogWrite(pinLedB,ledB_I);
}



void loop()
{
	// Guarda el resultado leído de una pulsación por teclado:
	char resultButton[7] = "null";

	// Se imprime la pantalla de inicio:
	lcd.setCursor(5, 0); lcd.print("giltesa.com");
	lcd.setCursor(0, 1); lcd.print("MenuLCD para LEDs v1");
	lcd.setCursor(7, 3); lcd.print("Pulse "); lcd.write(byte(1));

	// Se comprueba si se ha pulsado un botón:
	getButtonPush( resultButton, 0, 200);

	// Si se pulsa la tecla abajo, se entra en el menú:
	if( !strcmp(resultButton,"bottom"))
		LCDmenu();
}



// MENU PRINCIPAL, DA OPCION PARA ABRIR EL MENU 1 Y MENU 2, TAMBIEN PARA SALIR A LA PANTALLA PRINCIPAL
void LCDmenu()
{
	char resultButton[7];
	int posCursor=0, opcMenu=-2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("Encender LED");
	lcd.setCursor(2, 1); lcd.print("Cambiar brillo");
	lcd.setCursor(2, 2); lcd.print("Salir");


	// Mientras no se pulse sobre salir, no se saldrá a la pantalla principal:
	do{
		strcpy(resultButton,"null");
		getButtonPush( resultButton, 0, 200);

		// Cursor que indica la opción seleccionada:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));

		// Desplaza el cursor arriba o abajo:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 2, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 2, 's');

		else if(!strcmp(resultButton,"right"))
			opcMenu = posCursor;				// Al pulsar a la derecha (para acceder a esa opcion) se actualiza la opción de menú elegida según donde esta el cursor ahora.


		// Según la opción elegida del menú, se llama a otro menú o se cierra el menú actual:
		switch( opcMenu )
		{
			case 0:				// Se accede al menú de encendido y apagado de los leds.
				LCDmenu0();
				opcMenu = -2;
				break;
			case 1:				// Se accede al menú de ajuste de brillo de los leds.
				LCDmenu1();
				opcMenu = -2;
				break;
			case 2:				// Salir a la pantalla principal.
				opcMenu = -1;
				lcd.clear();
				break;
		};
	}
	while(opcMenu != -1);
}



// MENU PARA EL ENCENDIDO Y APAGADO DE LOS LEDs
void LCDmenu0()
{
	char resultButton[7];
	int posCursor=0, opcMenu=-2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("Led Amarillo");
	lcd.setCursor(2, 1); lcd.print("Led Azul");
	lcd.setCursor(2, 2); lcd.print("Salir");

	// Actualiza los indicadores (bombillas dibujadas) del LCD:
	LCDBombillasLed( ledY_E, 0);
	LCDBombillasLed( ledB_E, 1);


	// Mientras no se pulse sobre salir, no se saldrá al Menú:
	do{
		strcpy(resultButton,"null");
		getButtonPush( resultButton, 0, 200);

		// Cursor que indica la opcion seleccioanda:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));

		// Desplaza el cursor arriba o abajo,:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 2, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 2, 's');

		else if(!strcmp(resultButton,"right") || !strcmp(resultButton,"left"))
			opcMenu = posCursor;


		// Según la opción elegida del menú, se le cambia el estado a un LED, al otro o se cierra el menú actual:
		switch( opcMenu )
		{
			// LED Amarillo
			case 0:
				if(!strcmp(resultButton,"right"))		// A la derecha se enciende.
				{
					if(ledY_I == 0) ledY_I = 20; 		// Si la Intensidad de iluminación es 0 se encenderá como mínimo a 20.
					analogWrite(pinLedY,ledY_I);
					ledY_E = true;
				}
				else if( !strcmp(resultButton,"left") ) // A la izquierda se apaga.
				{
					analogWrite(pinLedY,0);
					ledY_E = false;
				}

				opcMenu = -2;
				LCDBombillasLed( ledY_E, 0);
			break;

			// LED Azul
			case 1:
				if(!strcmp(resultButton,"right"))
				{
					if(ledB_I == 0) ledB_I = 20;
					analogWrite(pinLedB,ledB_I);
					ledB_E = true;
				}
				else if( !strcmp(resultButton,"left") )
				{
					analogWrite(pinLedB,0);
					ledB_E = false;
				}

				opcMenu = -2;
				LCDBombillasLed( ledB_E, 1);
			break;

			// Salir
			case 2:
				if(!strcmp(resultButton,"right"))
					opcMenu = -1;
			break;
		};
	}
	while(opcMenu != -1);

	// Al salir del Menu1 se imprime el menú anterior:
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("Encender LED");
	lcd.setCursor(2, 1); lcd.print("Cambiar brillo");
	lcd.setCursor(2, 2); lcd.print("Salir");
}



// MENU PARA EL CONTROL DE BRILLO DE LOS LEDs
void LCDmenu1()
{
	char resultButton[7];
	int posCursor=0, opcMenu=-2, i, temp1, temp2;


	// Menu
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("Led Amarillo");
	lcd.setCursor(2, 1); lcd.print("Led Azul");
	lcd.setCursor(2, 2); lcd.print("Salir");

	LCDGraficaBrillo(ledY_I, 0);
	LCDGraficaBrillo(ledB_I, 1);


	// Mientras no se pulse sobre salir, no se saldrá al Menú:
	do{
		strcpy(resultButton,"null");
		getButtonPush( resultButton, 0, 200);

		// Cursor que indica la opcion seleccioanda:
		lcd.setCursor(0, posCursor); lcd.write(byte(0));

		// Desplaza el cursor arraba o abajo,:
		if(!strcmp(resultButton,"up"))
			LCDMueveCursor(&posCursor, 2, 'r');	// Se resta una posición al cursor

		else if(!strcmp(resultButton,"bottom"))	// Se suma una posición al cursor
			LCDMueveCursor(&posCursor, 2, 's');

		else if(!strcmp(resultButton,"right") || !strcmp(resultButton,"left"))
			opcMenu = posCursor;


		// Según la opción elegida del menú, se le cambia el estado a un LED, al otro o se cierra el menú actual:
		switch( opcMenu )
		{
			// LED Amarillo
			case 0:
				if(!strcmp(resultButton,"right"))		// A la derecha se enciende.
					if(ledY_I + aum < 250)
						ledY_I += aum;
					else
						ledY_I = 250;
				else if( !strcmp(resultButton,"left") ) // A la izquierda se apaga.
					if(ledY_I - aum > 0)
						ledY_I -= aum;
					else
						ledY_I = 0;
				opcMenu = -2;

				analogWrite(pinLedY,ledY_I);
				LCDGraficaBrillo(ledY_I, 0);
			break;

			// LED Azul
			case 1:
				if(!strcmp(resultButton,"right"))
					if(ledB_I + aum < 250)
						ledB_I += aum;
					else
						ledB_I = 250;
				else if( !strcmp(resultButton,"left") )
					if(ledB_I - aum > 0)
						ledB_I -= aum;
					else
						ledB_I = 0;
				opcMenu = -2;

				analogWrite(pinLedB,ledB_I);
				LCDGraficaBrillo(ledB_I, 1);
			break;

			// Salir
			case 2:
				if(!strcmp(resultButton,"right"))
					opcMenu = -1;
			break;
		};
	}
	while(opcMenu != -1);

	// Se imprime el menú anterior:
	lcd.clear();
	lcd.setCursor(2, 0); lcd.print("Encender LED");
	lcd.setCursor(2, 1); lcd.print("Cambiar brillo");
	lcd.setCursor(2, 2); lcd.print("Salir");
}


// Actualiza los indicadores (bombillas dibujadas) del LCD:
void LCDBombillasLed( boolean led_E, int fila)
{
	if(led_E)
	{
		lcd.setCursor(18, fila); lcd.write(byte(2));
	}
	else
	{
		lcd.setCursor(18, fila); lcd.print(" ");
	}
}

// Actualiza los indicadores de brillo del LCD:
void LCDGraficaBrillo( int led_I, int fila)
{
	int i, temp1 = led_I/aum, temp2 = 250/aum-temp1;

	for( i=0;  i < temp1; i++ )
	{
		lcd.setCursor(15+i, fila); lcd.write(byte(3));
	}
	for( i=0;  i < temp2; i++ )
	{
		lcd.setCursor(15+temp1+i, fila); lcd.print(" ");
	}
}



// DESPLAZA EL CURSOR UNA POSICION EN LA DIRECCION INDICADA
void LCDMueveCursor(int *pos, int max, char ope)
{
	// Precondición:	El parámetro ope debería de ser el carácter 's' o 'r' para sumar o restar una posición.
	//					El parámetro max indica el numero de opciones a elegir.
	// Poscondición:	Se devuelve por referencia el nuevo valor del cursor.

	if(ope == 's')
		if(*pos < max)	*pos += 1;
		else			*pos = 0;
	else
		if(*pos > 0)	*pos -= 1;
		else			*pos = max;

	// Se borran los cursores:
	lcd.setCursor(0, 0); lcd.print(" ");
	lcd.setCursor(0, 1); lcd.print(" ");
	lcd.setCursor(0, 2); lcd.print(" ");
	lcd.setCursor(0, 3); lcd.print(" ");
}



// PROCEDIMIENTO QUE COMPRUEBA SI HA HABIDO NUEVAS PULSACIONES CON LOS BOTONES:
void getButtonPush(char *resultButton, int pin, int pause)
{
	// Precondición:	El parámetro pin ha de ser un pin Analógico del Arduino.
	// Poscondición:	El parámetro resultButton es una cadena por referencia que devolverá el botón pulsado.
	//					El parámetro pause ha de ser un numero entero igual o mayor que 0.
	// Info:			El teclado usado es este: http://giltesa.com/?p=11458 , según el tipo de resistencias habrá que cambiar las condicionales para adaptarse al valor leído desde el pin Analógico.

	int data = analogRead(0);

	if (data > 10)
	{
		if (data > 500 && data < 540)
			strcpy(resultButton,"up");
		else if (data > 330 && data < 370)
			strcpy(resultButton,"left");
		else if (data > 240 && data < 280)
			strcpy(resultButton,"bottom");
		else if (data > 190 && data < 230)
			strcpy(resultButton,"right");

		delay(pause);
	}
}