/*
===========================================================
 CONTROL DEL ROBOT CON MANDO TIPO PLAYSTATION 4
===========================================================

MOVIMIENTO:
    ↑ D-PAD       = U = Adelante
    ↓ D-PAD       = D = Atrás
    ← D-PAD       = L = Izquierda
    → D-PAD       = R = Derecha

GIROS:
    □ CUADRADO    = H = Giro circular izquierda
    ○ CÍRCULO     = G = Giro circular derecha

DETENER:
    ✕ X            = S = Detener motores

CONTROL DE VELOCIDAD:
    L1             = P = PWM 120
    L2             = A = PWM 180
    R1             = B = PWM 200
    R2             = Y = PWM 230
    △ TRIÁNGULO    = O = PWM 254

El ESP32 recibe estos caracteres mediante Bluetooth
desde la aplicación de control.
===========================================================
*/

#include "BluetoothSerial.h"

// Se crea el objeto para comunicación Bluetooth
BluetoothSerial SerialBT;


// =========================================================
// 2. VARIABLES DE BLUETOOTH Y PWM
// =========================================================

// Guarda el último carácter recibido por Bluetooth
char dato = 0;

// Comando de PWM utilizado actualmente
char pwm = 'P';

// Valor de PWM inicial
int pwmValue = 120;


// =========================================================
// 3. PINES DE LOS MOTORES
// =========================================================

// MOTOR A
int in1 = 13;
int in2 = 12;
int pwmA = 26;

// MOTOR B
int in3 = 27;
int in4 = 14;
int pwmB = 25;


// =========================================================
// 4. OTROS PINES
// =========================================================

int led = 2;
int stop = 33;


// =========================================================
// 5. CONFIGURACIÓN INICIAL
// =========================================================

void setup() {

  // Comunicación con el monitor serial
  Serial.begin(115200);

  // Iniciar Bluetooth
  // El ESP32 aparecerá como "RSC_Test"
  SerialBT.begin("RSC_Test");

  Serial.println("El dispositivo está listo para conectarse");


  // -------------------------------------------------------
  // Configuración de pines de los motores
  // -------------------------------------------------------

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);


  // -------------------------------------------------------
  // Configuración de LED y pin STOP
  // -------------------------------------------------------

  pinMode(led, OUTPUT);
  pinMode(stop, OUTPUT);

  // Indica que el sistema está encendido
  digitalWrite(led, HIGH);
  digitalWrite(stop, HIGH);


  // -------------------------------------------------------
  // Configuración del PWM
  // -------------------------------------------------------

  // Canal 0:
  // Frecuencia = 5000 Hz
  // Resolución = 8 bits (0 - 255)
  ledcSetup(0, 5000, 8);

  // Conectar PWM del motor A al canal 0
  ledcAttachPin(pwmA, 0);


  // Canal 1 para el motor B
  ledcSetup(1, 5000, 8);

  // Conectar PWM del motor B al canal 1
  ledcAttachPin(pwmB, 1);


  delay(10);
}


// =========================================================
// 6. PROGRAMA PRINCIPAL
// =========================================================

void loop() {

  // Verifica si llegó algún dato por Bluetooth
  if (SerialBT.available() > 0) {

    // Leer el carácter recibido
    dato = SerialBT.read();

    // Mostrar en el monitor serial qué carácter llegó
    Serial.print("Dato recibido: ");
    Serial.println(dato);


    // -----------------------------------------------------
    // COMANDOS DE MOVIMIENTO
    // -----------------------------------------------------

    if (
      dato == 'U' ||
      dato == 'D' ||
      dato == 'R' ||
      dato == 'L' ||
      dato == 'G' ||
      dato == 'H'
    ) {

      // Ejecutar el movimiento
      start_Movement(dato);
    }


    // -----------------------------------------------------
    // COMANDOS PARA CAMBIAR EL PWM
    // -----------------------------------------------------

    else if (
      dato == 'P' ||
      dato == 'A' ||
      dato == 'B' ||
      dato == 'Y' ||
      dato == 'O'
    ) {

      // Guardar el nuevo comando PWM
      pwm = dato;

      // Convertir el comando en un valor numérico
      updatePWM();
    }


    // -----------------------------------------------------
    // COMANDO PARA DETENER LOS MOTORES
    // -----------------------------------------------------

    else if (dato == 'S') {

      stopMotors();
    }
  }


  // Pequeño retardo
  delay(10);
}


// =========================================================
// 7. ACTUALIZAR VELOCIDAD PWM
// =========================================================

void updatePWM() {

  // Dependiendo de la letra recibida,
  // se asigna un valor de PWM.

  switch (pwm) {

    // PWM = 120
    case 'P':
      pwmValue = 120;
      break;


    // PWM = 180
    case 'A':
      pwmValue = 180;
      break;


    // PWM = 200
    case 'B':
      pwmValue = 200;
      break;


    // PWM = 230
    case 'Y':
      pwmValue = 230;
      break;


    // PWM = 254
    case 'O':
      pwmValue = 254;
      break;


    // Si llega un comando no válido,
    // vuelve al PWM de 120
    default:
      pwmValue = 120;
      break;
  }


  // Mostrar el PWM actual en el monitor serial
  Serial.print("Valor de pwmValue actualizado: ");
  Serial.println(pwmValue);
}


// =========================================================
// 8. CONTROL DE MOVIMIENTO
// =========================================================

void start_Movement(char direction) {

  // Determina el movimiento según el carácter recibido.

  switch (direction) {


    // -----------------------------------------------------
    // ADELANTE
    // Comando: U
    // -----------------------------------------------------

    case 'U':

      // Motor A
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      ledcWrite(0, pwmValue);

      // Motor B
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      ledcWrite(1, pwmValue);

      break;


    // -----------------------------------------------------
    // ATRÁS
    // Comando: D
    // -----------------------------------------------------

    case 'D':

      // Motor A
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      ledcWrite(0, pwmValue);

      // Motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      ledcWrite(1, pwmValue);

      break;


    // -----------------------------------------------------
    // IZQUIERDA
    // Comando: L
    // -----------------------------------------------------

    case 'L':

      // Motor A
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);

      // Velocidad específica para girar
      ledcWrite(0, 140);


      // Motor B
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

      ledcWrite(1, 200);

      break;


    // -----------------------------------------------------
    // DERECHA
    // Comando: R
    // -----------------------------------------------------

    case 'R':

      // Motor A
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);

      ledcWrite(0, 200);


      // Motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

      ledcWrite(1, 140);

      break;


    // -----------------------------------------------------
    // GIRO 1
    // Comando: H
    // -----------------------------------------------------

    case 'H':

      // Motor A
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      ledcWrite(0, pwmValue);


      // Motor B
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      ledcWrite(1, pwmValue);

      break;


    // -----------------------------------------------------
    // GIRO 2
    // Comando: G
    // -----------------------------------------------------

    case 'G':

      // Motor A
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      ledcWrite(0, pwmValue);


      // Motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      ledcWrite(1, pwmValue);

      break;
  }


  // Mostrar movimiento y PWM utilizado
  Serial.print("Movimiento ");
  Serial.print(direction);
  Serial.print(" con PWM: ");
  Serial.println(pwmValue);
}


// =========================================================
// 9. DETENER LOS MOTORES
// =========================================================

void stopMotors() {

  // Detener Motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  ledcWrite(0, 0);


  // Detener Motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  ledcWrite(1, 0);


  // Mostrar mensaje en el monitor serial
  Serial.println("Motores detenidos.");
}