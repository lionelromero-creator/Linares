/*
===========================================================
 CONTROL DEL ROBOT CON MANDO TIPO PLAYSTATION 4 (DIRECTO)
===========================================================

Modos
triangulo: cambia de modos

modo 1: linares 2026

    JOYSTICK

    L3 (Joystick izquierdo) = Control de movimiento/velocidad mediante el joystick derecha izquierda

    CONTROL DE VELOCIDAD:
        L2          = A = PWM -150
        R2          = Y = PWM 150
        ○ CÍRCULO   = O = PWM 254

modo 2: espinoza

El ESP32 recibe los datos directamente del Mando de PS4 por Bluetooth.
===========================================================
*/

#include <PS4Controller.h>

// =========================================================
// 2. VARIABLES DE BLUETOOTH Y PWM
// =========================================================

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

// Prototipos de funciones
void start_Movement(char direction, int pwmValue);
void stopMotors();
int detectorPWM(char direction, int pwmValue);

// =========================================================
// 5. CONFIGURACIÓN INICIAL
// =========================================================

void setup() {

  // Comunicación con el monitor serial
  Serial.begin(115200);

  // Iniciar la librería del mando de PS4
  // Nota: Debes vincular tu mando con la dirección MAC del ESP32 previamente
  PS4.begin();

  Serial.println("El ESP32 está listo para emparejarse con el mando de PS4");


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
  // Configuración del PWM (Sintaxis ESP32 Core v2.x / v3.x)
  // -------------------------------------------------------

  // Canal 0: Frecuencia = 5000 Hz, Resolución = 8 bits (0 - 255)
  ledcSetup(0, 5000, 8);
  ledcAttachPin(pwmA, 0);

  // Canal 1 para el motor B
  ledcSetup(1, 5000, 8);
  ledcAttachPin(pwmB, 1);


  delay(10);
}


// =========================================================
// 6. PROGRAMA PRINCIPAL
// =========================================================

void loop() {

  // Verifica si el mando de PS4 está conectado físicamente por Bluetooth
  if (PS4.isConnected()) {

      // =====================================================
      // ZONA PARA TU LÓGICA CON EL MANDO DE PS4
      // =====================================================
      // Aquí es donde debes programar qué hace el robot 
      // leyendo directamente el control, por ejemplo:
      // 
      // if (PS{ stopMotors(); }
      //
      // ¡Aquí te dejo la base para que crees tu lógica!
      // =====================================================4.Up()) { start_Movement('U'); }
      // else if (PS4.Down()) { start_Movement('D'); }
      // else 
      


  if (PS4.LStickX() > 15 || PS4.LStickX() < -15) {
    int lStickX = PS4.LStickX();
    int normalizedStick = constrain(abs(lStickX), 0, 255);

    if (lStickX > 15) {
      start_Movement('R', normalizedStick);
    } else {
      start_Movement('L', normalizedStick);
    }
  } else if (PS4.L2Value() > 15) {
    int l2Value = constrain(PS4.L2Value(), 0, 255);
    start_Movement('D', l2Value);
  } else if (PS4.R2Value() > 15) {
    int r2Value = constrain(PS4.R2Value(), 0, 255);
    start_Movement('U', r2Value);
  } else {
    stopMotors();
  }
  } else {
    // Si el mando se desconecta, paramos los motores por seguridad
    stopMotors();
  }

  // Pequeño retardo para estabilidad
  delay(10);
}


// =========================================================
//  ACTUALIZAR VELOCIDAD PWM

//  CONTROL DE MOVIMIENTO
// =========================================================

void start_Movement(char direction, int pwmValue) {

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

      digitalWrite(in4, HIGH);
      digitalWrite(in3, LOW);
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
      ledcWrite(0, pwmValue);

      // Motor B
      digitalWrite(in4, HIGH);
      digitalWrite(in3, LOW);
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

int detectorPWM(char direction, int pwmValue) {
  // Ajusta el valor de PWM para que quede siempre dentro del rango válido.
  (void)direction;
  return constrain(pwmValue, 0, 255);
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