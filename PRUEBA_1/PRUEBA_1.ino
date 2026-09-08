// Definición de pines
const int joyPin = A0;  // Pin analógico conectado al eje Y del joystick
const int enA = 9;      // Pin PWM para controlar la velocidad del motor
const int in1 = 8;      // Pin de dirección 1
const int in2 = 7;      // Pin de dirección 2

void setup() {
  // Configurar los pines del motor como salida
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  // Iniciar la comunicación serial (opcional, para ver valores en el monitor)
  Serial.begin(9600);
}

void loop() {
  // 1. Leer el valor del joystick (rango de 0 a 1023)
  int joyVal = analogRead(joyPin);
  
  // Imprimir el valor para calibración (puedes verlo en el Monitor Serie)
  Serial.println(joyVal);

  // 2. Lógica de control
  // El centro del joystick suele dar un valor cercano a 512.
  // Creamos una "zona muerta" entre 470 y 550 para que el motor no vibre si el joystick no está perfectamente centrado.

  if (joyVal > 550) {
    // MOVIMIENTO HORARIO (Hacia adelante)
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    
    // Mapear el valor del joystick (550 a 1023) a un valor de velocidad PWM (0 a 255)
    int velocidad = map(joyVal, 550, 1023, 0, 100);
    analogWrite(enA, velocidad);
    
  } else if (joyVal < 470) {
    // MOVIMIENTO ANTIHORARIO (Hacia atrás)
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    
    // Mapear el valor del joystick (470 a 0) a un valor de velocidad PWM (0 a 255)
    // Nota: Invertimos los valores para que al empujar más abajo (cerca de 0), la velocidad aumente
    int velocidad = map(joyVal, 470, 0, 0, 100);
    analogWrite(enA, velocidad);
    
  } else {
    // MOTOR DETENIDO (Posición central)
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
  }
}