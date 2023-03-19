from server import *
from protocol import Protocol
import database
import threading

IP='127.0.0.1'
PORT=8989

class MainServer(Server):
    def __init__(self,  ip=IP, port=PORT):
        Server.__init__(self, ip, port)
        self.db = database.DataBase("mydb.db") # data base object    
    
    def handle_client(self, current_socket:socket.socket, data:bytes):
        """All client's requests end up here. 
        Args:
            current_socket (socket.socket): the client
            data (str): client request
        """        
        # handling clients requests
        def signup(info):
            """A signup request from the user. Append the new user to the database
            """
            pass
        
        def login(info) -> Tuple[bool,str]:
            """A login request from the user.
            """
            return True, "Connected!"
            
        def logout(info):
            pass
        
        def pull_info(info):
            pass
        
        def create(info):
            pass
        
        def add_user(info):
            pass
        
        def remove_user(info):
            pass
        
        def push_project(info):
            pass
        
        def pull_project(info):
            pass
        
        commands = {'SIGNUP': signup,
                    'LOGIN': login,
                    'LOGOUT': logout, 
                    'PULLINFO': pull_info,
                    'CREATE': create,
                    'ADDUSER': add_user,
                    'REMOVEUSER': remove_user,
                    'PUSHPRO JECT': push_project,
                    'PULLPROJECT': pull_project,
                    }
        k, cmd, message = Protocol.parse_data(data) # b for binary    
        response_parts = []
        try:
            if k:
                code, r_message = commands[cmd](message)
                response_parts = Protocol.build_response(cmd, code, r_message)
            else:
                response_parts = Protocol.build_response(cmd,"error", message.decode())
        except KeyError:
            response_parts = Protocol.build_response(cmd, "error", "Command doesn't exist")
        finally:
            for part in response_parts:
                self.messages_to_send.put((current_socket, part)) # command that does not exists
        
    

    def connection_closed(self, current_socket:socket.socket):
        """ Logout for the user

        Args:
            current_socket (socket.socket): the closed socket
        """
        pass
        

if __name__=='__main__':
    server = MainServer()
    server.activate()
