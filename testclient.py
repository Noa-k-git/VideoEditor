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
        time.sleep(1)
        s.send(b'00000002')
        response = s.recv(1024)
        print(response.decode())
        response = s.recv(1024)
        print(response.decode())

messages = [
    '00000001SIGNUP          |00000017|noa;1@gmail.com;1',
    '00000001SIGNUP          |00000017|ido;2@gmail.com;2',
    '00000001SIGNUP          |00000017|ofe;3@gmail.com;3',
    '00000001CREATE          |00000028|1;p1;2@gmail.com>3@gmail.com',
    '00000001CREATE          |00000005|1;p2;',
    '00000001PULLINFO        |00000001|2',
    '00000001PULLINFO        |00000001|1',
    '00000001ADDUSERS        |00000015|1;2;2@gmail.com',
    '00000001PULLINFO        |00000001|2',
    
]