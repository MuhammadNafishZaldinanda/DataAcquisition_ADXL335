import serial
import csv

ser = serial.Serial('COM4', 230400)

with open('data.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    while True:
        line = ser.readline().decode()
        if line:
            writer.writerow(line.split(','))