#include <ESP8266WiFi.h>

// Wi-Fi network credentials (the one providing Internet access)
// Informations du réseau Wi-Fi auquel l’ESP se connecte (source Internet)
const char* ssid_sta = "SSID_EXISTANT";
const char* password_sta = "MOT_DE_PASSE_EXISTANT";

// Access Point credentials (the hotspot that ESP8266 will create)
// Informations du réseau Wi-Fi que l’ESP va créer (le hotspot)
const char* ssid_ap = "Hotspot_ESP8266";
const char* password_ap = "motdepasseap";  // minimum 8 caractères

void setup() {
  Serial.begin(115200);

  // Set Wi-Fi mode to both Station (STA) and Access Point (AP)
  // Configure l’ESP en mode AP + STA
  WiFi.mode(WIFI_AP_STA);

  // Connect to external Wi-Fi (for Internet access)
  // Connexion au Wi-Fi existant (pour accéder à internet)
  WiFi.begin(ssid_sta, password_sta);
  Serial.println("Connexion au Wi-Fi existant...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecté ! Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Create the ESP8266 Access Point (hotspot)
  // Création du hotspot de ESP8266 (hotspot)
  bool result = WiFi.softAP(ssid_ap, password_ap);
  if (result) {
    Serial.println("Point d'accès Wi-Fi créé !");
    Serial.print("Adresse IP AP : ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Erreur lors de la création du point d'accès.");
  }
}

void loop() {
  // Nothing to do here for now
  // Rien ici pour l’instant
}
