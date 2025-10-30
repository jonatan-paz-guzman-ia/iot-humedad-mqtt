# main.py
from fastapi import FastAPI
from models import SensorData
from database import save_sensor_data
from mqtt_client import start_mqtt

app = FastAPI(title="API IoT Humedad y Temperatura")

@app.on_event("startup")
def startup_event():
    print("Iniciando API y cliente MQTT...")
    start_mqtt()

@app.get("/")
def root():
    return {"message": "API de sensores funcionando correctamente"}

@app.post("/data")
def post_data(data: SensorData):
    save_sensor_data(
        data.sensor_id,
        data.humedad_aire,
        data.humedad_suelo,
        data.temperatura
    )
    return {"status": "ok", "data": data.dict()}
