// --- PINES MOTOR 1 (Tu código original) ---
const int joyPin = A0;  // Pin analógico conectado al eje Y del joystick
const int enA = 9;      // Pin PWM para controlar la velocidad del motor 1
const int in1 = 8;      // Pin de dirección 1
const int in2 = 7;      // Pin de dirección 2

// --- PINES MOTOR 2 (Agregados para el botón) ---
const int btnPin = 2;   // Pin digital conectado al botón (SW) del joystick
const int enB = 10;     // Pin PWM para controlar la velocidad del motor 2
const int in3 = 6;      // Pin de dirección 1 del motor 2
const int in4 = 5;      // Pin de dirección 2 del motor 2

const int velAuto = 200; // Velocidad del ciclo del Motor 2 (0 a 255)

void setup() {
  // Configurar los pines del motor 1 como salida
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Configurar los pines del motor 2 como salida
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Asegurar que el Motor 2 inicie apagado
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);

  // Configurar botón del joystick
  pinMode(btnPin, INPUT_PULLUP); 

  // Iniciar la comunicación serial
  Serial.begin(9600);
}

void loop() {
  // =========================================================
  // 1. LÓGICA ORIGINAL PARA EL MOTOR 1 (Palanca Joystick)
  // =========================================================
  int joyVal = analogRead(joyPin);
  Serial.println(joyVal);

  if (joyVal > 550) {
    // MOVIMIENTO HORARIO (Hacia adelante)
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    
    int velocidad = map(joyVal, 550, 1023, 0, 100);
    analogWrite(enA, velocidad);
    
  } else if (joyVal < 470) {
    // MOVIMIENTO ANTIHORARIO (Hacia atrás)
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    
    int velocidad = map(joyVal, 470, 0, 0, 100);
    analogWrite(enA, velocidad);
    
  } else {
    // MOTOR DETENIDO (Posición central)
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
  }

  // =========================================================
  // 2. LÓGICA NUEVA PARA EL MOTOR 2 (Botón Joystick)
  // =========================================================
  if (digitalRead(btnPin) == LOW) {
    // Giro Horario por 3 segundos
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, velAuto);
    delay(3000);

    // Giro Antihorario por 3 segundos
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, velAuto);
    delay(3000);

    // Apagar Motor 2 al finalizar el ciclo
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);

    // Esperar a que se suelte el botón
    while (digitalRead(btnPin) == LOW) {
      delay(10);
    }
  } else {
    // Mantener apagado el Motor 2 si no se pulsa el botón
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);
  }
}