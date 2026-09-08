 // --- PINES MOTOR 1
const int enA = 9;      // Pin PWM para controlar la velocidad del motor 1
const int in1 = 8;      // Pin de dirección 1
const int in2 = 7;      // Pin de dirección 2

// --- PINES MOTOR 2
const int enB = 10;     // Pin PWM para controlar la velocidad del motor 2
const int in3 = 6;      // Pin de dirección 1 del motor 2
const int in4 = 5;      // Pin de dirección 2 del motor 2

void setup() {
  // Configurar los pines del motor1 como salida
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  // Configurar los pines del motor1 como salida
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Iniciar la comunicación serial (opcional, para ver valores en el monitor)
  Serial.begin(9600);
}

void loop() {

  Serial.println("Motor 1 - Forward");
  digitalWrite(enA, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(1000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  Serial.println("Motor 2 - Forward");
  digitalWrite(enB, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(1000);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

    Serial.println("Motor 1 - Reverse");
  digitalWrite(enA, HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(1000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  Serial.println("Motor 2 - Reverse");
  digitalWrite(enB, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
