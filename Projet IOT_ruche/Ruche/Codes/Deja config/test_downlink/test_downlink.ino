#include <MKRWAN.h>

LoRaModem modem;

String appEui = "A8610A3435305F10";  // Remplace avec ton AppEUI
String appKey = "357145389E12496DF97BF72FE937E69D";  // Remplace avec ton AppKey

int sendIntervalMinutes = 1;  // Intervalle d'envoi initial en minutes (modifiable via downlink)
int sendInterval=5*1000; // Conversion en millisecondes


void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);  // LED OFF au démarrage

    Serial.println("Initialisation du modem LoRa...");
    if (!modem.begin(EU868)) {  
        Serial.println("Échec d'initialisation !");
        while (1);
    }

    Serial.println("Modem initialisé !");
    Serial.println("Tentative de connexion à TTN...");

    while (!modem.joinOTAA(appEui, appKey)) {
        Serial.println("Échec, nouvelle tentative...");
        delay(10000);
    }
    Serial.println("Connecté à TTN !");
}

void loop() {
    Serial.println("Envoi d'un uplink...");
    modem.beginPacket();
    modem.write(0xAA);  // Payload test
    modem.endPacket(true);  // "true" pour recevoir un downlink

    delay(3000);  // Attendre la réponse downlink

    int packetSize = modem.parsePacket();
    if (packetSize) {
        Serial.print("⬇ Downlink reçu : ");
        int payload[2];
        for (int i = 0; i < 2; i++) {
            payload[i] = modem.read();
            Serial.print(payload[i], HEX);
            Serial.print(" ");
        }
        Serial.println();

        // Interprétation du payload
        if (payload[0] == 0x00) {
            // Payload[1] : Intervalle en secondes
            // Conversion en millisecondes
            sendInterval = payload[1] * 1000; 
            Serial.print("Nouvelle fréquence d'envoi : ");
            Serial.print(sendIntervalMinutes);
            Serial.println(" secondes");
        }
        else if(payload[0] == 0x01) {
            // Payload[1] : Intervalle en minutes
            sendIntervalMinutes = payload[1]; 
            // Conversion en millisecondes
            sendInterval = sendIntervalMinutes * 60 * 1000; 
            Serial.print("Nouvelle fréquence d'envoi : ");
            Serial.print(sendIntervalMinutes);
            Serial.println(" minutes");
          
        } 
        else if(payload[0] == 0x02) {
            // Payload[1] : Intervalle en minutes
            sendIntervalMinutes = payload[1]; 
            // Conversion en millisecondes
            sendInterval = sendIntervalMinutes * 60 * 60 * 1000; 
            Serial.print("Nouvelle fréquence d'envoi : ");
            Serial.print(sendIntervalMinutes);
            Serial.println(" heures");
        }else {
        Serial.println("Payload inconnu !");
        }
        
    } else {
        Serial.println("Pas de downlink reçu.");
    }

    Serial.print("Attente ");
    Serial.print(sendInterval/1000);
    Serial.println(" s avant le prochain uplink...");
    delay(sendInterval);  // Attendre l'intervalle en millisecondes
}
