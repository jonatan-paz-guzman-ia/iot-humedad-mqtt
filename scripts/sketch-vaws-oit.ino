#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// --- DHT11 ---
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- SENSOR CAPACITIVO DE HUMEDAD DEL SUELO ---
#define SOIL_PIN 34 

// --- CREDENCIALES WIFI ---
const char* ssid = "Fliapazmunoz";
const char* password = "Fliapazmunoz123";

// --- AWS IoT ---
const char* aws_endpoint = "a2feyrxd9ivrn-ats.iot.us-east-1.amazonaws.com";  // ← endpoint
const char* sensor_id = "esp32_1";

// ---- CERTIFICADOS ----
// Certificado del ESP32
static const char certificate_pem_crt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUcbroOY/aSpj1fuZGez/3WdlHHvgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MTExNDAxMTEx
MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANEtHN9fl/hci4cllrM5
6TucA6/hquCPKHPsjEN0WzZbP8874TRgStNVlkfksvGxOzac7n9gw+Qg5/uES3w/
ARlvjny2GLW9YN/5oq81fQE8E1SQlCgzi+DyUMMg0Rpv3jiAWiKogR0oaPQMEbfr
GTJQCYyjBSVjfOnorgcJOejRzWEZjOBCQH9tWicQjG5DTlWMz8RDuloRZn+noPwx
SneQ9fnGJLBso5ohXWGmKn5b7/GPvUvqXv3GkEef5gyeq85kICEgjhBN3F4Qxl3G
o9qtYWkOdZs49AyYcVBvHMch8iHlBb1yZY7RMFOuuU85MLeG5raFg/4kLol7Lwpp
3PkCAwEAAaNgMF4wHwYDVR0jBBgwFoAU34rXyadF+F4mgJ9KEZCOaadanBcwHQYD
VR0OBBYEFGIzQz+lVyLFAKqgYrpulZQQKOdhMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBIshj0HHdl9T+jwwq0RL9TudyF
zX7YQ5ZVSrS7dyQBvpDtXA3aT54Blyeg2m8+j2/lXECCABRM/ENFRARcWKaoM0VC
wPi6/cuzxxgSCZGflNVGYQ736/tlq1vp5encAL8sQfBc1lCZcmRLEWIGUVZKw3Ri
EFVVwAUL+/m4oPnLnmpzzs/G8kE3cWFyB1o7f7xGewgP4tsdT0XVQ43OaKfboxjr
asOGoyAQyfipK6ZWsqF99hVbmLl87dKSh2VhUvrt+SmDp3FgDg8kB1pEuk/zHwM5
66WHTu+Jaj5Ir7pCxwytG3wirFrzPJaxDX/E7twjHEDewGwLXmIMaqPZ9CKs
-----END CERTIFICATE-----
)EOF";

// Clave privada
static const char private_pem_key[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA0S0c31+X+FyLhyWWsznpO5wDr+Gq4I8oc+yMQ3RbNls/zzvh
NGBK01WWR+Sy8bE7Npzuf2DD5CDn+4RLfD8BGW+OfLYYtb1g3/mirzV9ATwTVJCU
KDOL4PJQwyDRGm/eOIBaIqiBHSho9AwRt+sZMlAJjKMFJWN86eiuBwk56NHNYRmM
4EJAf21aJxCMbkNOVYzPxEO6WhFmf6eg/DFKd5D1+cYksGyjmiFdYaYqflvv8Y+9
S+pe/caQR5/mDJ6rzmQgISCOEE3cXhDGXcaj2q1haQ51mzj0DJhxUG8cxyHyIeUF
vXJljtEwU665Tzkwt4bmtoWD/iQuiXsvCmnc+QIDAQABAoIBAFDHiDfnNcwPCT8m
z/zk52JCzNW2Y4QXJPsnpb9DBxqFwK2qJT1R1nOowMGA224lEIX1stD0MJI/9vaV
z4Tq2+TTxQX6WK6OtQOp5x6cnGrGpXxp3R1tUp6LVY8VdEsuWnYdczGhO0NkYKum
oY0SOIfKivWJ2ynmMs4uMk67TGbRM91MejHKRtaW/SQa68RY3m2FwNM1bUehWHN8
xsY0Q0Zj9sW/6txYa01XLc9+luccE+a/O7gOzOnQrqtgzOgopepd6glXj6Tq/8re
8DpPvbCI2DYvrAoftqFp2rm9DT1tdClYQgwn+4ySCGzRWyNRDrAy/av0Dx4ETjM+
tWlJFlECgYEA9dVIa976BtzNA1x2GAJVKrutFUnSzC9Towl9mG1zxCWqSEBJii3r
F/IMrcBE74wRvl2Qd4Vg4Mf3Y1+rbbNgu15aANXgGityxxWndlvJ22QroNSP6fRq
4Jj4/PHD8M6ttykOgIcHPZB/o9MGjTb8UUWuTkvnWIvR6aI+iG5oWfUCgYEA2dO7
H7UkVLG1oj+Kv1xbWk8cv6MmwJQC+sByQHFqqG5fUX0sm64zLaFRsrVp3Ut+d79b
1HwdrYBpTXSZV2T4Pwp/OrkihcrqG1fRt+VsTPn51wEw6SgRGa5i0Flzmaw4dHpL
m33e2B62ADBoKkRl1qRfpuGnVUqdpM7UlM6dwHUCgYAxKzuU98AiZz9KENA99Rl7
tpXj0CQtWrnrvhY0yo+BB9eLI/CRhqWi4HLQvcgL8YQLA0PF5BOAYf6FDbmH83zB
fRmbWSu9kDsgvqAD+E/jZaGkRV+Td6N40J/cV67NF0l/fOK9qkQ0eBZPegGxdPH6
f2lhl6Tp/O07QYwoVYzhkQKBgQCWPFtWbiglpf/hbLP0ce2r5t6sKgnjTvKTIcJr
Qg5PJvjlClGG+mnf8eCYQllGwPfz/W6HqicJ8IQwYNgWF9TEHihMRz5RSveWbkZ9
8GV23f58VRz34qOQs98BVoDNn6O8alDVaMYXB299x+VAKbaXiP1T61TvFayO6/xg
ufTBtQKBgQDkY2OYCibXHpCiaWsyvunq5w427MEY9SUL/t+WZdcbqcYAx2k6woGC
STYdfyv0CD0lKimHT4ZOuOYVgZe0lyLyjWwYa24x43I9/e/xMwqGTT3HxsXAI02l
/92hAgfCDAKpmR++5VShwaIBcwa2VHb4+MOh5ovAg5vr9DsbuGtm/g==
-----END RSA PRIVATE KEY-----
)EOF";

// Amazon Root CA1
static const char aws_root_ca[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";


// --- OBJETOS MQTT ---
WiFiClientSecure net;
PubSubClient client(net);


// ---------------- WIFI ----------------
void setup_wifi() {
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

// ---------------- MQTT RECONNECT ----------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a AWS IoT... ");
    if (client.connect(sensor_id)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      delay(3000);
    }
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  setup_wifi();

  // Configurar certificados
  net.setCACert(aws_root_ca);
  net.setCertificate(certificate_pem_crt);
  net.setPrivateKey(private_pem_key);

  // Puerto MQTT seguro de AWS IoT
  client.setServer(aws_endpoint, 8883);

  dht.begin();
}


// ---------------- LOOP ----------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // ---- Sensores ----
  float humedad_aire = dht.readHumidity();
  float temperatura = dht.readTemperature();

  int valor_analogico = analogRead(SOIL_PIN);
  float humedad_suelo = map(valor_analogico, 4095, 0, 0, 100);

  // ---- Crear JSON ----
  String mensaje = "{";
  mensaje += "\"id\":\"" + String(sensor_id) + "\",";
  mensaje += "\"humedad_aire\":" + String(humedad_aire) + ",";
  mensaje += "\"humedad_suelo\":" + String(humedad_suelo) + ",";
  mensaje += "\"temperatura\":" + String(temperatura);
  mensaje += "}";

  // ---- Publicar a AWS IoT ----
  client.publish("sensor/datos", mensaje.c_str());
  Serial.println("Enviado → " + mensaje);

  delay(5000);
}
