# Este script lee la información que se envía por el puerto serial y la imprime en la consola.
# La intención es tener acceso a la información enviada por el dispositivo y así leer los 
# mensajes que este imprime. 

import serial
import csv
from datetime import datetime

ser = serial.Serial('COM8', 9600) # Cambiar el puerto COM8 por el puerto al que está conectado el dispositivo. 
# El puerto correcto varía entre comutadoras. 
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(line)