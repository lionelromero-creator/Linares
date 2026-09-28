/*
===========================================

CONSIDERACIONES:
Para realizar el movimiento del robot (Adelante, atrás, izquierda y derecha)
se envian caracteres mediante una APK diseñada para el control de motores
y son los siguientes:

ADELANTE = U (UP)
ATRAS = D (DOWN)
IZQUIERDA = L (LEFT)
DERECHA = R (RIGHT)
DETENER MOTORES = S (STOP)

GIRO 1 = H (MOVIMIENTO CIRCULAR - IZQUIERDA)
GIRO 2 = G (MOVIMIENTO CIRCULAR - DERECHA)
Ello está detallado en el void loop() > SerialBT.available() > 0

Además de ello se agregaron caracteres para el control de PWM de los motores
y son los siguientes:

P=120 / A=180 / B=200 / Y=230 / O=254
Al encender el robot el valor por defecto asignado al PWM será de 120
Ello está detallado en el void updatePWM()

===========================================
*/

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

char dato = 0;
char pwm = 'P'; // Valor por defecto
int pwmValue = 120; // Valor de PWM por defecto 120

int in1 = 13; // MOTOR A
int in2 = 12; // MOTOR A
int pwmA = 26; // PWM MOTOR A

int in3 = 27; // MOTOR B
int in4 = 14;  // MOTOR B
int pwmB = 25; // PWM MOTOR B

int led = 2;
int stop = 33;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("RSC_Test"); // Nombre del Bluetooth Ekeko
  Serial.println("El dispositivo está listo para conectarse");

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);

  pinMode(led, OUTPUT);
  pinMode(stop, OUTPUT);
  digitalWrite(led, HIGH);  // Indica estado ON
  digitalWrite(stop, HIGH);  // Indica estado ON

  ledcSetup(0, 5000, 8); // canal / frecuencia / resolución
  ledcAttachPin(pwmA, 0); // pwm conectado al canal 0
  ledcSetup(1, 5000, 8);
  ledcAttachPin(pwmB, 1);
  delay(10);

}

void loop() {
  if (SerialBT.available() > 0) { 
    dato = SerialBT.read();
    Serial.print("Dato recibido: ");
    Serial.println(dato);

    if (dato == 'U' || dato == 'D' || dato == 'R' || dato == 'L' || dato == 'G' || dato == 'H') {
      // Actualizar PWM basado en el último valor recibido
      start_Movement(dato);
    } else if (dato == 'P' || dato == 'A' || dato == 'B' || dato == 'Y' || dato == 'O') {
      pwm = dato; // Guardar el nuevo valor PWM
      updatePWM(); // Actualizar el valor de pwmValue
    } else if (dato == 'S') {
      stopMotors();
    }
  }
  delay(10); // Retardo pequeño para evitar sobrecarga de procesamiento
}

void updatePWM() {
  // Asignar valor de PWM basado en el comando recibido
  switch (pwm) {
    case 'P':
      pwmValue = 120;
      break;
    case 'A':
      pwmValue = 180;
      break;
    case 'B':
      pwmValue = 200;
      break;
    case 'Y':
      pwmValue = 230;
      break;
    case 'O':
      pwmValue = 254;
      break;
    default:
      pwmValue = 120; // Valor por defecto si no coincide con ningún comando esperado 120
      break;
  }
  Serial.print("Valor de pwmValue actualizado: ");
  Serial.println(pwmValue);
}

void start_Movement(char direction) {
  // Configura los motores basados en la dirección y el valor de PWM
  switch (direction) {
    case 'U': // Arriba
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      ledcWrite(0, pwmValue);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      ledcWrite(1, pwmValue);
      break;
    case 'D': // Abajo
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      ledcWrite(0, pwmValue);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      ledcWrite(1, pwmValue);
      break;
    case 'L': // Izquierda
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      ledcWrite(0, 140); 
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      ledcWrite(1, 200); //1, pwmValue
      break;
    case 'R': // Derecha
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      ledcWrite(0, 200); //0, pwmValue
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      ledcWrite(1, 140); 
      break;
    case 'H': // Giro 1
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      ledcWrite(0, pwmValue);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      ledcWrite(1, pwmValue);
      break;
    case 'G': // Giro 2
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      ledcWrite(0, pwmValue);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      ledcWrite(1, pwmValue);
      break;
  }
  Serial.print("Movimiento ");
  Serial.print(direction);
  Serial.print(" con PWM: ");
  Serial.println(pwmValue);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  ledcWrite(0, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  ledcWrite(1, 0);
  
  Serial.println("Motores detenidos.");
