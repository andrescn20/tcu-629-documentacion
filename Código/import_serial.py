import serial
import csv
from datetime import datetime

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