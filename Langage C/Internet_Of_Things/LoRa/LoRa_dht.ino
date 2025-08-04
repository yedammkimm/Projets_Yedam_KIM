#include <SoftwareSerial.h>
#include <DHT.h>

// Define pins for serial communication with the LoRa E5 module
const int RX_PIN = D5;  // ESP8266 RX pin (receives data from the LoRa module)
const int TX_PIN = D6;  // ESP8266 TX pin (sends data to the LoRa module)

// Define pin and type for the DHT sensor
#define DHT_PIN D3  // Pin connected to the DHT11 sensor
#define DHT_TYPE DHT11  // Specify the sensor type: DHT11

// Initialize the DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize SoftwareSerial for LoRa communication
SoftwareSerial loraSerial(RX_PIN, TX_PIN);

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  loraSerial.begin(9600);  // Start communication with LoRa E5 module

  // Initialize the DHT sensor
  dht.begin();

  delay(1000);

  Serial.println("Initializing LoRa E5 module...");

  // Check LoRa module communication and configure for LoRaWAN OTAA
  executeATCommand("AT", "OK", 2000);
  setupLoRaWANParameters();
  Serial.println("LoRa module ready for data transmission.");
}

void loop() {
  // Capture temperature and humidity data from DHT sensor
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Ensure readings are valid
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print values for debugging
  Serial.printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp, hum);

  // Transmit data via LoRa
  String Data = "T:" + String(temp) + " H:" + String(hum);
  transmitDataToLoRa(Data);
  delay(60000);  // Send data every 60 seconds
}

// Function to configure LoRaWAN parameters for OTAA
void setupLoRaWANParameters() {
  executeATCommand("AT+MODE=LWOTAA", "+MODE:", 2000);
  executeATCommand("AT+ID=DevEUI,\"70B3D57ED006AEE8\"", "+ID:", 2000);
  executeATCommand("AT+ID=AppEUI,\"0123456789ABCDEF\"", "+ID:", 2000);
  executeATCommand("AT+KEY=APPKEY,\"3DDAE28111D660DA0F9263F5B424A97F\"", "+KEY:", 2000);
  joinLoRaNetwork();
}

// Function to join the LoRaWAN network
void joinLoRaNetwork() {
  Serial.println("Attempting to join the LoRa network...");

  String response = executeATCommand("AT+JOIN", "+JOIN: Done", 10000);

  // Retry if join fails
  if (response.indexOf("+JOIN: Join failed") != -1) {
    Serial.println("Join failed. Retrying...");
    delay(5000);
    response = executeATCommand("AT+JOIN", "+JOIN: Done", 10000);
  }
}

// Function to transmit temperature and humidity data to LoRaWAN
void transmitDataToLoRa(String message){
    // Convertir le message en hexadécimal
    String hexMessage = convertToHex(message);
    String command = "AT+MSGHEX=\"" + hexMessage + "\"\r\n";
    executeATCommand(command, "OK", 5000); // Attendre 5 secondes pour la réponse
}


// Function to execute AT commands and check for the expected response
String executeATCommand(String command, String expectedResponse, unsigned long timeout) {
  loraSerial.println(command);
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < timeout) {
    while (loraSerial.available()) {
      char c = loraSerial.read();
      response += c;
    }

    // Check for the expected response
    if (response.indexOf(expectedResponse) != -1) {
      Serial.println("Command executed successfully: " + command);
      Serial.println("Response: " + response);
      return response;
    }
  }

  Serial.println("Command execution failed: " + command);
  Serial.println("Received response: " + response);
  return response;
}

// Utility function to convert strings into hexadecimal format
String convertToHex(String input) {
  String result = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    result += String(c, HEX);
  }
  return result;
}
