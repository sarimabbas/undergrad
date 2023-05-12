import socket

LOCAL_UDP_IP = "192.168.1.2"
SHARED_UDP_PORT = 57222
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))

def loop():
    while True:
        data, addr = sock.recvfrom(2048)
        print(data)

if __name__ == "__main__":
    loop()
