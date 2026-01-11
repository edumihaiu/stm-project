import serial
import serial.tools.list_ports
import pyautogui
import time
import sys
import os


# no pause
pyautogui.PAUSE = 0 

SERIAL_PORT = 'COM11' 
BAUD_RATE = 115200 


print(f"start mouse on {SERIAL_PORT}...")

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.01)
    time.sleep(2) # wait for stm
    
    ser.reset_input_buffer()
    
    print("connected")

    while True:
        try:
            last_valid_line = None
            
            while ser.in_waiting > 0:
                line_bytes = ser.readline()
                try:
                    line_str = line_bytes.decode('utf-8').strip()
                    if "," in line_str:
                        last_valid_line = line_str
                except:
                    pass 
            
            if last_valid_line:
                parts = last_valid_line.split(',')
                if len(parts) == 2:
                    dx = int(parts[0])
                    dy = int(parts[1])
                    
                    pyautogui.moveRel(dx, dy, _pause=False)

        except ValueError:
            pass
        except OSError:
            print("disconnected")
            break

except serial.SerialException:
    print(f"error reading serial port {SERIAL_PORT}")