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

#include <Servo.h> // Inclui a biblioteca Servo para controlar o servo motor

// --- Definições de Pinos ---
// Pinos para o sensor ultrassônico (HC-SR04)
#define TRIG_PIN     18 // Pino Trigger do sensor ultrassônico
#define ECHO_PIN     19 // Pino Echo do sensor ultrassônico

// Pinos para os sensores infravermelhos (IR)
#define IR1_PIN      23  // Sensor IR 1 (geralmente para a menor dimensão da peça)
#define IR2_PIN      22  // Sensor IR 2 (para a dimensão média da peça)
#define IR3_PIN      21  // Sensor IR 3 (para a maior dimensão da peça)

// Pinos para os LEDs indicadores de tamanho
#define LED_SMALL    27 // LED que acende para peças pequenas
#define LED_MEDIUM   26 // LED que acende para peças médias
#define LED_LARGE    25 // LED que acende para peças grandes

// Pino para o servo motor que atua como cancela
#define SERVO_PIN    13

// Pino para o motor da esteira
#define ESTEIRA_PIN  14  // Pino de controle do motor da esteira

// --- Objeto Servo ---
Servo cancela; // Cria um objeto Servo para controlar a cancela

// --- Função: medirDistancia ---
// Mede a distância usando o sensor ultrassônico
long medirDistancia() {
  // Garante que o pino TRIG esteja LOW no início
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2); // Pequeno atraso para garantir um pulso limpo

  // Envia um pulso HIGH de 10 microssegundos para o TRIG
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mede a duração do pulso no pino ECHO (tempo que o som leva para ir e voltar)
  // O timeout de 20000 microssegundos (20ms) evita que o programa trave se nenhum eco for detectado
  long dur = pulseIn(ECHO_PIN, HIGH, 20000);

  // Calcula a distância em centímetros
  // A velocidade do som no ar é de aproximadamente 0.034 cm/microsegundo.
  // Dividimos por 2 porque o pulso percorre a distância duas vezes (ida e volta).
  // Se dur for 0 (timeout), retorna 999 para indicar que nada foi detectado.
  long dist = (dur > 0) ? (dur * 0.034 / 2) : 999;
  return dist;
}

// --- Função: setup ---
// Configura os pinos e inicializa a comunicação serial
void setup() {
  Serial.begin(115200); // Inicia a comunicação serial a 115200 bps para depuração

  // Configura os pinos do sensor ultrassônico
  pinMode(TRIG_PIN, OUTPUT); // TRIG como saída
  pinMode(ECHO_PIN, INPUT);  // ECHO como entrada

  // Configura os pinos dos sensores infravermelhos como entrada
  // Não é necessário pullup interno se os sensores já tiverem resistores de pullup/pulldown
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(IR3_PIN, INPUT);

  // Configura os pinos dos LEDs como saída
  pinMode(LED_SMALL, OUTPUT);
  pinMode(LED_MEDIUM, OUTPUT);
  pinMode(LED_LARGE, OUTPUT);

  // Configura o pino do motor da esteira como saída
  pinMode(ESTEIRA_PIN, OUTPUT);
  digitalWrite(ESTEIRA_PIN, LOW);  // Garante que a esteira esteja desligada ao iniciar

  // Associa o objeto Servo ao pino especificado
  cancela.attach(SERVO_PIN);
  cancela.write(90); // Move a cancela para a posição neutra (90 graus)
}

// --- Função: loop ---
// Onde o programa principal é executado repetidamente
void loop() {
  long dist = medirDistancia(); // Mede a distância atual

  // Verifica se uma peça foi detectada (distância menor que 10 cm)
  if (dist < 10) {
    Serial.println("Peça detectada → Ligando esteira");
    digitalWrite(ESTEIRA_PIN, HIGH);  // Liga o motor da esteira
    delay(1000); // Aguarda 1 segundo para a peça se mover e passar pelos sensores IR

    // --- Leitura dos Sensores IR ---
    // Os sensores IR retornam HIGH quando detectam uma peça (presença de luz refletida)
    int s1 = digitalRead(IR1_PIN);
    int s2 = digitalRead(IR2_PIN);
    int s3 = digitalRead(IR3_PIN);
    int total = s1 + s2 + s3; // Soma as leituras (0 ou 1) para determinar o tamanho

    // Desliga todos os LEDs antes de acender o correto
    digitalWrite(LED_SMALL, LOW);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_LARGE, LOW);

    // --- Lógica de Classificação e Ação da Cancela ---
    if (total == 3) { // Se todos os 3 sensores detectaram (peça grande)
      cancela.write(180); // Move a cancela para 180 graus
      digitalWrite(LED_LARGE, HIGH); // Acende o LED de peça grande
      Serial.println("Peça GRANDE → Cancela 180°");
    } else if (total == 2) { // Se 2 sensores detectaram (peça média)
      cancela.write(135); // Move a cancela para 135 graus
      digitalWrite(LED_MEDIUM, HIGH); // Acende o LED de peça média
      Serial.println("Peça MÉDIA → Cancela 135°");
    } else if (total == 1) { // Se apenas 1 sensor detectou (peça pequena)
      cancela.write(45); // Move a cancela para 45 graus
      digitalWrite(LED_SMALL, HIGH); // Acende o LED de peça pequena
      Serial.println("Peça PEQUENA → Cancela 45°");
    } else { // Se nenhum sensor ou mais de 3 (situação inesperada)
      Serial.println("Tamanho não identificado.");
    }

    delay(2000);  // Aguarda 2 segundos para a peça passar pela cancela
    cancela.write(90);  // Retorna a cancela para a posição neutra (90 graus)
    digitalWrite(ESTEIRA_PIN, LOW); // Desliga o motor da esteira
    Serial.println("Esteira desligada\n"); // Imprime uma linha vazia para melhor visualização
    delay(1000); // Pequeno atraso antes de procurar a próxima peça
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

