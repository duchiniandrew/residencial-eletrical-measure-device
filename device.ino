#include <WiFi.h>
#include <WiFiMulti.h>
#include "EmonLib.h"

EnergyMonitor SCT013;

void setup() {
  SCT013.current(pinSCT, 16);
  SCT013.voltage(36, 70, 1.7);

  Serial.begin(115200);

  WiFiMulti.addAp("Wifi Name", "Password");

  Serial.println();
  Serial.println();
  Serial.println("Waiting for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

void loop() {
  SCT013.calcVI(200, 200);
  float corrente = SCT013.Irms;
  float tensao = 127;
  float portencia = corrente * tensao;

  if(corrente < 0.80) {
    corrente = 0;
    tensao = 0;
    potencia = 0;
  } 
  String data = "Consume=" + String(corrente);
  Serial.println(data);
  const uint16_t port = 8000;
  const char * host = "endereco DNS do servidor";
  WiFiClient client;

  if(!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec")
    delay(5000);
    return;
  }

  client.println("GET /insertDadoConsumo?" + data + " HTTP/1.1");
  Serial.println("Sending GET");
  client.println("Host: DNS address again");
  client.println("Connection: close");
  client.println();
  Serial.println("closing connection");
  client.stop();
  Serial.println("waiting 5 sec...");
  delay(5000);
  Serial.println(" ");
  Serial.println("Corrente = ");
  Serial.println(corrente);
  Serial.println(" A");
  Serial.println("Tensao = ");
  Serial.println(tensao);
  Serial.println(" V");
  Serial.println("Potencia = ");
  Serial.println(Potencia);
  Serial.println(" W");
  delay(2000);

  if(client.connected()) {
    client.stop();
  }
}
