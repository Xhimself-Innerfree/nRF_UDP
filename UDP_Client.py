import socket

# RPi IP Port
UDP_IP = "192.168.1.100"  # <-- Your RPi IP 
UDP_PORT = 5005

#  UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"Sending to {UDP_IP}:{UDP_PORT}. Type messages to send. Type 'exit' to quit.\n")

# Read and send messages
while True:
    msg = input(">>> ")
    if msg.lower() == "exit":
        print("Exiting.")
        break
    sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
    print(f"Sent: {msg}")
