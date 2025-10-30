# mqtt_client.py
import paho.mqtt.client as mqtt
from database import save_sensor_data

BROKER = "localhost"
PORT = 1883
TOPIC = "sensor/datos"

def on_connect(client, userdata, flags, rc):
    print("Conectado a MQTT con código:", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        print(f"Mensaje recibido en {msg.topic}: {payload}")

        # Formato esperado: sensor_id,humedad_aire,humedad_suelo,temperatura
        sensor_id, ha, hs, t = payload.split(",")
        save_sensor_data(sensor_id, float(ha), float(hs), float(t))
        print("Datos guardados en InfluxDB")
    except Exception as e:
        print("Error procesando mensaje:", e)

def start_mqtt():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(BROKER, PORT, 60)
    client.loop_start()
