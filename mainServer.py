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
        def signup(info:bytes) -> Tuple[bool, str]:
            """A signup request from the user. Append the new user to the database
            
            Args:
                info (bytes): the message information

            Returns:
                Tuple[bool,str]: bool -> False if error occurred, srt -> message to send
            """
            username, email, password = "", "", ""
            try:
                username, email, password = Protocol.parse_message(info.decode())
            except ValueError:
                return False, "Message arguments"
            
            self.db.tables["users"].insert(database.User(name=username, email=email, password=password))
            u_id = self.db.tables["users"].select("id", database.User(email=email, password=password))
            
            if u_id:
                return True, f"{u_id[0][0]}"
            return False, "Email in use"
        
        def login(info:bytes) -> Tuple[bool,str]:
            """A login request from the user

            Args:
                info (bytes): the message information

            Returns:
                Tuple[bool,str]: bool -> False if error occurred, srt -> message to send
            """
            email, password = "", ""
            try:
                email, password = Protocol.parse_message(info.decode())
            except ValueError:
                return False, "Message arguments"
            
            u_id = self.db.tables["users"].select("id", database.User(email=email, password=password))
            if u_id:
                return True, f"{u_id[0][0]}"
            return False, "Incorrect email or password"
            
        def logout(info:bytes) -> Tuple[bool, str]:
            pass
        
        def pull_info(info:bytes) -> Tuple[bool, str]:
            pass
        
        def create(info:bytes) -> Tuple[bool, str]:
            pass
        
        def add_user(info:bytes) -> Tuple[bool, str]:
            pass
        
        def remove_user(info:bytes) -> Tuple[bool, str]:
            pass
        
        def push_project(info:bytes) -> Tuple[bool, str]:
            pass
        
        def pull_project(info:bytes) -> Tuple[bool, str]:
            pass
        
        commands = {'SIGNUP': signup,
                    'LOGIN': login,
                    'LOGOUT': logout, 
                    'PULLINFO': pull_info,
                    'CREATE': create,
                    'ADDUSER': add_user,
                    'REMOVEUSER': remove_user,
                    'PUSHPROJECT': push_project,
                    'PULLPROJECT': pull_project,
                    }
        k, cmd, message = Protocol.parse_data(data) # b for binary    
        response_parts = []
        try:
            if k:
                code, r_message = commands[cmd](message)
                response_parts = Protocol.build_response(cmd, code, r_message)
            else:
                response_parts = Protocol.build_response(cmd, False, message.decode())
        except KeyError as e:
            response_parts = Protocol.build_response(cmd, False, "Command doesn't exist")
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
