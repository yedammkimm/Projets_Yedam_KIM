#include <MKRWAN.h>
#include <DHT.h>

LoRaModem modem;

#define DHTPIN 15          // PA22 (D0) sur le MKR WAN 1310
#define DHTTYPE DHT11      // Capteur utilisé
DHT dht(DHTPIN, DHTTYPE);

String appEui = "A8610A34304B860A"; // Remplacez par votre AppEUI
String appKey = "64726FB32755F50C800F64EC23079E2E"; // Remplacez par votre AppKey

bool connected = false;
int err_count = 0;

void connectToTTN() {
   Serial.println("Tentative de connexion à TTN...");
   int attempt = 1;

   while (!connected) {  
      Serial.print("Tentative ");
      Serial.print(attempt);
      Serial.println(" de connexion...");

      if (modem.joinOTAA(appEui, appKey)) {
         connected = true;
         modem.minPollInterval(10);  // Intervalle minimum de 10 secondes
         Serial.println("Connecté à TTN !");
         modem.dataRate(5);  // SF7 pour des messages rapides
         delay(50);          
      } else {
         Serial.println("Échec de connexion, nouvelle tentative...");
         attempt++;
         delay(attempt * 2000);  // Attente progressive (2s, 4s, 6s...)
      }
   }
}

void setup() {
   Serial.begin(115200);
   while (!Serial);
   Serial.println("🔹 MKR WAN 1310 - Envoi en boucle des données DHT11");

   dht.begin();  // Initialisation du capteur DHT11

   if (!modem.begin(EU868)) {
      Serial.println("Échec d'initialisation du modem !");
      while (true);
   }

   connectToTTN(); // Se connecter à TTN au démarrage
}

void loop() {
   while (true) {  // Boucle infinie pour envoyer les données en continu
      if (!connected) {
         Serial.println("🔄 Connexion perdue, tentative de reconnexion...");
         connectToTTN();
      }

      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      if (isnan(temperature) || isnan(humidity)) {
         Serial.println("Échec de lecture du capteur !");
         delay(5000);
         continue; // Réessayer après 5 secondes
      }

      Serial.print("💧 Humidité : ");
      Serial.print(humidity);
      Serial.print(" % | 🌡️ Température : ");
      Serial.print(temperature);
      Serial.println(" °C");

      uint8_t payload[4]; 

      int16_t hum = (int16_t)(humidity * 100);  // Convertir en entier (ex: 56.78% -> 5678)
      int16_t temp = (int16_t)(temperature * 100);  // Convertir en entier (ex: 23.45°C -> 2345)

      payload[0] = hum >> 8;
      payload[1] = hum & 0xFF;
      payload[2] = temp >> 8;
      payload[3] = temp & 0xFF;

      modem.beginPacket();
      modem.write(payload, sizeof(payload));
      int err = modem.endPacket();

      if (err <= 0) {
         Serial.print("Erreur d'envoi : ");
         Serial.println(err);
         err_count++;
         if (err_count > 5) {
            Serial.println("⛔ Trop d'échecs, tentative de reconnexion...");
            connected = false;
         }
      } else {
         Serial.println("Message envoyé avec succès !");
         err_count = 0;
      }

      delay(10000); // Envoi toutes les 10 secondes (modifiable)
   }
}
