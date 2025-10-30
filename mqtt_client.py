import paho.mqtt.client as mqtt
import json
from database import SessionLocal, SensorData

BROKER = "localhost"  # Cambia por la IP de tu broker si está en otra máquina
PORT = 1883
TOPIC = "sensores/lecturas"

# Función que se ejecuta al recibir un mensaje
def on_message(client, userdata, msg):
    try:
        data_str = msg.payload.decode()
        print("Mensaje recibido:", data_str)
        data = json.loads(data_str)

        db = SessionLocal()
        nuevo_registro = SensorData(
            temperatura=data.get("temperatura"),
            humedad_aire=data.get("humedad_aire"),
            humedad_suelo=data.get("humedad_suelo"),
        )
        db.add(nuevo_registro)
        db.commit()
        db.close()

    except Exception as e:
        print("Error procesando mensaje:", e)

# Función que inicia el cliente MQTT
def start_mqtt():
    client = mqtt.Client()
    client.on_message = on_message
    client.connect(BROKER, PORT, 60)
    client.subscribe(TOPIC)
    client.loop_start()
    print(f"Suscrito al tópico '{TOPIC}' en el broker {BROKER}")
