código de apresentação da primeira etapa:
#include <Servo.h> // Inclui a biblioteca Servo.h

Servo Esc30A; // Cria um objeto Servo para controlar o motor
const int pino_pot = 0; // Define o pino analógico utilizado para o potenciômetro
const int pino_motor = 9; // Define o pino PWM utilizado para o motor
const int pino_pulse = 2; // Define o pino digital ligado em uma das fases do motor
int Speed; // Variável para armazenar a velocidade do motor
int pulseCount = 0; // Variável para armazenar o número de pulsos contados
float RPM; // Variável para armazenar a velocidade em rotações por minuto
unsigned long lastTime = 0; // Variável para armazenar a última vez que um pulso foi detectado
float pulses_per_rotation = 7; // Número de pulsos por rotação do motor

// Função para contar os pulsos 
void countPulses() {
  pulseCount++; // Incrementa o contador de pulsos
}

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial a uma taxa de 9600 bps
  Esc30A.attach(pino_motor); // Inicializa o objeto Servo com o pino do motor
  Serial.println("Iniciando..."); // Imprime "Iniciando..." na porta serial
  delay(3000); // Aguarda 3 segundos
  attachInterrupt(digitalPinToInterrupt(pino_pulse), countPulses, RISING); // Configura a detecção de pulso de subida no pino digital 
}

void loop() {
  Speed = analogRead(pino_pot); // Lê o valor do potenciômetro
  Speed = map(Speed, 0, 1023, 0, 179); // Mapeia o valor do potenciômetro para um valor entre 0 e 179
  Esc30A.write(Speed); // Define a velocidade do motor com o valor do potenciômetro

  unsigned long currentTime = millis(); // Armazena o tempo atual em milissegundos
  if (currentTime - lastTime >= 1000) { // Verifica se passou 1000ms desde a última detecção de pulso
    float timeElapsed = (float)(currentTime - lastTime) / 1000.0; // Calcula o tempo decorrido em segundos
    if(pulseCount < 1){
       RPM = (60.0 / timeElapsed) * (float)pulseCount / (pulses_per_rotation+1);
       Serial.println("Primeiro pulso");
    }else{
      RPM = (60.0 / timeElapsed) * (float)pulseCount / pulses_per_rotation; // Calcula a velocidade do motor em rotações por minuto
    }
    
    Serial.print("RPM: "); // Imprime "RPM: " na porta serial
    Serial.println(RPM); // Imprime a velocidade do motor em rotações por minuto na porta serial
    Serial.print("Contagem de pulsos: "); // Imprime "Contagem de pulsos: " na porta serial
    Serial.println(pulseCount); // Imprime o número de pulsos contados na porta serial
    pulseCount = 0; // Reseta o contador de pulsos
    lastTime = currentTime; // Armazena o tempo da última detecção de pulso

    //tirar textos nao necessários do serial monitor
  }

  delay(100); // Aguarda 100ms antes de executar novamente o loop 
  //reduzir delay 
}
