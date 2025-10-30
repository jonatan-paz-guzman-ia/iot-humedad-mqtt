from fastapi import FastAPI
from database import SessionLocal, SensorData
from mqtt_client import start_mqtt

app = FastAPI(title="API IoT Humedad")

@app.on_event("startup")
def startup_event():
    # Inicia el cliente MQTT cuando arranca la API
    start_mqtt()

@app.get("/")
def root():
    return {"mensaje": "API de Humedad con MQTT está funcionando"}

@app.get("/datos")
def obtener_datos():
    db = SessionLocal()
    datos = db.query(SensorData).all()
    db.close()
    return datos
