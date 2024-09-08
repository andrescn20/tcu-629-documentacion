# Este código está pensado para hacer pruebas con arduino. Dado que estos no poseen nativamente wifi, 
# se lee la información en el puerto serial y se exporta a un archivo csv.
# Este archivo puede ser cargado en excel o almacenado manualmente en la base de datos. 

import serial
import csv
from datetime import datetime

# Camnbiar el puerto "COM8" por el puerto al que está conectado el dispositivo.
# Este puerto varía entre computadoras.
ser = serial.Serial('COM8', 9600)


with open('temperature_data.csv', 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(['Fecha', 'Temperatura_C']) 
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line:
                timestamp = datetime.now().isoformat() + 'Z'
                
                temperature = line
                
                # Write the timestamp and temperature to the CSV file
                csvwriter.writerow([timestamp, temperature])
                print(f"Written to CSV: {timestamp}, {temperature}")