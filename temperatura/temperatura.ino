#include <math.h>
#include "AdafruitIO_WiFi.h"

#define WIFI_SSID "..."
#define WIFI_PASS "..."

//Authenticação Adafruit IO
#define IO_USERNAME  "..."
#define IO_KEY "..."


AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


#define pinNTC 34 //pino ligado na 34 sensor de temperatura
#define pinLed 14 //Pino do led

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




void setup() {
  pinMode(pinNTC, INPUT); //configurando o pino como entrada
  pinMode(pinLed, OUTPUT);


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

  //Configuração do callback, quando o feed receber(atualizar) um valor 
temperatura -> onMessage(handleTemperatura);
//registra a função de callback
//ela deverá ser chgamada sempre que o feed receber um novo dado

}

void loop() {

    //Manter a conexão com Adafruit IO ativa
  io.run();

 // publicacao(); // chamada da funcao publish

  delay(7000); //aguarda 7 segundos
}