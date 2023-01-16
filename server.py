import socket

IP = '127.0.0.1'
PORT = 38745

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.bind((IP, PORT))

print(f"server is up on {PORT}")
server_socket.listen(1)

client_socket, address = server_socket.accept()

client_socket.send(b'hello from server')

data = client_socket.recv(1024)

print(data)
client_socket.close()
