void setup() {
    Serial.begin(9600);   // Initialise la communication série
    pinMode(LED_BUILTIN, OUTPUT);  // La LED intégrée de la Pico (GPIO 25)
}

void loop() {
    if (Serial.available()) {  // Vérifie si des données sont reçues
        String command = Serial.readStringUntil('\n');  // Lire jusqu'à un saut de ligne

        if (command == "ON") {
            digitalWrite(LED_BUILTIN, HIGH);
            // Serial.println("LED ON");
        } else if (command == "OFF") {
            digitalWrite(LED_BUILTIN, LOW);
            // Serial.println("LED OFF");
        }
    }
}
