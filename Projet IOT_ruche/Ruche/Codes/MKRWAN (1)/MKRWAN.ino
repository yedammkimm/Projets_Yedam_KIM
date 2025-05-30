#include <MKRWAN.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTCZero.h>
#include <ArduinoLowPower.h>
#include <Adafruit_TSL2591.h>
#include <HX711.h>

//LoRa & RTC
LoRaModem modem;
RTCZero rtc;  

//Light Sensor
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // Light Sensor

// HX711 Weight Sensor
#define LOADCELL_DOUT_PIN 4  // HX711 Data Out
#define LOADCELL_SCK_PIN 5    // HX711 Clock
HX711 scale;

// DHT22 (Temperature and Humidity Sensor)
#define DHTPIN_INT A6  
#define DHTPIN_EXT A5 
#define DHTTYPE DHT22
DHT dht_int(DHTPIN_INT, DHTTYPE);
DHT dht_ext(DHTPIN_EXT, DHTTYPE);

// DS18B20 Temperature Sensors
#define ONE_WIRE_BUS_1 0  
OneWire oneWire1(ONE_WIRE_BUS_1);
DallasTemperature sensors1(&oneWire1);

#define ONE_WIRE_BUS_2 1  
OneWire oneWire2(ONE_WIRE_BUS_2);
DallasTemperature sensors2(&oneWire2);

// Battery Measurement
#define BATTERY_PIN A1  
#define VREF 3.3
#define RESOLUTION 4095  
#define V_MAX 3.3  
#define V_MIN 2.6  

//TPL 
#define TPL_DONE_PIN 6  // DONE pin to TPL5110
#define TPL_DELAY_PIN 3 // DELAY pin to TPL5110

String appEui = "A8610A34304B860A";  
String appKey = "64726FB32755F50C800F64EC23079E2E";  

bool connected = false;
int err_count = 0;

static float previous_weight = 0;  // Stores the last weight measurement


void connectToTTN() {
   Serial.println("Connecting to TTN...");
   int attempt = 1;
   while (!connected) {  
      Serial.print("Attempt ");
      Serial.print(attempt);
      Serial.println("...");

      if (modem.joinOTAA(appEui, appKey)) {
         connected = true;
         modem.minPollInterval(10);  
         Serial.println("Connected to TTN!");
         modem.dataRate(5);  
         delay(50);          
      } else {
         Serial.println("Connection failed, retrying...");
         attempt++;
         delay(attempt*2000);  
      }
   }
}


// Configure Light Sensor (TSL2591)
void configureLightSensor() {
   tsl.setGain(TSL2591_GAIN_LOW); 
   tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS); // Increase integration time for better accuracy
}


float correctionfactor = 4.0;

// Read light sensor (Lux Value)
float readLightSensor() {
    Serial.println("Reading TSL2591 Light Sensor...");

    // Request luminosity
    uint32_t lum = tsl.getFullLuminosity();
    uint16_t ir = lum >> 16;
    uint16_t full = lum & 0xFFFF;

    Serial.print("Raw IR: "); Serial.println(ir);
    Serial.print("Raw Full Spectrum: "); Serial.println(full);

    // **Avoid getting stuck if sensor returns invalid data**
    if (full == 0 && ir == 0) {
        Serial.println("ERROR: Light sensor returned zero values! Resetting sensor...");
        if (!tsl.begin()) {
            Serial.println("TSL2591 reinitialization failed!");
            return -1.0;  // Indicate error
        }
        configureLightSensor();
        return 0;  // Return default value to avoid blocking execution
    }

    float lux = tsl.calculateLux(full, ir);
    return (lux > 0) ? lux * correctionfactor : 0;  // Apply correction factor
}


// Read HX711 Weight Sensor
float readWeightSensor() {
    float weight = 0; // Variable to store the weight

    if (scale.is_ready()) {
        float avg_weight = scale.get_units(20); // Get the raw value
        // Correct the value using the given equation
        weight = (avg_weight - 373036) / 28728;

        // If the weight is negative, set it to 0
        if (weight < 0) weight = 0;
    }
    return weight; // Return the weight in kg
}

void setup() {
   Serial.begin(9600);
   Serial1.begin(115200);
   delay(1000);
   analogReadResolution(12);

   Serial.println("Initializing LoRa modem...");
   if (!modem.begin(EU868)) {
      Serial.println("Modem initialization failed!");
      while (true);
   }
   Serial.println("LoRaWAN Modem Initialized");
}



void wakeUp() {
    Serial.println("RTC Wake-Up Triggered");
    digitalWrite(TPL_DONE_PIN, LOW); // PAS DONE -> TPL active les capteurs
    delay(2000);  // Attendre pour s'assurer que l'alim est bien rétablie
    digitalWrite(TPL_DELAY_PIN, HIGH); // PAS DONE -> TPL active les capteurs
    delay(5000);  // Attendre pour s'assurer que l'alim est bien rétablie

    // Réinitialisation complète des capteurs (pour éviter qu'ils restent en état bloqué)
    dht_int.begin();
    dht_ext.begin();

    if (!tsl.begin()) {
    delay(200);
    Wire.end();
    delay(100);
    Wire.begin();
    tsl.begin();
  }
  
  configureLightSensor();

    // Initialize HX711 Load Cell
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.power_up();

    // **RESET ONE WIRE (DS18B20)**
    pinMode(ONE_WIRE_BUS_1, INPUT);
    pinMode(ONE_WIRE_BUS_2, INPUT);
    delay(100);
    pinMode(ONE_WIRE_BUS_1, OUTPUT);
    pinMode(ONE_WIRE_BUS_2, OUTPUT);
    delay(100);
    pinMode(ONE_WIRE_BUS_1, INPUT);
    pinMode(ONE_WIRE_BUS_2, INPUT);
    delay(100);
    sensors1.begin();
    sensors2.begin();

   Serial1.end();
   Serial1.begin(115200);
    if (!connected) {  // Avoid reconnecting if already connected
        connectToTTN();
    }
}

// **Signale au TPL5110 de couper l'alimentation des capteurs**
void sendDoneSignal() {
    Serial.println("Signaling TPL5110 to cut power...");
    delay(1000); 
    digitalWrite(TPL_DELAY_PIN, LOW); 
    delay(1000);
    digitalWrite(TPL_DONE_PIN, HIGH); // DONE -> TPL coupe l’alimentation des capteurs
    delay(1000);  
}

void loop() {
   wakeUp();
   delay(2500);
   // Read DHT22
   Serial.println("\nReading sensors...");
   
   // Internal DHT22
   float temp_int = dht_int.readTemperature();
   float hum_int = dht_int.readHumidity();
   if (isnan(temp_int) || isnan(hum_int)) {
      Serial.println("DHT22 (Internal) read failed");
      temp_int = hum_int = 0;
   }

   // External DHT22
   float temp_ext = dht_ext.readTemperature();
   float hum_ext = dht_ext.readHumidity();
   if (isnan(temp_ext) || isnan(hum_ext)) {
      Serial.println("DHT22 (External) read failed");
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
   
   // Read DS18B20 #1
   sensors1.requestTemperatures();
   sensors2.requestTemperatures();
   float t_1 = sensors1.getTempCByIndex(0);
   float t_2 = sensors2.getTempCByIndex(0);
   if (t_1 == DEVICE_DISCONNECTED_C) {
      Serial.println("DS18B20 #1 read failed!");
      t_1 = 0;
   }
   if (t_2 == DEVICE_DISCONNECTED_C) {
      Serial.println("DS18B20 #2 read failed!");
      t_2 = 0;
   }

   Serial.print("Internal Humidity: ");
   Serial.print(hum_int);
   Serial.print(" % | Internal Temperature: ");
   Serial.print(temp_int);
   Serial.println(" °C");

   Serial.print("External Humidity: ");
   Serial.print(hum_ext);
   Serial.print(" % | External Temperature: ");
   Serial.print(temp_ext);
   Serial.println(" °C");

   Serial.print("Temperature #1 (DS18B20): ");
   Serial.print(t_1);
   Serial.println(" °C");

   Serial.print("Temperature #2 (DS18B20): ");
   Serial.print(t_2);
   Serial.println(" °C");

   Serial.print("Light Intensity: ");
   Serial.print(lux);
   Serial.println(" Lux");

   Serial.print("Weight: ");
   Serial.print(weight_kg);
   Serial.println(" kg");

   Serial.print("Measured Voltage: ");
   Serial.print(voltage);
   Serial.print(" V | Battery: ");
   Serial.print(batteryPercentage);
   Serial.println(" %");
                       
                       
 
   float temp_nano = -1.0;
   float pressure = -1.0; 
   float hum_nano = -1.0;
   int queenDetected = 0; 
                  
  if (Serial1.available()) {
    String receivedData = Serial1.readString();
    receivedData.trim(); // Supprimer les espaces et caractères parasites

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
      int qEnd = receivedData.indexOf(",", hStart);

      temp_nano = receivedData.substring(tStart, pEnd).toFloat();
      pressure = receivedData.substring(pStart, hEnd).toFloat();
      hum_nano = receivedData.substring(hStart, qEnd).toFloat();
      queenDetected = receivedData.substring(qStart).toInt();

      // Affichage des valeurs extraites
      Serial.print("Température : ");
      Serial.print(temp_nano);
      Serial.println(" °C");

      Serial.print("Pression : ");
      Serial.print(pressure);
      Serial.println(" hPa");

      Serial.print("Humidité : ");
      Serial.print(hum_nano);
      Serial.println(" %");

      Serial.print("Reine : ");
      Serial.print(queenDetected);
      Serial.println("");
    } else {
      Serial.println("Format des données incorrect !");
    }
  }else{
    Serial.print("SERIAL1 NOT AVALAIABLE ");
  }
  
   // Encode Payload
   uint8_t payload[29]; 
   int16_t hum_i = (int16_t)(hum_int * 100);         // Internal Humidity (DHT22)
   int16_t temp_i = (int16_t)(temp_int * 100);   // Internal Temperature (DHT22)
   int16_t hum_e = (int16_t)(hum_ext * 100);         // External Humidity (DHT22)
   int16_t temp_e = (int16_t)(temp_ext * 100);   // External Temperature (DHT22)
   int16_t temp_1 = (int16_t)((t_1 == 0 ? t_1 : t_1 - 0.3) * 100);  // Internal Temperature #1 (DS18B20)
   int16_t temp_2 = (int16_t)((t_2+1) * 100);           // Internal Temperature #2 (DS18B20)
   int16_t temp_3 = (int16_t)((temp_nano == -1 ? 0: temp_nano)* 100);     //Internal Temperature #3 (Nano)
   int16_t hum_3 = (int16_t)((hum_nano == -1 ? 0: hum_nano)* 100);       // Internal Humidity (Nano)
   int16_t batt = (int16_t)(batteryPercentage);     // Battery Level (%)
   int16_t lux_value = (int16_t)(lux* 10);         // Light Intensity (TSL2591)
   int16_t weight_value = weight_kg * 100;          // Weight (HX711 Load Cell)
   int16_t pressure_value = (int16_t)((pressure == -1 ? 0: pressure) * 10); // Pression (hPa)
   uint8_t queen_present = queenDetected ? 1 : 0;   // Présence de la reine (0 ou 1)
   uint8_t colony_size = (uint8_t)(temp_i > 35) + (uint8_t)(temp_1 > 35) + (uint8_t)(temp_2 > 35); // Taille de la colonie (0 à 3) 
   int16_t weight_diff_value = (int16_t)(weight_diff * 100);  // Scale to 2 decimals

   
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

  payload[27] = weight_diff_value >> 8;
  payload[28] = weight_diff_value & 0xFF; // Weight difference (kg * 100)

  previous_weight = weight_kg;
  
  modem.beginPacket();
   modem.write(payload, sizeof(payload));
   int err = modem.endPacket();

   if (err <= 0) {
      Serial.println("Send failed!");
   } else {
      Serial.println("Message sent!");
   }

   Serial.println("Envoi d'un uplink...");
   modem.beginPacket();
   modem.endPacket(true);  // "true" pour recevoir un downlink

   delay(1000);  // Attendre la réponse downlink

   int packetSize = modem.parsePacket();
   int intervalSeconds = 60;  // Par défaut 1 minute

   if (packetSize) {
       Serial.print(" Downlink reçu : ");
       int r_payload[2];
       for (int i = 0; i < 2; i++) {
           r_payload[i] = modem.read();
           Serial.print(r_payload[i], HEX);
           Serial.print(" ");
       }
       Serial.println();

       if (r_payload[0] == 0x00) {  
           intervalSeconds = r_payload[1];  // Intervalle en secondes
           Serial.print("Nouvel intervalle en secondes : ");
           Serial.print(intervalSeconds);
           Serial.println(" s.");
       } 
       else if (r_payload[0] == 0x01) {  
           intervalSeconds = r_payload[1] * 60;  // Intervalle en minutes
           Serial.print("Nouvel intervalle en minutes : ");
           Serial.print(intervalSeconds / 60);
           Serial.println(" min.");
       } 
       else {
           Serial.println("Payload inconnu, utilisation de l'intervalle par défaut (1 min).");
       }
   } else {
       Serial.println("Pas de downlink reçu, utilisation de l'intervalle par défaut (1 min).");
   }

   // **Reconfiguration complète du RTC**
   rtc.begin();
   rtc.setTime(0, 0, 0);
   rtc.setDate(1, 1, 2025);
   rtc.setAlarmTime(0, intervalSeconds / 60, intervalSeconds % 60);
   rtc.enableAlarm(rtc.MATCH_MMSS);

   sendDoneSignal();  // **Couper l’alimentation des capteurs**

   Serial.println("Going to sleep..."); 
   delay(2000);
   LowPower.deepSleep();
}
