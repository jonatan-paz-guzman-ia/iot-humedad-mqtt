# database.py
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

INFLUX_URL = "http://localhost:8086"
INFLUX_TOKEN = "MKaO8LC8VDc43vjLZvOgGEL4XYzUBGGWObkWS6tANfExDwO2yTKtoCqdTTR6jMfGLyEjrq7ZiDAzoUAjGvewOQ=="
INFLUX_ORG = "iot_org"
INFLUX_BUCKET = "iot_data"

client = InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)
write_api = client.write_api(write_options=SYNCHRONOUS)

def save_sensor_data(sensor_id: str, humedad_aire: float, humedad_suelo: float, temperatura: float):
    """Guarda varias lecturas del sensor en InfluxDB"""
    point = (
        Point("sensores")
        .tag("sensor_id", sensor_id)
        .field("humedad_aire", humedad_aire)
        .field("humedad_suelo", humedad_suelo)
        .field("temperatura", temperatura)
        .time(write_precision=WritePrecision.NS)
    )
    write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=point)
