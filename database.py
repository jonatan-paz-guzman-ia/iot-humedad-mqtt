from sqlalchemy import create_engine, Column, Integer, Float, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from datetime import datetime

# URL de la base de datos SQLite (archivo local)
SQLALCHEMY_DATABASE_URL = "sqlite:///./iot_data.db"

# Crear el motor de base de datos
engine = create_engine(SQLALCHEMY_DATABASE_URL, connect_args={"check_same_thread": False})

# Crear sesión
SessionLocal = sessionmaker(bind=engine, autoflush=False, autocommit=False)

# Clase base para definir tablas
Base = declarative_base()

# Definición de la tabla de datos
class SensorData(Base):
    __tablename__ = "sensor_data"
    id = Column(Integer, primary_key=True, index=True)
    temperatura = Column(Float)
    humedad_aire = Column(Float)
    humedad_suelo = Column(Float)
    timestamp = Column(DateTime, default=datetime.utcnow)

# Crear la tabla automáticamente si no existe
Base.metadata.create_all(bind=engine)
