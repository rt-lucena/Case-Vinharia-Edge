#include <DHT.h>
#include <LiquidCrystal.h>
 
// Definindo as variáveis e pinos
#define DHTPIN 2 // Pino digital onde o DHT22 está conectado
#define DHTTYPE DHT22 // Define o modelo do sensor (DHT22)
DHT dht(DHTPIN, DHTTYPE); // Inicializa o sensor DHT22
#define LDRPIN A0 // Pino analógico onde o LDR está conectado
int LED_VERDE = 5; // Pino onde o LED verde está conectado
int LED_AMARELO = 4; // Pino onde o LED amarelo está conectado
int LED_VERMELHO = 3; // Pino onde o LED vermelho está conectado
int BUZZER = 6; // Pino onde o buzzer está conectado
 
// Inicializa a biblioteca com os números dos pinos da interface
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
 
// Variáveis globais
int somaIntensidadeLuz = 0;
float somaTemperatura = 0.0;
float somaUmidade = 0.0;
 
//Aranha Traseira Superior 1
 
 
//Icone do Luminosidade
byte solB[8] = { //Luminosidade baixa
  B00000,
  B00000,
  B00000,
  B01000,
  B11100,
  B01110,
  B10100,
  B00000
};
 
byte solM[8] = { //Luminosidade Média
  B00000,
  B00000,
  B10000,
  B01000,
  B11100,
  B01110,
  B10101,
  B00000
};
 
byte solA[8] = { //Luminosidade Alta
  B00000,
  B00000,
  B10101,
  B01110,
  B11111,
  B01110,
  B10101,
  B00000
};
 
//Icone da umidade baixa
byte aguaB[8] = { //umidade baixa
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};
 
//Icone da umidade média
byte aguaM[8] = { //umidade média
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10111,
  B11111,
  B01110
};
 
//Icone da umidade Alta
byte aguaA[8] = { //umidade Alta
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};
 
//Icone da temp.
  byte temperaturaB[8] = { //Temperatura baixa
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B11111,
    B01110
  };
 
byte temperaturaM[8] = { //Temperatura Média
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};
byte temperaturaA[8] = { //Temperatura Alta
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};
 
void setup() {
  Serial.begin(9600); // Inicia a comunicação serial a uma velocidade de 9600 bits por segundo
  pinMode(LDRPIN, INPUT); // Define o pino do LDR como entrada
  pinMode(LED_VERDE, OUTPUT); // Define o pino do LED verde como saída
  pinMode(LED_AMARELO, OUTPUT); // Define o pino do LED amarelo como saída
  pinMode(LED_VERMELHO, OUTPUT); // Define o pino do LED vermelho como saída
  pinMode(BUZZER, OUTPUT); // Define o pino do buzzer como saída
 
  // Iniciando o display LCD 16x2
  lcd.begin(16, 2);
 
 // Criando os icones
  lcd.createChar(10, solA);
  lcd.createChar(5, solM);
  lcd.createChar(6, aguaB);
  lcd.createChar(7, aguaM);
  lcd.createChar(8, temperaturaB);
  lcd.createChar(9, temperaturaM);
 
 
  // Inicializa o sensor DHT22
  dht.begin();
  logo();
  delay(2000);
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    lcd.scrollDisplayRight(); // scroll one position left
    delay(100);
 
  }
  delay(2000);}
 
 
 
void loop() {
  // Lê a intensidade da luz do sensor LDR
  int IntensidadeLuz = analogRead(LDRPIN);
 
  // Lê a umidade e a temperatura do sensor DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  // Verifica se as leituras falharam e, em caso afirmativo, retorna para tentar novamente
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }
 
  // Converte a leitura do LDR para uma escala de 0 a 100
  IntensidadeLuz = map(IntensidadeLuz, 0, 1023, 0, 100);
 
  // Limpa o display
  lcd.clear();
  // Imprime a condição de luz do ambiente
  luminosidade();
  // Escreve o ícone do sol
  lcd.print(": ");
  if(IntensidadeLuz < 45) {
    luminosidade();
    lcd.print("OK");
  } else if(IntensidadeLuz >= 45 && IntensidadeLuz < 70) {
    lcd.setCursor(0,0);
    lcd.write(5);
    lcd.print("+ -");
  } else if(IntensidadeLuz >= 70) {
    lcd.setCursor(0,0);
    luzAlta();
    lcd.print("ALTA");
  }
 
  // Imprime a condição de temperatura
  temperatura();
   //Escreve o ícone da temperatura
  lcd.print(": ");
  if(t < 20) {
    lcd.setCursor(0,1);
    lcd.write(8);
    lcd.print("BAIXA");
  } else if(t >= 20 && t < 30) {
    temperaturaMe();
    lcd.print("OK");
  } else if(t >= 30) {
    temperatura;
    lcd.print("ALTA");
  }
 
  // Imprime a condição de umidade
  umidade();
  // Escreve o ícone da umidade
  lcd.print(":");
  if(h < 40) {
    lcd.setCursor(9,1);
    lcd.write(6);
    lcd.print("BAIXA");
  } else if(h >= 40 && h < 65) {
    lcd.setCursor(9,1);
    lcd.write(7);
    lcd.print("OK");
  } else if(h >= 60) {
    umidade();
    lcd.print("ALTA");
  }
 
  // Adiciona as leituras às somas
  somaIntensidadeLuz += IntensidadeLuz;
  somaTemperatura += t;
  somaUmidade += h;
 
  // Calcula as médias
  float mediaIntensidadeLuz = somaIntensidadeLuz / 100.0;
  float mediaTemperatura = somaTemperatura / 100.0; // Modificado aqui
  float mediaUmidade = somaUmidade / 100.0;
 
  // Imprime as médias no Serial
  Serial.print("Media Luz: ");
  Serial.print(mediaIntensidadeLuz * 100);
  Serial.println("%");
  Serial.print("Media Temp: ");
  Serial.print(mediaTemperatura * 100); // Imprime a média da temperatura em graus Celsius
  Serial.println("°C");
  Serial.print("Media Umi: ");
  Serial.print(mediaUmidade * 100);
  Serial.println("%");
 
 
  // Reinicia as somas
  somaIntensidadeLuz = 0;
  somaTemperatura = 0.0;
  somaUmidade = 0.0;
 
  delay(5000); // Aguarda 5 segundos antes da próxima leitura
   
  // Verifica a intensidade da luz, temperatura e umidade e aciona o LED e buzzer correspondente
  if((IntensidadeLuz < 45) && (t >= 20 && t < 30) && (h >= 40 && h < 60)) {
    digitalWrite(LED_VERDE, HIGH); // Acende o LED verde
    digitalWrite(LED_AMARELO, LOW); // Apaga o LED amarelo
    digitalWrite(LED_VERMELHO, LOW); // Apaga o LED vermelho
  } else if((IntensidadeLuz >= 45 && IntensidadeLuz < 70) || t < 20 || h < 40) {
    digitalWrite(LED_VERDE, LOW); // Apaga o LED verde
    digitalWrite(LED_AMARELO, HIGH); // Acende o LED amarelo
    digitalWrite(LED_VERMELHO, LOW); // Apaga o LED vermelho
    tone(BUZZER, 500);
    delay(100); // Espera 100 milissegundos
    noTone(BUZZER); // Desliga o buzzer
  delay(100); // Espera 100 milissegundos (intervalo breve)
  // Liga o buzzer novamente com uma frequência de 500 Hz por um curto período
    tone(BUZZER, 500);
    delay(100); // Espera 100 milissegundos
    noTone(BUZZER); // Desliga o buzzer
  } else if(IntensidadeLuz >= 70 || t >= 30 || h >= 60) {
    digitalWrite(LED_VERDE, LOW); // Apaga o LED verde
    digitalWrite(LED_AMARELO, LOW); // Apaga o LED amarelo
    digitalWrite(LED_VERMELHO, HIGH); // Acende o LED vermelho
 
    tone(BUZZER, 500); // Liga o buzzer com uma frequência de 500 Hz
    delay(500); // Espera 500 milisegundos
    noTone(BUZZER); // Desliga o buzzer
  }
}

// Função da logo do grupo
void logo(){
  byte aranhaTraseira1[8] = {
  B00000,
  B01001,
  B10010,
  B10010,
  B01010,
  B00111,
  B01111,
  B01111
};
 
//Aranha Traseira Inferior 1
byte aranhaTraseira2[8] = {
  B01111,
  B01111,
  B00111,
  B01010,
  B10010,
  B10010,
  B01001,
  B00000
};
 
 
//Aranha Frotal Superior 1
byte aranhaFrontal1[8] = {
  B00000,
  B00100,
  B01000,
  B10000,
  B10000,
  B11000,
  B11110,
  B11000
};
 
 
//Aranha Frontal Inferior 1
byte aranhaFrontal2[8] = {
  B11000,
  B11110,
  B11000,
  B10000,
  B10000,
  B01000,
  B00100,
  B00000
};
 
  lcd.createChar(1, aranhaTraseira1);
  lcd.createChar(2, aranhaTraseira2);
  lcd.createChar(3, aranhaFrontal1);
  lcd.createChar(4, aranhaFrontal2);
 
  lcd.setCursor(1, 0);
  lcd.write(1);
  lcd.setCursor(1, 1);
  lcd.write(2);
 
  lcd.setCursor(2, 0);
  lcd.write(3);
 
  lcd.setCursor(2, 1);
  lcd.write(4);
 
  lcd.setCursor(4, 0);
  lcd.print("RaySpiders");
 
}
 

// Icones
void temperatura() {
  lcd.createChar(1, temperaturaA);
  lcd.setCursor(0, 1);
  lcd.write(1);
}

void luminosidade() {
  lcd.createChar(2, solB);
  lcd.setCursor(0, 0);
  lcd.write(2);
}

void umidade() {
  lcd.createChar(3, aguaA);
  lcd.setCursor(9, 1);
  lcd.write(3);
}

void luzAlta() {
  lcd.createChar(4, solA);
  lcd.setCursor(0, 0);
  lcd.write(4);
}

void temperaturaMe() {
  lcd.createChar(5, temperaturaM);
  lcd.setCursor(0, 1);
  lcd.write(5);
}
