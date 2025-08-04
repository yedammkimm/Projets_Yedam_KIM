#include <SoftwareSerial.h>

// Définir les broches pour la communication série
SoftwareSerial LoRaSerial(D5, D6); // RX (D5) et TX (D6) de l'ESP8266

void setup() {
  // Initialiser la communication série avec le PC et le module LoRa-E5
  Serial.begin(115200);       // Communication série avec le moniteur série du PC
  LoRaSerial.begin(9600);     // Communication série avec le module LoRa E5 à 9600 bauds

  delay(1000);  // Pause pour que le module LoRa soit prêt

  // Envoyer une commande AT au module LoRa-E5
  LoRaSerial.println("AT");

  // Attendre un peu pour voir la réponse
  delay(1000);

  // Lire et afficher la réponse du module LoRa-E5
  if (LoRaSerial.available()) {
    while (LoRaSerial.available()) {
      String response = LoRaSerial.readString();
      Serial.println("Réponse du module LoRa: " + response);
    }
  } else {
    Serial.println("Pas de réponse du module LoRa.");
  }
}

void loop() {
  // Ajouter ici les autres commandes AT ou les données que vous souhaitez envoyer
}
