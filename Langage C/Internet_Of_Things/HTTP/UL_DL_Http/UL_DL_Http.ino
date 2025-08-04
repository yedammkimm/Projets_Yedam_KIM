
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "iPhone de Aymane";              // Remplacez par votre SSID
const char* password = "12345678";  // Remplacez par votre mot de passe WiFi


// Node-RED Server IP (IP du serveur Node-RED)
const char* serverName = "http://172.20.10.6:1880/dht-data";

// Définir le serveur Web pour contrôler la LED
ESP8266WebServer server(80);  // Créer un serveur web sur le port 80

// DHT sensor settings
#define DHTPIN 14          // GPIO où le DHT14 est connecté
#define DHTTYPE DHT11     // Type de capteur DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED pin
#define LED 2  // GPIO pour la LED intégrée

WiFiClient wifiClient;  // Déclaration d'un client WiFi


void setup() {
  // Initialisation du port série
  Serial.begin(115200);
  
  // Initialisation du capteur DHT
  dht.begin();
  
  // Configuration de la LED
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW); // Par défaut, la LED est éteinte

  // Connexion au WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connexion au WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("not connected");
  }
  Serial.println("");
  Serial.println("WiFi connecté !");
  
  // Serveur HTTP pour contrôler la LED
  server.on("/control-led", HTTP_POST, []() {
    String command = server.arg("plain");  // Récupérer la commande "ON" ou "OFF"
    Serial.println("Commande reçue : " + command);

    // Contrôler la LED selon la commande
    if (command == "ON") {
      digitalWrite(LED, HIGH);  // Allumer la LED
      Serial.println("LED éteinte");
    } else if (command == "OFF") {
      digitalWrite(LED, LOW);   // Éteindre la LED
      Serial.println("LED allumée");
    }
    // Réponse au client (Node-RED)
    server.send(200, "text/plain", "Commande exécutée");
  });

  server.begin();  // Démarrer le serveur
  Serial.println("Serveur HTTP démarré");
 
}

void loop() {
  delay(2000);

  // Lire les données du capteur DHT
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Vérifier si les valeurs sont valides
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur de lecture du capteur DHT !");
  } else {
    // Afficher les données du capteur sur le moniteur série
    Serial.print("temperature : ");
    Serial.print(temperature);
    Serial.print(" °C\thumidity : ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  
  // Envoyer les données via HTTP POST
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverName);  // URL de destination pour HTTP POST

    // Créer les données JSON à envoyer
    String postData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
    http.addHeader("Content-Type", "application/json");

    // Envoyer la requête HTTP POST
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString(); // Réponse du serveur
      Serial.println(httpResponseCode);   // Code de retour
      Serial.println(response);           // Réponse du serveur
    } else {
      Serial.print("Erreur lors de l'envoi du POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Libérer les ressources
  }
  // Attendre un peu plus pour stabiliser les lectures
  delay(1000);  // Augmentez à 1 seconde pour donner plus de temps
  
  // Traiter les requêtes du serveur pour la LED
  server.handleClient();
}
