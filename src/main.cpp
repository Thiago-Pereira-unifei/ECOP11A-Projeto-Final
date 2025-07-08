#include <Servo.h> // Inclui a biblioteca Servo, necessária para controlar o servo motor.

// --- Definições de Pinos ---
// Estas linhas definem os números dos pinos do Arduino aos quais os componentes estão conectados.
// O uso de #define torna o código mais legível e fácil de modificar, pois você pode mudar o pino
// em um único lugar e a alteração se propagará por todo o código.

// Pinos para o sensor ultrassônico (HC-SR04)
#define TRIG_PIN     18 // Pino "Trigger" do sensor ultrassônico. Envia o pulso sonoro.
#define ECHO_PIN     19 // Pino "Echo" do sensor ultrassônico. Recebe o pulso sonoro refletido.

// Pinos para os sensores infravermelhos (IR)
// Estes sensores são usados para detectar a presença da peça e, indiretamente, seu tamanho.
#define IR1_PIN      23 // Sensor IR 1
#define IR2_PIN      22 // Sensor IR 2
#define IR3_PIN      21 // Sensor IR 3

// Pinos para os LEDs indicadores de tamanho
// Cada LED acenderá para indicar o tamanho classificado da peça.
#define LED_SMALL    27 // LED para indicar peça pequena
#define LED_MEDIUM   26 // LED para indicar peça média
#define LED_LARGE    25 // LED para indicar peça grande

// Pino para o servo motor
#define SERVO_PIN    13 // Pino de controle do servo motor, que atua como uma cancela.

// Pino para o motor da esteira
#define ESTEIRA_PIN  14 // Pino de controle do motor da esteira transportadora.

// --- Objeto Servo ---
Servo cancela; // Cria uma instância do objeto Servo, que será usada para controlar o servo motor da cancela.

// --- Função: medirDistancia ---
// Esta função mede a distância de um objeto usando o sensor ultrassônico HC-SR04.
long medirDistancia() {
  // Garante que o pino TRIG esteja em estado LOW antes de enviar um novo pulso.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2); // Pequeno atraso para garantir um pulso limpo.

  // Envia um pulso HIGH de 10 microssegundos para o pino TRIG.
  // Este pulso é o que faz o sensor ultrassônico emitir uma onda sonora.
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mede a duração do pulso no pino ECHO.
  // pulseIn() retorna o tempo em microssegundos que o pino ECHO permanece HIGH.
  // O timeout de 20000 microssegundos (20ms) evita que o programa trave se nenhum eco for detectado.
  long dur = pulseIn(ECHO_PIN, HIGH, 20000);

  // Calcula a distância em centímetros.
  // A velocidade do som no ar é de aproximadamente 0.034 cm/microsegundo.
  // Dividimos por 2 porque o pulso sonoro percorre a distância duas vezes (ida e volta).
  // Se 'dur' for 0 (indicando timeout ou nenhuma detecção), a distância é definida como 999 (um valor alto para indicar "longe").
  long dist = (dur > 0) ? (dur * 0.034 / 2) : 999;
  return dist; // Retorna a distância calculada.
}

// --- Função: setup ---
// A função setup() é executada uma vez quando o Arduino é ligado ou resetado.
// É usada para inicializar pinos, comunicação serial e outros componentes.
void setup() {
  Serial.begin(115200); // Inicia a comunicação serial a 115200 bits por segundo para depuração e mensagens no Monitor Serial.

  // Configura os pinos do sensor ultrassônico.
  pinMode(TRIG_PIN, OUTPUT); // O pino TRIG é configurado como saída para enviar pulsos.
  pinMode(ECHO_PIN, INPUT);  // O pino ECHO é configurado como entrada para receber pulsos.

  // Configura os pinos dos sensores infravermelhos como entrada.
  // Eles lerão se há um objeto presente (HIGH) ou não (LOW).
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(IR3_PIN, INPUT);

  // Configura os pinos dos LEDs como saída.
  // Eles serão controlados para acender ou apagar.
  pinMode(LED_SMALL, OUTPUT);
  pinMode(LED_MEDIUM, OUTPUT);
  pinMode(LED_LARGE, OUTPUT);

  // Configura o pino do motor da esteira como saída.
  pinMode(ESTEIRA_PIN, OUTPUT);
  digitalWrite(ESTEIRA_PIN, LOW);  // Garante que o motor da esteira esteja desligado ao iniciar.

  // Associa o objeto 'cancela' ao pino especificado para o servo motor.
  cancela.attach(SERVO_PIN);
  cancela.write(90); // Move a cancela para a posição neutra (90 graus) ao iniciar o programa.
}

// --- Função: loop ---
// A função loop() é executada repetidamente e continuamente após a função setup() ser concluída.
// Contém a lógica principal do programa.
void loop() {
  long dist = medirDistancia(); // Chama a função para medir a distância de um objeto.

  // Verifica se uma peça foi detectada pelo sensor ultrassônico (distância menor que 10 cm).
  if (dist < 10) {
    Serial.println("Peça detectada → Ligando esteira"); // Informa no Monitor Serial que uma peça foi detectada.
    digitalWrite(ESTEIRA_PIN, HIGH);  // Liga o motor da esteira para mover a peça.
    delay(1000); // Aguarda 1 segundo para a peça se mover e passar pelos sensores IR.

    // --- Leitura dos Sensores IR ---
    // Lê o estado de cada sensor infravermelho.
    // Se o sensor detectar uma peça, ele retorna HIGH (1); caso contrário, LOW (0).
    int s1 = digitalRead(IR1_PIN);
    int s2 = digitalRead(IR2_PIN);
    int s3 = digitalRead(IR3_PIN);
    int total = s1 + s2 + s3; // Soma as leituras dos sensores. A soma indicará o tamanho da peça.
                               // Por exemplo, se 'total' for 3, todos os sensores foram ativados (peça grande).

    // Desliga todos os LEDs antes de determinar qual deve ser aceso.
    digitalWrite(LED_SMALL, LOW);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_LARGE, LOW);

    // --- Lógica de Classificação e Ação da Cancela ---
    // Baseado na soma das leituras dos sensores IR, o programa classifica o tamanho da peça
    // e move a cancela para a posição correspondente.
    if (total == 3) { // Se todos os 3 sensores detectaram (provavelmente uma peça GRANDE).
      cancela.write(180); // Move a cancela para 180 graus.
      digitalWrite(LED_LARGE, HIGH); // Acende o LED que indica peça grande.
      Serial.println("Peça GRANDE → Cancela 180°"); // Imprime a classificação no Monitor Serial.
    } else if (total == 2) { // Se 2 sensores detectaram (provavelmente uma peça MÉDIA).
      cancela.write(135); // Move a cancela para 135 graus.
      digitalWrite(LED_MEDIUM, HIGH); // Acende o LED que indica peça média.
      Serial.println("Peça MÉDIA → Cancela 135°"); // Imprime a classificação no Monitor Serial.
    } else if (total == 1) { // Se apenas 1 sensor detectou (provavelmente uma peça PEQUENA).
      cancela.write(45); // Move a cancela para 45 graus.
      digitalWrite(LED_SMALL, HIGH); // Acende o LED que indica peça pequena.
      Serial.println("Peça PEQUENA → Cancela 45°"); // Imprime a classificação no Monitor Serial.
    } else { // Caso a soma seja 0 ou outro valor inesperado.
      Serial.println("Tamanho não identificado."); // Informa que o tamanho não pôde ser identificado.
    }

    delay(2000);  // Aguarda 2 segundos para a peça passar pela cancela e ser direcionada.
    cancela.write(90);  // Retorna a cancela para a posição neutra (90 graus), pronta para a próxima peça.
    digitalWrite(ESTEIRA_PIN, LOW); // Desliga o motor da esteira.
    Serial.println("Esteira desligada\n"); // Informa que a esteira foi desligada e adiciona uma linha vazia para clareza.
    delay(1000); // Pequeno atraso antes de verificar a próxima peça, evitando leituras falsas.
  }
}
