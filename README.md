# ECOP11A - Projeto Final - Introdução a Engenharia
Introdução a Programacao: Projeto final com introducao a engenharia e metodo cientifico

## Equipe

-Higor Ribeiro  
-João Kinjo  
-Thiago Pereira  
-Tulio Franciscon  
-Weister Oliveira  

## Objetivo

O objetivo principal deste projeto é criar um sistema automatizado para classificar peças em três categorias de tamanho (pequena, média e grande) e direcioná-las usando um servo motor.  

Para alcançar esse objetivo, o sistema utiliza os seguintes componentes e lógicas:  

Detecção de Presença: Um sensor ultrassônico é usado para detectar a presença de uma peça quando ela se aproxima a menos de 10 cm do sensor.  
Classificação de Tamanho: Três sensores infravermelhos são estrategicamente posicionados para identificar o tamanho da peça com base em quantos sensores são ativados simultaneamente:  
Peça Pequena: Ativa apenas um sensor IR.  
Peça Média: Ativa dois sensores IR.  
Peça Grande: Ativa todos os três sensores IR.  
Indicação Visual: Três LEDs indicam visualmente a categoria de tamanho da peça detectada.  
Direcionamento Automatizado: Um servo motor (cancela) atua como um mecanismo de direcionamento, movendo-se para uma posição específica correspondente ao tamanho da peça detectada, presumivelmente para direcioná-la para diferentes compartimentos ou esteiras.  
Controle e Log: O código do ESP32 gerencia a leitura dos sensores, a lógica de classificação, o controle do servo e a comunicação serial para depuração e monitoramento.  
Em resumo, o projeto simula um sistema de triagem simples que pode ser aplicado em linhas de produção para automatizar a separação de objetos com base em seu tamanho.  

## Hipótese

Se um sistema automatizado for implementado utilizando sensores ultrassônicos e infravermelhos conectados a um microcontrolador ESP32, então será possível detectar a presença de peças em uma esteira, classificá-las de acordo com o seu tamanho (pequena, média ou grande) e direcioná-las adequadamente por meio de um servo motor, garantindo maior eficiência e precisão no processo de triagem automatizada em linhas de produção.

## Código

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


## Conclusão

O sistema desenvolvido atendeu com sucesso ao objetivo proposto, demonstrando ser capaz de identificar a presença de peças utilizando um sensor ultrassônico e classificá-las corretamente com base no número de sensores infravermelhos ativados. A lógica implementada permitiu controlar LEDs indicativos e um servo motor que atuou como uma cancela direcional, redirecionando as peças conforme seu tamanho. Além disso, o acionamento automático da esteira garantiu o fluxo contínuo das peças durante o processo. Portanto, conclui-se que a hipótese foi validada: é viável utilizar sensores simples e um microcontrolador ESP32 para criar um sistema eficiente de triagem de peças, com potencial aplicação em ambientes industriais e educacionais.

## Diagrama da montagem na ESP32

<p align="center">
  <img src="https://github.com/user-attachments/assets/a7ed25e5-d98b-4da0-a22d-dfd27c382ade" alt="Imagem centralizada" width="400">
</p>

## Diagrama em blocos

<p align="center">
  <img src="https://github.com/user-attachments/assets/ca7d7462-ce95-42d5-a282-6a5ed5bb6f97" alt="Imagem centralizada" width="400">
</p>
Para melhor visualizacao do diagrama, segue o link: https://miro.com/welcomeonboard/Zjh4YUNFWVlRbGUwYThXUnJKWkRTdEx2UE9zZlhVSllwTVlyWmlpZlc4aXdXNG8xM0xQSTAvV0ZYUHJWUU5SWEZzRFdWYkdYcndqZC96emRGYjlpbDRTMytnLzVBTWdVZGs2ckIyY211ZzFHTGRaazVxNnBoU3U5dG5wdXpuTUJhWWluRVAxeXRuUUgwWDl3Mk1qRGVRPT0hdjE=?share_link_id=345214892846

