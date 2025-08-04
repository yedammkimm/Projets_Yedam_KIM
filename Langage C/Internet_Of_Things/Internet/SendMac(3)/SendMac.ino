#include <ESP8266WiFi.h>

#define MAX_NETWORKS 50  // Nombre maximum de réseaux WiFi à enregistrer

// Structure pour enregistrer les informations des points d'accès
struct AccessPoint {
  String ssid;
  uint8_t bssid[6];  // Adresse MAC
  int32_t rssi;
  int32_t channel;
};

// Tableau pour stocker les informations des points d'accès
AccessPoint accessPoints[MAX_NETWORKS];
int apCount = 0;  // Compteur des points d'accès enregistrés

float calculateDistance(int rssi) {
  int A = -45;  // RSSI à 1 mètre (valeur typique, à ajuster selon les tests)
  float n = 2.0;  // Facteur d'atténuation (à ajuster selon l'environnement)
  return pow(10.0, (A - rssi) / (10.0 * n));
}

void printMAC(uint8_t* mac) {
  for (int i = 0; i < 6; i++) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("\nESP8266 WiFi scan example"));

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Disconnect from an AP if it was previously connected
  WiFi.disconnect();
  delay(100);
}

void loop() {
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t *bssid;
  int32_t channel;
  bool hidden;
  int scanResult;

  Serial.println(F("Starting WiFi scan..."));

  // Scanner les réseaux WiFi
  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

  if (scanResult == 0) {
    Serial.println(F("No networks found"));
  } else if (scanResult > 0) {
    Serial.printf(PSTR("%d networks found:\n"), scanResult);

    apCount = 0;  // Réinitialiser le compteur de points d'accès

    // Enregistrer les informations des points d'accès dans le tableau
    for (int8_t i = 0; i < scanResult && apCount < MAX_NETWORKS; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

      // Si vous souhaitez détecter uniquement des points d'accès publics, 
      // vous pouvez ajouter un filtre ici (par exemple, sans chiffrement).
      if (encryptionType == ENC_TYPE_NONE) {  // Aucun cryptage, potentiellement un point d'accès public
        // Stocker les informations du point d'accès
        accessPoints[apCount].ssid = ssid;
        memcpy(accessPoints[apCount].bssid, bssid, 6);
        accessPoints[apCount].rssi = rssi;
        accessPoints[apCount].channel = channel;
        apCount++;

        // Afficher les informations du point d'accès public détecté
        Serial.print("SSID: ");
        Serial.println(ssid);
        Serial.print("MAC Address: ");
        printMAC(bssid);
        Serial.print(" RSSI: ");
        Serial.print(rssi);
        Serial.print(" Channel: ");
        Serial.println(channel);
        Serial.println("-----------------------");
      }
    }

    // Exemple d'affichage de tous les points d'accès enregistrés
    Serial.println(F("\nPoints d'accès publics enregistrés :"));
    for (int i = 0; i < apCount; i++) {
      Serial.print("SSID: ");
      Serial.println(accessPoints[i].ssid);
      Serial.print("MAC Address: ");
      printMAC(accessPoints[i].bssid);
      Serial.print(" RSSI: ");
      Serial.print(accessPoints[i].rssi);
      Serial.print(" Channel: ");
      Serial.println(accessPoints[i].channel);
      Serial.println("-----------------------");
    }
  } else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }

  // Wait a bit before scanning again
  delay(10000);  // Scanner à nouveau après un certain délai
}
