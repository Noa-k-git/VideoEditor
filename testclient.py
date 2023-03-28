import socket

def connect_to_server():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('127.0.0.1', 8989))
    return s

def test(messages):
    s = connect_to_server()
    import time
    for a_message in messages:
        message =a_message.encode()
        print(message)
        s.send(message)
        time.sleep(0.1)
        s.send(b'00000002')
        response = s.recv(1024)
        print(response.decode())
        response = s.recv(1024)
        print(response.decode())
