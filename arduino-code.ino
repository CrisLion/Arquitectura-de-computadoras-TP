#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
//leds
int led1=13;

//pirs
int a=0;
int b=0;
int c=0;
int d=0;

//piezo
int piezo=12;
bool valorTeclado=false;

//sensor Sonido
int pinDesencadenador=2; //trig emite onda de sonido
int pinEco=3; // recibe la onda de sonido

float tiempoSonido=0;
float distanciaSonido=0;
char clave[]="1245";
char codigo[4];
int opc=0;
int cont=0;


//---Teclado
const byte ROWS = 4; //filas
const byte COLS = 4; //columnas

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11, 10, 9, 8}; //conexion con pines
byte colPins[COLS] = {7, 6, 5, 4}; //conexion pines keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
byte i=0;
//---Pantalla
//porcion de memoria, cantidad de caracteres, cantidad de lineas
LiquidCrystal_I2C lcd(34, 16, 2);


void setup()
{
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
//piezo
  pinMode(piezo,OUTPUT);

  pinMode(pinEco,INPUT);
  pinMode(pinDesencadenador, OUTPUT);
   //pantalla
  lcd.init(); //se inicializa
  lcd.backlight(); //para que se prenda

  //teclado
  Serial.println("Teclado 4x4");
  for( i=0;i<8;i++){
  	pinMode(i,OUTPUT);
  }
}

void loop()
{

	menu();
	
	//pir1
	a=analogRead(A0);
	b=map(a,0,1023,0,255);
	//pir2
	c=analogRead(A1);
	d=map(c,0,1023,0,255);

	//sensor sonido
	Ultrasound_Working();
  
	//tecla presionada
	char pulsacion=keypad.getKey();
	if(pulsacion){
		lcd.clear();
		lcd.print("Opcion:");
		lcd.setCursor(0,1);
		lcd.print(pulsacion);
		Serial.print("Tecla");
		Serial.println(pulsacion);

	}
	if(b>100||d>100){
		Serial.println("Movimiento Detectado");
		Serial.println(b);
		digitalWrite(led1, HIGH);
		tone(piezo, 523,200);

	}
	else{
		digitalWrite(led1, LOW);
		noTone(piezo);
	}
	
	
	if (pulsacion != NO_KEY){
		codigo[cont]=pulsacion;
		Serial.print(codigo[cont]);
		cont++;
		if(cont==4){
		if(codigo[0]==clave[0]&&codigo[1]==clave[1]&&codigo[2]==clave[2]&&codigo[3]==clave[3]){
			Serial.println("Clave correcta");
			lcd.setCursor(0,1);
			lcd.print("Clave correcta");
			delay(10);
		}
		else{
			Serial.println("Clave Incorrecta");
			lcd.setCursor(0,1);
			lcd.print("Clave Incorrecta");
			delay(10);
		}
		cont=0;
		}
	}
}

void menu(){
	lcd.setCursor(0,0);
	lcd.print("Elige una opcion");
	lcd.setCursor(0,1);
	lcd.print("1 Encender alarma");
	delay(200);
	lcd.clear();
	
	lcd.setCursor(0,0);
	lcd.print("2 1/2 alarma"); //REMOVE
	lcd.setCursor(0,1);
	lcd.print("3 Cambiar clave");
	delay(200);
	lcd.clear();

	lcd.setCursor(0,0);
	lcd.print("4 Apagar Alarma ");
	lcd.setCursor(0,1);
	lcd.print(">>> ");
	delay(200);
	lcd.clear();
}

void Ultrasound_Working(){

	digitalWrite(pinDesencadenador,HIGH);
	delay(1);//emite 1 pulso
	digitalWrite(pinDesencadenador,LOW);
	tiempoSonido=pulseIn(pinEco, HIGH);
	distanciaSonido=tiempoSonido/52.8;

	if(distanciaSonido<130){//120cm
		digitalWrite(led1,HIGH);
		tone(piezo, 523,200);
		Serial.println("sonido Detectado");
	}
	else{
		digitalWrite(led1,LOW);
		noTone(piezo);
	}
}