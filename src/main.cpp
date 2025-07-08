#include <Servo.h>  

#define TRIG_PIN     18  
#define ECHO_PIN     19  
#define IR1_PIN      23  
#define IR2_PIN      22  
#define IR3_PIN      21  
#define LED_SMALL    27  
#define LED_MEDIUM   26  
#define LED_LARGE    25  
#define SERVO_PIN    13  
#define ESTEIRA_PIN  14  
  
Servo cancela;  
  
long medirDistancia() {  
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2);  
  digitalWrite(TRIG_PIN, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(TRIG_PIN, LOW);  
  
  long dur = pulseIn(ECHO_PIN, HIGH, 20000);  
  long dist = (dur > 0) ? (dur * 0.034 / 2) : 999;  
  return dist;  
}  

void setup() {  
  Serial.begin(115200);  
  
  pinMode(TRIG_PIN, OUTPUT);  
  pinMode(ECHO_PIN, INPUT);  
  
  pinMode(IR1_PIN, INPUT);  
  pinMode(IR2_PIN, INPUT);  
  pinMode(IR3_PIN, INPUT);  
  
  pinMode(LED_SMALL, OUTPUT);  
  pinMode(LED_MEDIUM, OUTPUT);  
  pinMode(LED_LARGE, OUTPUT);  
  
  pinMode(ESTEIRA_PIN, OUTPUT);  
  digitalWrite(ESTEIRA_PIN, LOW);  
  
  cancela.attach(SERVO_PIN);  
  cancela.write(90);  
}  
  
void loop() {  
  long dist = medirDistancia();  

  if (dist < 10) {  
    Serial.println("Peça detectada → Ligando esteira");  
    digitalWrite(ESTEIRA_PIN, HIGH);  
    delay(1000);  

  int s1 = digitalRead(IR1_PIN);  
  int s2 = digitalRead(IR2_PIN);  
  int s3 = digitalRead(IR3_PIN);  
  int total = s1 + s2 + s3;  
  
  digitalWrite(LED_SMALL, LOW);  
  digitalWrite(LED_MEDIUM, LOW);  
  digitalWrite(LED_LARGE, LOW);  
  
  if (total == 3) {  
    cancela.write(180);  
    digitalWrite(LED_LARGE, HIGH);  
    Serial.println("Peça GRANDE → Cancela 180°");  
  } else if (total == 2) {  
      cancela.write(135);  
      digitalWrite(LED_MEDIUM, HIGH);  
      Serial.println("Peça MÉDIA → Cancela 135°");  
    } else if (total == 1) {  
      cancela.write(45);  
      digitalWrite(LED_SMALL, HIGH);  
      Serial.println("Peça PEQUENA → Cancela 45°");  
    } else {  
      Serial.println("Tamanho não identificado.");  
    }  

  delay(2000);  
  cancela.write(90);  
  digitalWrite(ESTEIRA_PIN, LOW);  
  Serial.println("Esteira desligada\n");  
  delay(1000);  
  }  
}  

