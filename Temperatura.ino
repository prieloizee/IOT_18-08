#include <math.h>
#include "AdafruitIO_WiFi.h"

#define WIFI_SSID "Maria"
//#define WIFI_PASS "11102007"

//Authenticação Adafruit IO
#define IO_USERNAME "prieloize"


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


#define pinNTC 34 //pino ligado na 34

//Controle de envio de dados
float temp_atual = 0;
float temp_anterior = -1;

// variável / ponteiro para referenciar o feed temperatura
AdafruitIO_Feed * temperatura = io.feed("Temperatura");

const float Rfixo = 10000.0; //resistor do projeto
const float Beta = 3950.0;
const float R0 = 10000.0; //nominal do sensor
const float T0_kelvin = 298.15; // 25°c em kelvin
const float Vcc = 3.3;

float lerTemperaturaNTC(int pino, int numLeituras){
  long somaLeituras = 0;

  for(int i = 0; i < numLeituras; i++){
    somaLeituras += analogRead(pino);
    delay(5); //entre uma leitura e outra
  }

    float leituraMedia = somaLeituras / (float)numLeituras;

    float Vout = leituraMedia * (Vcc / 4095.0);

    float Rntc = Rfixo * ((Vcc / Vout) - 1.0);

    float tempK = 1.0 / ((1.0 / T0_kelvin) + (1.0 / Beta) * log(Rntc / R0));

    return tempK - 273.15;

}
void setup() {
  pinMode(pinNTC, INPUT); //configurando o pino como entrada

  Serial.begin(115200);

  while(!Serial);

  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  while(io.status() < AIO_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  delay(1000);

}

void loop() {

  //Manter a conexão com Adafruit IO ativa
  io.run();

  temp_atual = lerTemperaturaNTC(pinNTC, 10);

  //verificando alteração na temperatura
  if (temp_atual == temp_anterior){
    return;
  }


    // Serial.print("Temperatura Analogica: ");
    // Serial.println(analogRead(pinNTC));

    Serial.print(F("Temperatura enviada: "));
    Serial.print(temp_atual, 2);
    Serial.println(F("°C"));

    //envio / registrono feed "temperatura" no Adafruit IO
    temperatura -> save(temp_atual);

    temp_anterior = temp_atual;

    delay(3000);

}