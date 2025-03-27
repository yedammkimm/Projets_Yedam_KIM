void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial.println("MKR WAN 1310 prêt !");
}

void loop() {
  if (Serial1.available()) {
    String receivedData = Serial1.readStringUntil('\n'); // Lire la ligne complète

    Serial.print("Reçu: ");
    Serial.println(receivedData);

    // Vérification que la chaîne contient bien les marqueurs T:, P:, H:
    if (receivedData.indexOf("T:") != -1 && receivedData.indexOf("P:") != -1 && receivedData.indexOf("H:") != -1 && receivedData.indexOf("Q:") != -1) {
      // Extraction des valeurs avec substring()
      int tStart = receivedData.indexOf("T:") + 2; // Trouver le début de la température
      int pStart = receivedData.indexOf("P:") + 2; // Trouver le début de la pression
      int hStart = receivedData.indexOf("H:") + 2; // Trouver le début de l'humidité
      int qStart = receivedData.indexOf("Q:") + 2; // Trouver le début de l'humidité

      int pEnd = receivedData.indexOf(",", tStart); // Trouver la fin de la température
      int hEnd = receivedData.indexOf(",", pStart); // Trouver la fin de la pression

      float temperature = receivedData.substring(tStart, pStart - 3).toFloat();
      float pressure = receivedData.substring(pStart, hStart - 3).toFloat();
      float humidity = receivedData.substring(hStart).toFloat();
      int queen_presence = receivedData.substring(qStart, qStart+1).toInt();

      // Affichage des valeurs extraites
      Serial.print("Température : ");
      Serial.print(temperature);
      Serial.println(" °C");

      Serial.print("Pression : ");
      Serial.print(pressure);
      Serial.println(" hPa");

      Serial.print("Humidité : ");
      Serial.print(humidity);
      Serial.println(" %");

      Serial.print("Reine : ");
      Serial.print(queen_presence);
      Serial.println("");
    } else {
      Serial.println("Format des données incorrect !");
    }
  }

  delay(2000);
}
