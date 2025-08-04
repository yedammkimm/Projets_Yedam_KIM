#include <MKRWAN.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoLowPower.h>
#include <Adafruit_TSL2591.h>
#include <HX711.h>

// LoRa & RTC
LoRaModem modem;
RTCZero rtc;

// Light Sensor
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// HX711 Weight Sensor
#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5
HX711 scale;

// DHT22 Sensors
#define DHTPIN_INT A6
#define DHTPIN_EXT A5
#define DHTTYPE DHT22
DHT dht_int(DHTPIN_INT, DHTTYPE);
DHT dht_ext(DHTPIN_EXT, DHTTYPE);

// DS18B20 Sensors
#define ONE_WIRE_BUS_1 0
OneWire oneWire1(ONE_WIRE_BUS_1);
DallasTemperature sensors1(&oneWire1);

#define ONE_WIRE_BUS_2 1
OneWire oneWire2(ONE_WIRE_BUS_2);
DallasTemperature sensors2(&oneWire2);

// Battery
#define BATTERY_PIN A1
#define VREF 3.3
#define RESOLUTION 4095
#define V_MAX 3.3
#define V_MIN 2.6

// TPL
#define TPL_DONE_PIN 6
#define TPL_DELAY_PIN 3

#define SUNRISE_LUX_THRESHOLD 100
#define TIME_INCREMENT_MINUTES 1  // adapt to your actual interval
#define LED_PIN LED_BUILTIN

static int sleepDurationMs = 60000 ;
static uint16_t minutes_ensoleillement = 0;
static float hours_ensoleillement = 0;
static float max_hours_ensoleillement = 0;
static uint16_t software_minutes_counter = 0;
static bool ensoleillement = false;


String appEui = "A8610A34304B860A";
String appKey = "64726FB32755F50C800F64EC23079E2E";

float previous_weight = 0;

void connectToTTN() {
  Serial.println("Connecting to TTN...");
  while (!modem.joinOTAA(appEui, appKey)) {
    delay(1000);
  }
  modem.minPollInterval(10);
  modem.dataRate(5);
  delay(50);
}


void setup() {
    Serial.begin(115200);
    Serial1.begin(9600);
    delay(2000);
    analogReadResolution(12);
  if (!modem.begin(EU868)) {
    while (true);
  }
  pinMode(LED_PIN, OUTPUT);
  connectToTTN();
  dht_int.begin();
  dht_ext.begin();

  if (!tsl.begin()) {
    delay(100);
    Wire.end();
    delay(100);
    Wire.begin();
    tsl.begin();
  }
  
  tsl.setGain(TSL2591_GAIN_LOW);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.power_up();

  pinMode(ONE_WIRE_BUS_1, INPUT);
  pinMode(ONE_WIRE_BUS_2, INPUT);
  delay(100);
  sensors1.begin();
  sensors2.begin();
  Serial1.end();
  Serial1.begin(115200);
}

float readWeightSensor() {
  if (scale.is_ready()) {
    float avg = scale.get_units(20);
    float w = (avg - 373036) / 28728;
    return (w < 0) ? 0 : w;
  }
  return 0;
}

float readLightSensor() {
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir = lum >> 16;
  uint16_t full = lum & 0xFFFF;
  if (full == 0 && ir == 0) {
    if (!tsl.begin()) {
      return -1.0;
    }
    tsl.setGain(TSL2591_GAIN_LOW);
    tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
    return 0;
  }
  float lux = tsl.calculateLux(full, ir);
  return (lux > 0) ? lux : 0;
}

void sendDoneSignal() {
  digitalWrite(TPL_DELAY_PIN, LOW);
  delay(100);
  digitalWrite(TPL_DONE_PIN, HIGH);
  delay(500);
}

void loop() {
    digitalWrite(TPL_DONE_PIN, LOW);
    delay(1000);
    digitalWrite(TPL_DELAY_PIN, HIGH);
    delay(1000);

      
   // Internal DHT22
   float temp_int = dht_int.readTemperature();
   float hum_int = dht_int.readHumidity();
   if (isnan(temp_int) || isnan(hum_int)) {
      temp_int = hum_int = 0;
   }

   // External DHT22
   float temp_ext = dht_ext.readTemperature();
   float hum_ext = dht_ext.readHumidity();
   if (isnan(temp_ext) || isnan(hum_ext)) {
      temp_ext = hum_ext = 0;
   }
   
   // Read Light Sensor
   float lux = readLightSensor();

   // Read Weight Sensor
   float weight_kg = readWeightSensor();

   float weight_diff = weight_kg - previous_weight;

   // Measure Battery Voltage
   int sensorValue = analogRead(BATTERY_PIN);
   float voltage = (sensorValue * VREF) / RESOLUTION;

   // Compute Battery Percentage
   int batteryPercentage = (voltage >= V_MAX) ? 100 : 
                           (voltage <= V_MIN) ? 0 : 
                           ((voltage - V_MIN) / (V_MAX - V_MIN)) * 100;
  
   float temp_nano = -1.0;
   float pressure = -1.0; 
   float hum_nano = -1.0;
   int queenDetected = 0; 
                  
  if (Serial1.available()) {
    String receivedData = Serial1.readString();
    receivedData.trim(); // Supprimer les espaces et caractères parasites
    
    // Vérification que la chaîne contient bien les marqueurs T:, P:, H:
    if (receivedData.indexOf("T:") != -1 && receivedData.indexOf("P:") != -1 && receivedData.indexOf("H:") != -1 && receivedData.indexOf("Q:") != -1) {
      // Extraction des valeurs avec substring()
      int tStart = receivedData.indexOf("T:") + 2; // Trouver le début de la température
      int pStart = receivedData.indexOf("P:") + 2; // Trouver le début de la pression
      int hStart = receivedData.indexOf("H:") + 2; // Trouver le début de l'humidité
      int qStart = receivedData.indexOf("Q:") + 2; // Trouver le début de l'humidité

      int pEnd = receivedData.indexOf(",", tStart); // Trouver la fin de la température
      int hEnd = receivedData.indexOf(",", pStart); // Trouver la fin de la pression
      int qEnd = receivedData.indexOf(",", hStart);

      temp_nano = receivedData.substring(tStart, pEnd).toFloat();
      pressure = receivedData.substring(pStart, hEnd).toFloat();
      hum_nano = receivedData.substring(hStart, qEnd).toFloat();
      queenDetected = receivedData.substring(qStart).toInt();   
    }
  }
  // Read DS18B20 #1
   sensors1.requestTemperatures();
   sensors2.requestTemperatures();
   float t_1 = sensors1.getTempCByIndex(0);
   float t_2 = sensors2.getTempCByIndex(0);
   if (t_1 == DEVICE_DISCONNECTED_C) {
      t_1 = 0;
   }
   if (t_2 == DEVICE_DISCONNECTED_C) {
      t_2 = 0;
   }
  
   // Encode Payload
   uint8_t payload[37]; 
   int16_t hum_i = (int16_t)(hum_int * 100);         // Internal Humidity (DHT22)
   int16_t temp_i = (int16_t)((temp_int == 0 ? temp_int : temp_int- 0.5) * 100);   // Internal Temperature (DHT22)
   int16_t hum_e = (int16_t)(hum_ext * 100);         // External Humidity (DHT22)
   int16_t temp_e = (int16_t)(temp_ext * 100);   // External Temperature (DHT22)
   int16_t temp_1 = (int16_t)((t_1 == 0 ? t_1 : t_1-1) * 100);  // Internal Temperature #1 (DS18B20)
   int16_t temp_2 = (int16_t)((t_2 == 0 ? t_2 : t_2) * 100);           // Internal Temperature #2 (DS18B20)
   int16_t temp_3 = (int16_t)((temp_nano == -1 ? 0: temp_nano- 1.5)* 100);     //Internal Temperature #3 (Nano)
   int16_t hum_3 = (int16_t)((hum_nano == -1 ? 0: hum_nano)* 100);       // Internal Humidity (Nano)
   int16_t batt = (int16_t)(batteryPercentage);     // Battery Level (%)
   int16_t lux_value = (int16_t)(lux == -1.0 ? 0 : lux*10);         // Light Intensity (TSL2591)
   int16_t weight_value = weight_kg * 100;          // Weight (HX711 Load Cell)
   int16_t pressure_value = (int16_t)((pressure == -1 ? 0: pressure) * 10); // Pression (hPa)
   uint8_t queen_present = queenDetected ? 1 : 0;   // Présence de la reine (0 ou 1)
   uint8_t colony_size = (uint8_t)(temp_i > 35) + (uint8_t)(temp_1 > 35) + (uint8_t)(temp_2 > 35); // Taille de la colonie (0 à 3) 
    float weight_diff_value = ((int)(weight_diff * 100 + 0.5)) / 100.0;

  software_minutes_counter += TIME_INCREMENT_MINUTES;
  
  union {
    float floatValue;
    uint8_t bytes[4];
  } floatToBytes;

    union {
    float floatValue1;
    uint8_t bytes[4];
  } floatToBytes2;
  
  // Reset daily tracking at midnight (1440 minutes)
  if (software_minutes_counter >= 1440) {
    max_hours_ensoleillement = hours_ensoleillement;
    minutes_ensoleillement = 0;
    hours_ensoleillement = 0;
    software_minutes_counter = 0;
  }
  
  // Track sunlight duration
  if (lux > SUNRISE_LUX_THRESHOLD) {
    ensoleillement = true;
    minutes_ensoleillement += TIME_INCREMENT_MINUTES;
    hours_ensoleillement = (minutes_ensoleillement / 60.0);
  } else {
    ensoleillement = false;
    // keep value until next sunrise
  }
  
  uint8_t ensoleil = ensoleillement ? 1 : 0;   // Présence de la reine (0 ou 1)
  floatToBytes.floatValue = weight_diff_value;
  floatToBytes2.floatValue1 = (max_hours_ensoleillement == 0.0) ? ((int)(hours_ensoleillement * 100 + 0.5)) / 100.0 : max_hours_ensoleillement;

   // Encoding payload in the correct order
  payload[0] = hum_i >> 8;
  payload[1] = hum_i & 0xFF;         // Internal Humidity (DHT22)
      
  payload[2] = temp_i >> 8;
  payload[3] = temp_i & 0xFF;        // Internal Temperature (DHT22)

  payload[4] = hum_e >> 8;
  payload[5] = hum_e & 0xFF;         // External Humidity (DHT22)
      
  payload[6] = temp_e >> 8;
  payload[7] = temp_e & 0xFF;        // External Temperature (DHT22)
      
  payload[8] = temp_1 >> 8;
  payload[9] = temp_1 & 0xFF;      // Internal Temperature #1 (DS18B20)
      
  payload[10] = temp_2 >> 8;
  payload[11] = temp_2 & 0xFF;      // Internal Temperature #2 (DS18B20)
      
  payload[12] = temp_3 >> 8;
  payload[13] = temp_3 & 0xFF;        //Internal Temperature #3 (Nano)
      
  payload[14] = hum_3 >> 8;
  payload[15] = hum_3 & 0xFF;  //Internal Humidity #3 (Nano)
      
  payload[16] = batt >> 8;
  payload[17] = batt & 0xFF; // Battery Level (%)

  payload[18] = lux_value >> 8;
  payload[19] = lux_value & 0xFF; // Light Intensity (TSL2591)

  payload[20] = weight_value >> 8;
  payload[21] = weight_value & 0xFF; // Weight (HX711 Load Cell)
  
  payload[22] = pressure_value >> 8;
  payload[23] = pressure_value & 0xFF; // Pression (hPa)
  
  payload[24] = queen_present; // Présence de la reine (0 ou 1)

  payload[25] = colony_size >> 8;
  payload[26] = colony_size & 0xFF; // Taille de colonie (sur 3, fractionnée)

  payload[27] = floatToBytes.bytes[0];
  payload[28] = floatToBytes.bytes[1];
  payload[29] = floatToBytes.bytes[2];
  payload[30] = floatToBytes.bytes[3];  
  
  payload[31] = ensoleillement ? 1 : 0;

  payload[32] = floatToBytes2.bytes[0];
  payload[33] = floatToBytes2.bytes[1];
  payload[34] = floatToBytes2.bytes[2];
  payload[35] = floatToBytes2.bytes[3];
 
  previous_weight = weight_kg;
  
  modem.beginPacket();
  modem.write(payload, sizeof(payload));
  modem.endPacket();

  int packetSize = modem.parsePacket();
  if (packetSize) {
    Serial.print("⬇ Downlink reçu : ");
    int r_payload[2];
    for (int i = 0; i < 2; i++) {
      r_payload[i] = modem.read();
      Serial.print(r_payload[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Interprétation du downlink
    if (r_payload[0] == 0x00) {  // Intervalle en secondes
      sleepDurationMs = r_payload[1] * 1000UL;
      Serial.print("Nouvel intervalle en secondes : ");
      Serial.println(r_payload[1]);
    } else if (r_payload[0] == 0x01) {  // Intervalle en minutes
      sleepDurationMs = r_payload[1] * 60UL * 1000UL;
      Serial.print("Nouvel intervalle en minutes : ");
      Serial.println(r_payload[1]);
    } else if (r_payload[0] == 0x02) {  // Intervalle en heures
      sleepDurationMs = r_payload[1] * 3600UL * 1000UL;
      Serial.print("Nouvel intervalle en heures : ");
      Serial.println(r_payload[1]);
    } else {
      Serial.println("⚠️ Type de commande inconnu.");
    }
  } else {
    Serial.println("Aucun downlink reçu. Utilisation de l'intervalle précédent.");
  }
      
   sendDoneSignal();  // **Couper l’alimentation des capteurs**
   digitalWrite(LED_PIN, LOW);
   LowPower.deepSleep(sleepDurationMs);
}
