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
            
            self.db.insert(self.db.tables['users'], database.User(name=username, email=email, password=password))
            u_id = self.db.select(self.db.tables['users']['id_'], database.User(email=email, password=password))
            
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
            u_id = self.db.select(self.db.tables['users']['id_'], database.User(email=email, password=password))
            if u_id:
                return True, f"{u_id[0][0]}"
            return False, "Incorrect email or password"
            
        def logout(info:bytes) -> Tuple[bool, str]:
            pass
        
        def pull_info(info:bytes) -> Tuple[bool, str]:
            user_id = ""
            try:
                user_id = Protocol.parse_message(info.decode())
            except ValueError:
                return False, "Message arguments"
            projects = self.db.join_select(og=self.db.tables['project_users'],
                                columns=self.db.tables['projects']['id_', 'name'],
                                join={self.db.tables['projects']['id_']:[self.db.tables['project_users']['project_id'],]},
                                where=[(self.db.tables['project_users'], database.ProjectUser(user_id=user_id)),])
            
            if not projects:
                return True, ''
            where_items = []
            for raw in projects:
                where_items.append(database.ProjectUser(project_id=int(raw[0])))

            users_names = self.db.join_select(
                og = self.db.tables["project_users"],
                columns= [self.db.tables['project_users']['project_id'], self.db.tables['users']['name']],
                join= {self.db.tables['users']['id_']:[self.db.tables['project_users']['user_id'],]},
                where=[(self.db.tables['project_users'], item) for item in where_items]
            )
            projects_users = {}
            for project_id, name in users_names:
                if project_id in projects_users:
                    projects_users[project_id].append(name)
                else:
                    projects_users[project_id] = [name, ]
            for p in projects_users:
                projects_users[p].sort()
            result = [(p_id, p_name, projects_users[p_id]) for p_id, p_name in projects]
            return True, result
        
        def _add_users_by_mail(emails, p_id, my_id=None):
            if not isinstance(emails, list):
                emails = [emails]
            if emails:
                users_id = self.db.join_select(og=self.db.tables['users'],
                                            columns=self.db.tables['users']['id_'],
                                            where= [(self.db.tables['users'], database.User(email=email)) for email in emails])
            else:
                users_id = []
            if my_id:
                users_id.insert(0, (my_id,))
                
            for id_ in users_id:
                self.db.insert(self.db.tables['project_users'], database.ProjectUser(project_id=p_id, user_id=id_[0]))
                
        def create(info:bytes) -> Tuple[bool, str]:
            u_id, p_name, emails = "", "" ,""
            try:
                u_id, p_name, emails = Protocol.parse_message(info.decode())
            except ValueError:
                return False, 'Message arguments' 
            self.db.insert(self.db.tables['projects'], database.Project(name=p_name, admin_id=u_id))
            p_id = self.db.select(self.db.tables['projects']['id_'], database.Project(name=p_name))[0][0]
            _add_users_by_mail(emails, p_id, u_id)
            return True, p_id
                
        def add_users(info:bytes) -> Tuple[bool, str]:
            u_id, p_id, emails = "", "", ""
            try:
                u_id, p_id, emails = Protocol.parse_message(info.decode())
            except ValueError:
                return False, 'Message arguments'
            if self.db.select(self.db.tables['projects']['admin_id'], database.Project(id_=p_id, admin_id=u_id)):
                _add_users_by_mail(emails, p_id)
            else:
                return False, 'Permission'
            return True, ''
        
        def remove_user(info:bytes) -> Tuple[bool, str]:
            u_id, p_id, email = "", "", ""
            try:
                u_id, p_id, email = Protocol.parse_message(info.decode())
            except ValueError:
                return False, 'Message arguments'
            remove_user_id = self.db.select(self.db.tables['users']['id_'], database.User(email=email))[0][0]
            admin_id = self.db.select(self.db.tables['projects']['admin_id'], database.Project(id_=p_id))[0][0]
            int_u_id = int(u_id)
            if (remove_user_id == int_u_id or int_u_id == admin_id) and remove_user_id != admin_id:
                self.db.delete(self.db.tables['project_users'], database.ProjectUser(user_id=remove_user_id, project_id=int(p_id)))
            else: 
                return False, 'Permission'
            return True, ''   
        
        def push_project(info:bytes) -> Tuple[bool, str]:
            u_id, p_id, content = "", "", ""
            try:
                u_id, p_id, content = Protocol.parse_message(info.decode())
            except ValueError:
                return False, 'Message arguments'
            admin_id = self.db.select(self.db.tables['projects']['admin_id'], database.Project(id_=p_id))[0][0]
            approved = None
            if int(u_id) == admin_id:
                approved = content
            
            self.db.update(self.db.tables['projects'], database.Project(content=content, approved=approved),
                           database.Project(id_=int(p_id)))
            return True, ''
        
        def pull_project(info:bytes) -> Tuple[bool, str]:
            pass
        
        commands = {'SIGNUP': signup,
                    'LOGIN': login,
                    'LOGOUT': logout, 
                    'PULLINFO': pull_info,
                    'CREATE': create,
                    'ADDUSERS': add_users,
                    'REMOVEUSER': remove_user,
                    'PUSHPROJECT': push_project,
                    'PULLPROJECT': pull_project,
                    }
        k, cmd, message = Protocol.parse_data(data) # b for binary    
        response_parts = []
        try:
            if k:
                code, r_message = commands[cmd](message)
                response_parts = Protocol.build_response(cmd, code, Protocol.build_message(r_message))
            else:
                response_parts = Protocol.build_response(cmd, False, message.decode())
        except KeyError as e:
            response_parts = Protocol.build_response(cmd, False, "Command doesn't exist")
        finally:
            for part in response_parts:
                self.messages_to_send.put((current_socket, part)) # command that does not exists
                

if __name__=='__main__':
    server = MainServer()
    server.activate()
