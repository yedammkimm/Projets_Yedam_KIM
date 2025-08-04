#include <ESP8266WiFi.h>
#include <math.h>

float calculateDistance(int rssi) {
  int A = -45;  // RSSI à 1 mètre (valeur typique, à ajuster selon les tests)
  float n = 2.0;  // Facteur d'atténuation (à ajuster selon l'environnement)
  return pow(10.0, (A - rssi) / (10.0 * n));
}

int compare(uint8_t* table1, uint8_t* table2) {
  for (int8_t i = 0; i < 6; i++) {
    if (table1[i] != table2[i]) {
      return 0;
    }
  }
  return 1;
}

// Coordonnées des points d'accès
float x1 = 0.0, y1_coord = 0.0;   // Coordonnées du point d'accès 319
float x2 = 10.0, y2_coord = 0.0;  // Coordonnées du point d'accès 320

void calculatePosition(float d1, float d2, float &x, float &y) {
  // Cette implémentation suppose que l'ESP se trouve sur la ligne reliant les deux points
  // Simplification en 1D (l'axe des x), car on a seulement 2 points d'accès
  x = (d1 * d1 - d2 * d2 + (x2 - x1) * (x2 - x1)) / (2 * (x2 - x1));
  y = 0;  // L'ESP se situe quelque part sur l'axe des x, donc y reste constant ici
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

  uint8_t esp319[] = {0xEE, 0xFA, 0xBC, 0xA8, 0x0F, 0xC8};
  uint8_t esp320[] = {0xEE, 0xFA, 0xBC, 0xA7, 0x6A, 0xF1};

  float d1 = 0.0, d2 = 0.0;  // Distances aux points d'accès esp319 et esp320
  bool foundEsp319 = false, foundEsp320 = false;

  Serial.println(F("Starting WiFi scan..."));

  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/true);

  if (scanResult == 0) {
    Serial.println(F("No networks found"));
  } else if (scanResult > 0) {
    Serial.printf(PSTR("%d networks found:\n"), scanResult);

    // Print unsorted scan results
    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

      // Calculer la distance à partir du RSSI
      float distance = calculateDistance(rssi);

      // Vérifier si le point d'accès est esp319 ou esp320
      if (compare(bssid, esp319)) {
        d1 = distance;
        foundEsp319 = true;
        Serial.printf(PSTR("ESP319 found: Distance = %.2f m\n"), d1);
      } else if (compare(bssid, esp320)) {
        d2 = distance;
        foundEsp320 = true;
        Serial.printf(PSTR("ESP320 found: Distance = %.2f m\n"), d2);
      }

      if (foundEsp319 && foundEsp320) {
        // Si les deux points d'accès sont trouvés, calculer la position estimée
        float x = 0, y = 0;
        calculatePosition(d1, d2, x, y);

        // Afficher la position estimée
        Serial.print("Position estimée de l'ESP: ");
        Serial.print("x = ");
        Serial.print(x);
        Serial.print(" m, y = ");
        Serial.println(y);
        break;  // On arrête après avoir trouvé les deux AP
      }
    }
  } else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }

  // Wait a bit before scanning again
  delay(5000);
}
