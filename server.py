import coloredlogs
import logging
import queue
import select
import socket
import threading
import time
from abc import ABC, abstractmethod

import shift_cipher
from protocol import *


class Client:
    def __init__(self, conn):
        self.conn = conn
        self.rsa_key = -1
        self.rsa_n = -1
        self.id = -1
        self.update_conn = None


class Clients(list):
    def remove(self, __value: socket.socket) -> None:
        value = None
        for client in self:
            if client.conn == __value:
                value = client
        if value is not None:
            super().remove(value)

    def get_client(self, conn: socket.socket):
        for client in self:
            if client.conn == conn:
                return client

    def get_sockets(self):
        return [client.conn for client in self]


class Server(ABC):
    def __init__(self, ip, port):
        logging.basicConfig(level=logging.DEBUG)
        coloredlogs.install(level='DEBUG')

        self.IP = ip
        self.PORT = port
        self.server_socket = None
        self.clients = Clients()
        self.messages_to_send = queue.Queue()  # [socket, data], ...
        self.rlist, self.wlist = [], []

        self.public_key, self.private_key, self.n = rsa_crypto.setkeys()

        self.sending_messages_thread = threading.Thread(target=self.send_waiting_messages,)
    
    def create_server(self):
        # Creating a server
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
        self.server_socket.bind((self.IP, self.PORT))
        logging.info(f'server is up at : {self.IP}, {self.PORT}')

        self.server_socket.listen(1)
        self.sending_messages_thread.start()

    def send_waiting_messages(self):
        """A function for sending messages
        Args:
                """
        while True:
            current_socket, data = self.messages_to_send.get() 
            try:
                current_socket.send(data)
                logging.debug(f"sending data: {data} to {current_socket.getpeername()}")
            except (Exception,) as e:
                logging.warning(f"Failed to send: {data} to {current_socket}\n{e}")
    
    def activate(self):
        """Handling clients
        """
        self.create_server()
        while True:
            self.rlist, self.wlist, _ = select.select([self.server_socket] + self.clients.get_sockets(),
                                                      self.clients.get_sockets(), [])
            
            for current_socket in self.rlist:
                if current_socket is self.server_socket:
                    (new_socket, address) = self.server_socket.accept()
                    logging.info(f"new socket connected to server: {new_socket.getpeername()}")
                    self.clients.append(Client(new_socket))
                    threading.Thread(target=self.send_rsa_keys, args=(new_socket,)).start()

                else:
                    try:
                        data = b''
                        parts = []
                        while True:
                            try:
                                part = current_socket.recv(1024)
                            except ConnectionAbortedError:
                                raise ConnectionResetError
                            # if it is an empty packet then it is the last packet
                            if len(part) == Protocol.NUM_FIELD_LENGTH:
                                logging.debug(f'new data from client {current_socket.getpeername()}: {parts}')
                                break
                            try:
                                parts.append((int(part[:Protocol.NUM_FIELD_LENGTH]), part[Protocol.NUM_FIELD_LENGTH:]))
                            except ValueError:
                                cl = self.clients.get_client(current_socket)
                                cl_par = (cl.rsa_key, cl.rsa_n)
                                response_parts = Protocol.build_response('NUMERIC', False,
                                                                         "Packets number format", *cl_par)
                                for response in response_parts:
                                    self.messages_to_send.put((current_socket, response))
                        parts.sort()
                        for part in parts:
                            data += part[1]
                        data = shift_cipher.decrypt(data.decode(), Protocol.SHIFT_KEY).encode()
                        if data == b'end':
                            p_id = current_socket.getpeername()
                            self.messages_to_send.put((current_socket, b'end'))
                            self.connection_closed(current_socket)
                            current_socket.close()
                            logging.info(f"Connection with client {p_id} closed.")

                        else:
                            threading.Thread(target=self.handle_client, args=(current_socket, data)).start()

                    except ConnectionResetError:  # handling a client randomly closed
                        logging.error("ConnectionResetError: Socket forcibly closed!")
                        self.connection_closed(current_socket)
    
    @abstractmethod
    def handle_client(self, current_socket: socket.socket, data: str):
        """Handling the client's request.
        Args:
            current_socket (socket.socket): the client's socket
            data (str): data received from client
        """ 
        pass
    
    def connection_closed(self, current_socket: socket.socket):
        """ removing client from requests.
        """
        self.clients.remove(current_socket)
        current_socket.close()

    def send_rsa_keys(self, new_socket):
        time.sleep(0.1)
        # sending to client the server public key
        response_fields = [self.public_key, self.n]
        res = Protocol.join_response_fields(response_fields)
        response_parts = Protocol.chunk_response(res)
        for part in response_parts:
            self.messages_to_send.put((new_socket, part))
