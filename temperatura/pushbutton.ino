void testeBotao (int pinBotao){
  Serial.print(F("Estado Botao:"));
  Serial.println(digitalRead(pinBotao));
  delay(500);
}