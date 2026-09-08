// --- PINES MOTOR 1 (Joystick Eje Y) ---
const int joyPin = A0;  
const int enA = 9;      // PWM Motor 1
const int in1 = 8;      // Dirección 1 Motor 1
const int in2 = 7;      // Dirección 2 Motor 1

// --- PINES MOTOR 2 (Botón Joystick) ---
const int btnPin = 2;   // Botón de Inicio (SW del Joystick)
const int enB = 10;     // PWM Motor 2
const int in3 = 6;      // Dirección 1 Motor 2
const int in4 = 5;      // Dirección 2 Motor 2

// --- PIN RELÉ ---
const int pinRele = 11; // Pin de control del relé

// --- PIN FINAL DE CARRERA / PARO DE EMERGENCIA ---
const int btnStopPin = 12; // Final de carrera (conectado a GND)

const int velAuto = 200; // Velocidad Motor 2 (0 a 255)

// ---------------------------------------------------------
// FUNCIÓN PARA APAGAR AMBOS MOTORES Y EL RELÉ DE INMEDIATO
// ---------------------------------------------------------
void apagarTodo() {
  // Apagar Motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);

  // Apagar Motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);

  // Apagar Relé
  digitalWrite(pinRele, LOW); // Si tu relé es active LOW, cambia este LOW por HIGH
}

// ---------------------------------------------------------
// FUNCIÓN DE ESPERA CON MONITOREO DE PARO
// Retorna 'true' si se presionó el final de carrera
// Retorna 'false' si transcurrió todo el tiempo sin interrupción
// ---------------------------------------------------------
bool esperarOParar(unsigned long tiempoMs) {
  unsigned long inicio = millis();
  while (millis() - inicio < tiempoMs) {
    // Si se presiona el final de carrera
    if (digitalRead(btnStopPin) == LOW) {
      apagarTodo();
      return true; // Se interrumpió el proceso
    }
    delay(5); // Pequeña pausa para filtrar lecturas erróneas
  }
  return false; // El tiempo se completó normalmente
}

void setup() {
  // Configuración Motor 1
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Configuración Motor 2
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Configuración Relé
  pinMode(pinRele, OUTPUT);

  // Configuración Botones y Final de Carrera (Pull-Up interno)
  pinMode(btnPin, INPUT_PULLUP); 
  pinMode(btnStopPin, INPUT_PULLUP);

  // Estado inicial seguro
  apagarTodo();

  Serial.begin(9600);
}

void loop() {

  // =========================================================
  // 1. MOTOR 1 (Joystick) - Bloqueado si el final de carrera está activo
  // =========================================================
  if (digitalRead(btnStopPin) == LOW) {
    // Si se presiona el final de carrera, apaga obligatoriamente el Motor 1
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
  } 
  else {
    // Si el final de carrera no está activo, el joystick opera con normalidad
    int joyVal = analogRead(joyPin);

    if (joyVal > 550) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(enA, map(joyVal, 550, 1023, 0, 100));
    } 
    else if (joyVal < 470) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(enA, map(joyVal, 470, 0, 0, 100));
    } 
    else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(enA, 0);
    }
  }

  // =========================================================
  // 2. MOTOR 2 Y RELÉ (Ciclo automático)
  // =========================================================

  // Si el final de carrera está presionado, garantizamos apagado global
  if (digitalRead(btnStopPin) == LOW) {
    apagarTodo();
  }
  // Si se presiona el botón de Inicio (Pin 2) y el final de carrera está libre
  else if (digitalRead(btnPin) == LOW) {

    bool interrumpido = false;

    // A. Encender Relé
    digitalWrite(pinRele, HIGH);

    // B. Giro Horario por 3 segundos
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, velAuto);

    interrumpido = esperarOParar(3000);

    // C. Giro Antihorario por 3 segundos (solo si no se activó el final de carrera)
    if (!interrumpido) {
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enB, velAuto);

      interrumpido = esperarOParar(3000);
    }

    // D. Finalizar y apagar todo
    apagarTodo();

    // E. Esperar a que se libere el final de carrera o botón de inicio antes de reanudar
    while (digitalRead(btnPin) == LOW || digitalRead(btnStopPin) == LOW) {
      apagarTodo(); // Mantiene apagado todo mientras siga presionado
      delay(10);
    }
  }
}