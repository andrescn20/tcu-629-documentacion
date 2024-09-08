import serial
import csv
from datetime import datetime

ser = serial.Serial('COM8', 9600)
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(line)