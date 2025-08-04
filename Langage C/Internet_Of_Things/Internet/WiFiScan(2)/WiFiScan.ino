#include <ESP8266WiFi.h>

float calculateDistance(int rssi) {
  int A = -45;  // RSSI à 1 mètre (valeur typique, à ajuster selon les tests)
  float n = 2.0;  // Facteur d'atténuation (à ajuster selon l'environnement)
  return pow(10.0, (A - rssi) / (10.0 * n));
}

int compare (uint8_t* table1, uint8_t* table2){
  for (int8_t i = 0; i <6 ; i++){
    if (table1[i]!=table2[i]){
      return 0;
    }
  }
  return 1;
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

      // get extra info
      const bss_info *bssInfo = WiFi.getScanInfoByIndex(i);
      String phyMode;
      const char *wps = "";
      if (bssInfo) {
        phyMode.reserve(12);
        phyMode = F("802.11");
        String slash;
        if (bssInfo->phy_11b) {
          phyMode += 'b';
          slash = '/';
        }
        if (bssInfo->phy_11g) {
          phyMode += slash + 'g';
          slash = '/';
        }
        if (bssInfo->phy_11n) {
          phyMode += slash + 'n';
        }
        if (bssInfo->wps) {
          wps = PSTR("WPS");
        }
      }

      if(compare(bssid, esp319) || compare(bssid, esp320)){
      Serial.printf(PSTR("  %02d: [CH %02d] [%02X:%02X:%02X:%02X:%02X:%02X] %ddBm %c %c %-11s %3S %s\n distance:%fm \n"), i, channel, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], rssi, (encryptionType == ENC_TYPE_NONE) ? ' ' : '*', hidden ? 'H' : 'V', phyMode.c_str(), wps, ssid.c_str(),calculateDistance(rssi));
      yield();
      }
    }
  } else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }

  // Wait a bit before scanning again
  delay(5000);
}
