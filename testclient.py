import socket

def connect_to_server():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('127.0.0.1', 8989))
    while True:
        message = input().encode()
        s.send(message)
        s.send(b'00000002')
        response = s.recv(1024)
        print(response.decode())
        response = s.recv(1024)
        print(response.decode())
    s.close()
    