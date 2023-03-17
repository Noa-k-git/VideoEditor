from abc import ABC, abstractmethod
import select
import socket
import threading
import logging, coloredlogs
import queue
from protocol import *
class Server(ABC):
    def __init__(self, ip, port):
        logging.basicConfig(level=logging.DEBUG)
        coloredlogs.install(level='DEBUG')

        self.IP = ip
        self.PORT = port
        self.server_socket = None
        self.open_client_sockets = []
        self.messages_to_send = queue.Queue() # [socket, data], ...
        self.rlist, self.wlist = [], []
        
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
            wlist (list): sockets to write to.
        """
        while True:
            current_socket, data = self.messages_to_send.get() 
            if current_socket in self.wlist:
                try: 
                    current_socket.send(data)
                    logging.debug(f"sending data: {data} to {current_socket.getpeername()}")
                except:
                    logging.warning(f"Failed to send: {data} to {current_socket}")
    
    def activate(self):
        """Handling clients
        """
        self.create_server()
        while True:
            self.rlist, self.wlist, _ = select.select( [self.server_socket] + self.open_client_sockets, self.open_client_sockets, [])
            
            for current_socket in self.rlist:
                if current_socket is self.server_socket:
                    (new_socket, address) = self.server_socket.accept()
                    logging.info(f"new socket connected to server: {new_socket.getpeername()}")
                    self.open_client_sockets.append(new_socket)
                else:
                    try:
                        data = b''
                        parts = []
                        while True:
                            part = current_socket.recv(1024)
                            # if it is an empty packet then it is the last packet
                            if len(part)==Protocol.NUM_FIELD_LENGTH:
                                logging.debug(f'new data from client {current_socket.getpeername()}: {data}')
                                break
                            try:
                                parts.append((int(part[:Protocol.NUM_FIELD_LENGTH]), data[Protocol.NUM_FIELD_LENGTH:]))
                            except ValueError:
                                response_parts = Protocol.build_response('NUMERIC', 'error', "Packets number format")   
                                for part in response_parts:
                                    self.messages_to_send.put((current_socket, part))
                        parts.sort()
                        for part in parts:
                            data += part[1]
                            
                        if data == b'end':
                            p_id = current_socket.getpeername()
                            self.messages_to_send.put((current_socket, b'end'))
                            self.connection_closed(current_socket)
                            current_socket.close()
                            logging.info(f"Connection with client {p_id} closed.")

                        else:
                            threading.Thread(target=self.handle_client, args=(current_socket, data)).start()
                            data=''
                            
                    except ConnectionResetError: # handling a client randomly closed
                        logging.error("ConnectionResetError: Socket forcibly closed!")
                        self.connection_closed(current_socket)
    
    @abstractmethod
    def handle_client(self, current_socket:socket.socket, data:str):
        """Handling the client's request.
        Args:
            current_socket (socket.socket): the client's socket
            data (str): data received from client
        """ 
        pass
    
    def connection_closed(self, current_socket:socket.socket):
        """ removing client from requests.
        """
        self.open_client_sockets.remove(current_socket)
        