# models.py
from pydantic import BaseModel

class SensorData(BaseModel):
    sensor_id: str
    humedad_aire: float
    humedad_suelo: float
    temperatura: float
