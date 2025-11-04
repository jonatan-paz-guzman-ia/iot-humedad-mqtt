#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- SENSOR CAPACITIVO DE HUMEDAD DEL SUELO ---
#define SOIL_PIN 34     // Pin analógico (solo lectura ADC)

const char* ssid = "Fliapazmunoz";
const char* password = "Fliapazmunoz123";
const char* mqtt_server = "ec2-52-91-242-207.compute-1.amazonaws.com"; 
const char* sensor_id = "esp32_1";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect(sensor_id)) {
      Serial.println("conectado!");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float humedad_aire = dht.readHumidity();
  float temperatura = dht.readTemperature();

 // --- Lectura del sensor de humedad del suelo ---
  int valor_analogico = analogRead(SOIL_PIN);
  // El sensor devuelve valores entre 0 (húmedo) y 4095 (seco)
  float humedad_suelo = map(valor_analogico, 4095, 0, 0, 100);


  String mensaje = String(sensor_id) + "," +
                   String(humedad_aire) + "," +
                   String(humedad_suelo) + "," +
                   String(temperatura);

  client.publish("sensor/datos", mensaje.c_str());
  Serial.println("Mensaje enviado: " + mensaje);

  delay(5000); // cada 5 segundos
}
