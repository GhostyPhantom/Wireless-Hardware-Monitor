import socket
import threading
from utils.HWinfo import HWinformation
import pythoncom
from time import sleep

HEADER = 1024
PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"
REQUEST = "REQUEST_DATA"

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(ADDR)


def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    pythoncom.CoInitialize()
    HW = HWinformation()
    connected = True

    while connected:
        sleep(0.5)
        connected, msg = receive_handshake(addr, conn, HEADER, FORMAT)
        if msg in [DISCONNECT_MESSAGE, REQUEST, "NULL"]:
            pass
        else:
            pass
            #print(f"[{addr}] {msg}")
            send("Message Received", conn)
        if msg == REQUEST:
            message = HW.read()
            data = f"CPU Temp:{str(message['CPU Temp'])}//CPU Usage:{str(message['CPU Usage'])}%  //" \
                   f"GPU Temp:{str(message['GPU Temp'])}//GPU Usage:{str(message['GPU Usage'])}%  //" \
                   f"Memory Usage:{str(message['Memory Usage'])}%  //GPU Memory:{str(message['GPU Memory'])}%  //$$"
            send_data(data, conn)

    conn.close()


def send(msg, server):
    message = msg.encode(FORMAT)
    msg_length = len(message)
    send_length = str(msg_length).encode(FORMAT)
    send_length += b' ' * (HEADER - len(send_length))
    server.send(send_length)
    server.send(message)


def send_data(data, server):
    message = data.encode(FORMAT)
    server.send(message)


def receive_handshake(addr, conn, HEADER, FORMAT):
    msg = conn.recv(1024).decode(FORMAT)
    if msg == DISCONNECT_MESSAGE:
        return False, msg
    #print(f"[{addr}] {msg}")
    return True, msg


def start():
    server.listen()
    print(f"[LISTENING] Server is listenling on {SERVER}")
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        print(f"[ACTIVATE CONNECTIONS] {threading.activeCount() - 1}")


print("[STARTING] server is starting...")

start()
