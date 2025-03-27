#include <MKRWAN.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTCZero.h>
#include <ArduinoLowPower.h>

LoRaModem modem;
RTCZero rtc;  

// DHT22 (Température et Humidité interne)
#define DHTPIN A6  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// DS18B20 #1 (Température interne 1)
#define ONE_WIRE_BUS_1 1  
OneWire oneWire1(ONE_WIRE_BUS_1);
DallasTemperature sensors1(&oneWire1);

// DS18B20 #2 (Température interne 2)
#define ONE_WIRE_BUS_2 2  
OneWire oneWire2(ONE_WIRE_BUS_2);
DallasTemperature sensors2(&oneWire2);

// Batterie (mesure de tension)
#define BATTERY_PIN A1  
#define VREF 3.3
#define RESOLUTION 4095  
#define V_MAX 3.3  
#define V_MIN 2.6  

// Pulse 
#define PULSE_PIN 6

// Alim (Contrôle de l'alimentation 3.3V)
#define ENABLE_3V3_PIN 7  

String appEui = "A8610A34304B860A";  
String appKey = "64726FB32755F50C800F64EC23079E2E";  

bool connected = false;
int err_count = 0;

void connectToTTN() {
   Serial.println("Tentative de connexion à TTN...");
   int attempt = 1;
   while (!connected) {  
      Serial.print("Tentative ");
      Serial.print(attempt);
      Serial.println("...");

      if (modem.joinOTAA(appEui, appKey)) {
         connected = true;
         modem.minPollInterval(10);  
         Serial.println("Connecté à TTN !");
         modem.dataRate(5);  
         delay(50);          
      } else {
         Serial.println("Échec de connexion, nouvelle tentative...");
         attempt++;
         delay(attempt * 2000);  
      }
   }
}

void setup() {
   Serial.begin(115200);
   analogReadResolution(12);
   delay(2000);

   Serial.println("MKR WAN 1310 - Envoi des données DHT22, DS18B20 & Batterie");

   // Activer 3.3V au démarrage
   pinMode(ENABLE_3V3_PIN, OUTPUT);
   digitalWrite(ENABLE_3V3_PIN, HIGH);  

   dht.begin();
   sensors1.begin();
   sensors2.begin();

   Serial.println("Initialisation du modem LoRa...");
   if (!modem.begin(EU868)) {
      Serial.println("Échec d'initialisation du modem !");
      while (true);
   }
   Serial.println("Modem LoRaWAN initialisé");

   connectToTTN();

   // Configuration du RTC pour réveil périodique
   rtc.begin();
   rtc.setTime(0, 0, 0);  
   rtc.setDate(1, 1, 2025);
   rtc.setAlarmTime(0, 1, 0);  // Réveil toutes les 1 minute
   rtc.enableAlarm(rtc.MATCH_MMSS);
   rtc.attachInterrupt(wakeUp);

   pinMode(PULSE_PIN, OUTPUT);
   digitalWrite(PULSE_PIN, LOW);

   wakeUp();
}

void wakeUp() {
    Serial.println("Réveil du RTC");

    //Activer immédiatement l'alimentation 3.3V pour éviter le décalage
    digitalWrite(ENABLE_3V3_PIN, HIGH);
    delay(500);  // Laisser 500ms pour stabiliser l'alimentation avant de mesurer les capteurs

    //Allumer le signal de réveil (Pulse sur D6)
    digitalWrite(PULSE_PIN, HIGH);
    delay(5000);  
}


void loop() {
   Serial.println("\nLecture des capteurs...");

   // Lecture DHT22 (Température et Humidité interne)
   float temperature = dht.readTemperature();
   float humidity = dht.readHumidity();

   if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Échec de lecture du DHT22");
      delay(5000);
      return;
   }

   Serial.print("Humidité interne : ");
   Serial.print(humidity);
   Serial.print(" % | Température interne (DHT22) : ");
   Serial.print(temperature);
   Serial.println(" °C");

   // Lecture DS18B20 #1
   sensors1.requestTemperatures();
   float t_0 = sensors1.getTempCByIndex(0);
   if (t_0 == DEVICE_DISCONNECTED_C) {
      Serial.println("Échec de lecture du DS18B20 #1 !");
      t_0 = 0;
   }
   Serial.print("Température interne #1 (DS18B20) : ");
   Serial.print(t_0);
   Serial.println(" °C");

   // Lecture DS18B20 #2
   sensors2.requestTemperatures();
   float t_1 = sensors2.getTempCByIndex(0);
   if (t_1 == DEVICE_DISCONNECTED_C) {
      Serial.println("Échec de lecture du DS18B20 #2 !");
      t_1 = 0;
   }
   Serial.print("Température interne #2 (DS18B20) : ");
   Serial.print(t_1);
   Serial.println(" °C");

   // Mesure de la tension de la batterie
   int sensorValue = analogRead(BATTERY_PIN);
   float voltage = (sensorValue * VREF) / RESOLUTION;

   // Calcul du pourcentage de batterie
   int batteryPercentage = (voltage >= V_MAX) ? 100 : 
                           (voltage <= V_MIN) ? 0 : 
                           ((voltage - V_MIN) / (V_MAX - V_MIN)) * 100;

   Serial.print("Tension mesurée : ");
   Serial.print(voltage);
   Serial.print(" V | Batterie : ");
   Serial.print(batteryPercentage);
   Serial.println(" %");

   // Encodage du payload
   uint8_t payload[10]; 
   int16_t hum = (int16_t)(humidity * 100);  
   int16_t temp = (int16_t)(temperature * 100);
   int16_t temp_0 = (int16_t)(t_0 * 100);
   int16_t temp_1 = (int16_t)(t_1 * 100);
   int16_t batt = (int16_t)(batteryPercentage);

   payload[0] = hum >> 8;
   payload[1] = hum & 0xFF;
   payload[2] = temp >> 8;
   payload[3] = temp & 0xFF;
   payload[4] = temp_0 >> 8;
   payload[5] = temp_0 & 0xFF;
   payload[6] = temp_1 >> 8;
   payload[7] = temp_1 & 0xFF;
   payload[8] = batt >> 8;
   payload[9] = batt & 0xFF;

   modem.beginPacket();
   modem.write(payload, sizeof(payload));
   int err = modem.endPacket();

   if (err <= 0) {
      Serial.println("Erreur d'envoi !");
   } else {
      Serial.println("Message envoyé !");
   }

   Serial.println("Mise en veille...");
   Serial.println(" ");
   digitalWrite(ENABLE_3V3_PIN, LOW);
   digitalWrite(PULSE_PIN, LOW);

   LowPower.sleep();  
}
