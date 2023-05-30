
  #include <Servo.h> // Inclui a biblioteca Servo.h

  Servo Esc30A; // Cria um objeto Servo para controlar o motor
  unsigned int Ts = 500;  //tempo de amostragem em ms
  const int pino_pot = 0; // Define o pino analógico utilizado para o potenciômetro
  const int pino_motor = 9 ; // Define o pino PWM utilizado para o motor
  const int pino_pulse = 2; // Define o pino digital ligado em uma das fases do motor
  int Speed; // Variável para armazenar a velocidade do motor
  int pulseCount = 0; // Variável para armazenar o número de pulsos contados
  float RPM; // Variável para armazenar a velocidade em rotações por minuto
  unsigned long lastTime = 0; // Variável para armazenar a última vez que um pulso foi detectado
  float pulses_per_rotation = 7; // Número de pulsos por rotação do motor
  int PWM = 0;
  int aux = 0;
  int PWM1 = 1500;
  double tempo;
  

  // Função para contar os pulsos 
  void countPulses() {
    pulseCount++; // Incrementa o contador de pulsos
  }

  void setup() {
    Serial.begin(2000000); // Inicializa a comunicação serial a uma taxa de 9600 bps
    Serial.setTimeout(5); //limita o tempo de espera de caracteres de entrada da serial
    Esc30A.attach(pino_motor, 1000, 2000); // Inicializa o objeto Servo com o pino do motor
    //Serial.println("Iniciando..."); // Imprime "Iniciando..." na porta serial
   // delay(3000); // Aguarda 3 segundos
    attachInterrupt(digitalPinToInterrupt(pino_pulse), countPulses, RISING); // Configura a detecção de pulso de subida no pino digital
    //Esc30A.writeMicroseconds(1000); //initialize the signal to 1000 
    
  }

  int retornaPWM(){
      PWM = Serial.parseInt(); //lê caracteres ASCII da serial e converte em um número inteiro
      PWM = constrain(PWM, 1500, 2000);  //limita os valores da variável PWM entre 0 a 180
    return PWM;
  }

  void loop() { 
  tempo = millis();      //armazena o valor do tempo na variável "tempo" (em ms)
    unsigned long currentTime = millis(); // Armazena o tempo atual em milissegundos
    if (currentTime - lastTime >= 1000) { // Verifica se passou 1000ms desde a última detecção de pulso
      float timeElapsed = (float)(currentTime - lastTime) / 1000.0; // Calcula o tempo decorrido em segundos
      if(pulseCount < 1){
        RPM = (60.0 / timeElapsed) * (float)pulseCount / (pulses_per_rotation+1);
        //Serial.println("Primeiro pulso");
      }else{
        RPM = (60.0 / timeElapsed) * (float)pulseCount / pulses_per_rotation; // Calcula a velocidade do motor em rotações por minuto
      }
      
  
      Serial.print(PWM1);
      Serial.print(" ");
      Serial.println(RPM); // Imprime a velocidade do motor em rotações por minuto na porta serial
      
      pulseCount = 0; // Reseta o contador de pulsos
      lastTime = currentTime; // Armazena o tempo da última detecção de pulso 
    }

    if (Serial.available() > 0) {  //verifica se há dados na serial 
      PWM1= retornaPWM();
    }

    Esc30A.writeMicroseconds(PWM1);
  
 
   while ((tempo+Ts) > millis()){}   //espera o tempo necessário para completar o tempo de amostragem Ts
  }
