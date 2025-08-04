#include <Arduino_LPS22HB.h>  // Capteur de pression intégré
#include <Arduino_HTS221.h>   // Capteur de température & humidité intégré
#include <PDM.h>              // Gestion du microphone PDM
#include <azdefd-project-1_inferencing.h> // Edge Impulse (modèle de détection)

// Structure pour stocker les données d'inférence
typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

static inference_t inference;
static signed short sampleBuffer[2048];
static bool debug_nn = false;  // Mode debug
bool etat_queen = 0;  // Variable pour indiquer si la reine est détectée

void setup() {
  Serial.begin(9600);   
  Serial1.begin(115200);  
  delay(1000);

 
  while (!Serial);
  Serial.println("Nano BLE prêt !");

  // Initialisation des capteurs
  if (!BARO.begin()) {  
    Serial.println("Erreur : Capteur de pression non détecté !");
    while (1);
  }

  if (!HTS.begin()) {  
    Serial.println("Erreur : Capteur de température/humidité non détecté !");
    while (1);
  }

  // Initialisation du microphone PDM
  if (!microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT)) {
    Serial.println("Erreur : Impossible d'initialiser le microphone !");
    while (1);
  }
}

void loop() {
  float pressure = BARO.readPressure();  
  float temperature_hts = HTS.readTemperature();
  float temperature_baro = BARO.readTemperature();
  float temperature = (temperature_hts + temperature_baro) / 2;
  float humidity = HTS.readHumidity();

  Serial.println("Analyse du son pour la détection de la reine...");

  // Enregistrement du son
  if (!microphone_inference_record()) {
    Serial.println("Erreur : Échec de l'enregistrement audio.");
    return;
  }

  // Création du signal audio
  signal_t signal;
  signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
  signal.get_data = &microphone_audio_signal_get_data;

  // Exécuter l'inférence Edge Impulse
  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn);

  if (r != EI_IMPULSE_OK) {
    Serial.print("Erreur d'inférence : ");
    Serial.println(r);
    // return;
  }

  // Afficher les résultats de classification
  Serial.println("Résultat de l'inférence :");
  for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    Serial.print(result.classification[i].label);
    Serial.print(": ");
    Serial.println(result.classification[i].value);
  }

  // Déterminer si la reine est détectée
  etat_queen = (result.classification[0].value < result.classification[1].value);

  // Envoi des données sur Serial1 pour le MKR WAN 1310
  // Serial1.print("Sent Data: ");
  Serial1.print("T:");
  Serial1.print(temperature, 2);
  Serial1.print(", P:"); 
  Serial1.print(pressure, 2);
  Serial1.print(", H:"); 
  Serial1.print(humidity, 2);
  Serial1.print(", Q:"); 
  Serial1.println(etat_queen ? "1" : "0");

  // Debug affichage sur le Moniteur Série
  Serial.print("Envoyé: ");
  Serial.print("T:");
  Serial.print(temperature, 2);
  Serial.print(", P:"); 
  Serial.print(pressure, 2);
  Serial.print(", H:"); 
  Serial.print(humidity, 2);
  Serial.print(", Q:"); 
  Serial.println(etat_queen ? "1" : "0");
  Serial.println(" ");

  delay(2000);
}

/** 
 * Fonction d'initialisation du microphone PDM 
 */
static bool microphone_inference_start(uint32_t n_samples) {
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));

    if (inference.buffer == NULL) {
        return false;
    }

    inference.buf_count  = 0;
    inference.n_samples  = n_samples;
    inference.buf_ready  = 0;

    // Configure la réception des données
    PDM.onReceive(&pdm_data_ready_inference_callback);
    PDM.setBufferSize(4096);

    // Initialiser PDM avec 1 canal et fréquence 16 kHz
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY*8)) {
        Serial.println("Échec de l'initialisation PDM !");
        microphone_inference_end();
        return false;
    }

    PDM.setGain(127);
    return true;
}

/** 
 * Fonction d'acquisition du microphone 
 */
static bool microphone_inference_record(void) {
    inference.buf_ready = 0;
    inference.buf_count = 0;

    while (inference.buf_ready == 0) {
        delay(10);
    }

    return true;
}

/** 
 * Fonction de récupération des données audio 
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr) {
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);
    return 0;
}

/** 
 * Fonction pour arrêter le microphone et libérer la mémoire 
 */
static void microphone_inference_end(void) {
    PDM.end();
    free(inference.buffer);
}

/** 
 * Fonction callback lorsque le buffer PDM est plein 
 */
static void pdm_data_ready_inference_callback(void) {
    int bytesAvailable = PDM.available();
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

    if (inference.buf_ready == 0) {
        for (int i = 0; i < bytesRead >> 1; i++) {
            inference.buffer[inference.buf_count++] = sampleBuffer[i];

            if (inference.buf_count >= inference.n_samples) {
                inference.buf_count = 0;
                inference.buf_ready = 1;
                break;
            }
        }
    }
}
