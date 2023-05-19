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
        self.u_id = -1
        self.p_id = -1
        self.update_conn = None


class Clients(list):
    def __init__(self):
        super().__init__()
        self.lock = threading.Lock()

    def append(self, item):
        self.lock.acquire()
        try:
            super().append(item)
        except (Exception,) as e:
            self.lock.release()
            raise e
        self.lock.release()

    def __add__(self, other):
        self.lock.acquire()
        try:
            res = super().__add__(other)
        except (Exception,) as e:
            self.lock.release()
            raise e
        self.lock.release()
        return res

    def remove(self, item) -> None:
        self.lock.acquire()
        try:
            super().remove(item)
        except (Exception,) as e:
            self.lock.release()
            raise e
        self.lock.release()

    def get_client(self, conn: socket.socket):
        self.lock.acquire()
        for client in self:
            if client.conn == conn:
                self.lock.release()
                return client
        self.lock.release()

    def get_sockets(self):
        self.lock.acquire()
        li = [client.conn for client in self]
        self.lock.release()
        return li


class Server(ABC):
    def __init__(self, ip, port):
        logging.basicConfig(level=logging.DEBUG)
        coloredlogs.install(level='DEBUG')

        self.IP = ip
        self.PORT = port
        self.server_socket = None
        self.clients = Clients()
        self.messages_to_send = queue.Queue()  # [socket, data], ...
        self.rlist, self.wlist, = [], []

        self.active_projects = {}

        self.public_key, self.private_key, self.n = rsa_crypto.setkeys()

        self.sending_messages_thread = threading.Thread(target=self.send_waiting_messages, )

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

                else:
                    threading.Thread(target=self.receive_request, args=(current_socket,)).start()

    def receive_request(self, current_socket):
        try:
            data = b''
            parts = []
            while True:
                part = current_socket.recv(1024)
                if part == b'':
                    raise ConnectionAbortedError
                # if it is an empty packet then it is the last packet
                if len(part) == Protocol.NUM_FIELD_LENGTH:
                    logging.debug(f'new data from client {current_socket.getpeername()}: {parts}')
                    break
                try:
                    parts.append((int(part[:Protocol.NUM_FIELD_LENGTH]), part[Protocol.NUM_FIELD_LENGTH:]))
                    logging.debug(shift_cipher.decrypt(part[Protocol.NUM_FIELD_LENGTH:].decode(), Protocol.SHIFT_KEY))
                except ValueError:
                    cl = self.clients.get_client(current_socket)
                    cl_par = (cl.rsa_key, cl.rsa_n)
                    response_parts = Protocol.build_response('NUMERIC', False,
                                                             "Packets number format", *cl_par)
                    for response in response_parts:
                        self.messages_to_send.put((current_socket, response))
                    return

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
                time.sleep(0.2)
                self.handle_client(current_socket, data)

        except (ConnectionResetError, ConnectionAbortedError, OSError):  # handling a client randomly closed
            logging.error("Socket forcibly closed!")
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
        client = self.clients.get_client(current_socket)
        self.clients.remove(client)
        current_socket.close()
        try:
            client.update_conn.close()
        except AttributeError:
            pass
