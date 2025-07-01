import socket
import serial
import time

# Your Serial Port Configuration
SERIAL_PORT = "/dev/ttyACM1"  # 
BAUDRATE = 115200

# Open the serial port
ser = serial.Serial(SERIAL_PORT, baudrate=BAUDRATE, timeout=1)
time.sleep(2)

# UDP Configuration
UDP_IP = "0.0.0.0"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on UDP {UDP_IP}:{UDP_PORT} and forwarding to {SERIAL_PORT}")

while True:
    data, addr = sock.recvfrom(1024)  # UDP data
    decoded = data.decode().strip()
    print(f"Received from {addr}: '{decoded}'")
    
    # Send to serial port
    ser.write(decoded.encode())
    print(f"Forwarded to serial: '{decoded}'")
