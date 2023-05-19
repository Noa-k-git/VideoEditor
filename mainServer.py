from server import *
import database
import threading

IP = '127.0.0.1'
PORT = 8989


class MainServer(Server):
    def __init__(self, ip=IP, port=PORT):
        Server.__init__(self, ip, port)
        self.db = database.DataBase("mydb.db")  # data base object

    def handle_client(self, current_socket: socket.socket, data: bytes):
        """All client's requests end up here. 
        Args:
            current_socket (socket.socket): the client
            data (bytes): client request
        """

        def client_in_project(user_client: Client, project_id: int):
            """
            Checks Whether the client is in a project according to project_id
            :param user_client: the client
            :param project_id: the project id
            :return: True if yes, False of no
            """
            res = self.db.select(self.db.tables['project_users'], item=database.ProjectUser(user_id=user_client.u_id,
                                                                                            project_id=project_id))
            if res:
                return True
            return False

        # handling clients requests
        def send_rsa_keys():
            # time.sleep(0.1)
            # sending to client the server public key
            response_fields = [self.public_key, self.n]
            res = Protocol.join_response_fields(response_fields)
            return Protocol.chunk_response(res)

        def rsa_key(user_client: Client, info: bytes) -> Tuple[bool, str]:
            """
            The client public key, n
            :param user_client: the client
            :param info: the message information
            :return: bool -> if error occurred false, str -> message to send
            """
            try:
                user_client.rsa_key, user_client.rsa_n = [int(x) for x in Protocol.parse_message(info.decode())]
            except ValueError:
                return False, "Message arguments"
            return True, ""

        def signup(user_client: Client, info: bytes) -> Tuple[bool, str]:
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
                user_client.u_id = int(u_id[0][0])
                return True, f"{u_id[0][0]}"
            return False, "Email in use"

        def login(user_client: Client, info: bytes) -> Tuple[bool, str]:
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
                user_client.u_id = int(u_id[0][0])
                return True, f"{u_id[0][0]}"
            return False, "Incorrect email or password"

        def logout(user_client: Client, info: bytes) -> Tuple[bool, str]:
            user_client.u_id = -1
            return True, ""

        def pull_info(user_client: Client, info: bytes) -> Tuple[bool, list or str]:

            projects = self.db.join_select(og=self.db.tables['project_users'],
                                           columns=self.db.tables['projects']['id_', 'name'],
                                           join={self.db.tables['projects']['id_']: [
                                               self.db.tables['project_users']['project_id'], ]},
                                           where=[(self.db.tables['project_users'],
                                                   database.ProjectUser(user_id=int(user_client.u_id))), ])

            if not projects:
                return True, ''
            where_items = []
            for raw in projects:
                where_items.append(database.ProjectUser(project_id=int(raw[0])))

            users_names = self.db.join_select(
                og=self.db.tables["project_users"],
                columns=[self.db.tables['project_users']['project_id'], self.db.tables['users']['name']],
                join={self.db.tables['users']['id_']: [self.db.tables['project_users']['user_id'], ]},
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

        def _add_users_by_mail(emails, p_id: int, my_id=None):
            if not isinstance(emails, list):
                emails = [emails]
            if emails:
                users_id = self.db.join_select(og=self.db.tables['users'],
                                               columns=self.db.tables['users']['id_'],
                                               where=[(self.db.tables['users'], database.User(email=email)) for email in
                                                      emails])
            else:
                users_id = []
            if my_id:
                users_id.insert(0, (my_id,))

            for id_ in users_id:
                self.db.insert(self.db.tables['project_users'], database.ProjectUser(project_id=p_id, user_id=id_[0]))

        def connect_proj(user_client: Client, info: bytes) -> [bool, str]:
            try:
                p_id = int(info.decode())
            except ValueError:
                return False, 'Message arguments'
            res = self.db.select(self.db.tables['project_users'], database.ProjectUser(user_id=user_client.u_id,
                                                                                       project_id=p_id))
            if res[0]:
                # appending user active projects, changing user current project
                if user_client.p_id != -1:
                    self.active_projects[user_client.p_id].remove(user_client)

                user_client.p_id = int(p_id)

                try:
                    self.active_projects[user_client.p_id] += user_client
                except KeyError:
                    self.active_projects[user_client.p_id] = [user_client, ]

                return True, ''

            return False, 'Access Denied'

        def create(user_client: Client, info: bytes) -> Tuple[bool, str]:
            p_name, emails = "", ""
            try:
                p_name, *emails = Protocol.parse_message(info.decode())
            except ValueError:
                return False, 'Message arguments'
            self.db.insert(self.db.tables['projects'], database.Project(name=p_name, admin_id=user_client.u_id))
            p_id = self.db.select(self.db.tables['projects']['id_'], database.Project(name=p_name))[0][0]
            _add_users_by_mail(emails, p_id, user_client.u_id)

            connect_proj(user_client, str(p_id).encode())
            return True, p_id

        def add_users(user_client: Client, info: bytes) -> Tuple[bool, str]:
            emails = ""
            try:
                emails = Protocol.parse_message(info.decode())
            except ValueError:
                return False, 'Message arguments'
            if self.db.select(self.db.tables['projects']['admin_id'],
                              database.Project(id_=user_client.p_id, admin_id=user_client.u_id)):

                _add_users_by_mail(emails, user_client.p_id)
            else:
                return False, 'Permission'
            return True, ''

        def remove_user(user_client: Client, info: bytes) -> Tuple[bool, str]:
            email = ""
            try:
                email_lst = Protocol.parse_message(info.decode())
                email = email_lst[0]
            except (ValueError, IndexError):
                return False, 'Message arguments'

            # the id of the user to be removed
            remove_user_id = self.db.select(self.db.tables['users']['id_'], database.User(email=email))[0][0]

            # The project admin id
            admin_id = \
                self.db.select(self.db.tables['projects']['admin_id'], database.Project(id_=user_client.p_id))[0][0]

            # if the user asking to remove is the admin or himself && the admin can't remove himself.
            if (remove_user_id == user_client.u_id or user_client.u_id == admin_id) and remove_user_id != admin_id:
                self.db.delete(self.db.tables['project_users'],
                               database.ProjectUser(user_id=remove_user_id, project_id=int(user_client.p_id)))
            else:
                return False, 'Permission'
            return True, ''

        def push_project(user_client: Client, info: bytes) -> Tuple[bool, str]:
            content = ""
            try:
                content_lst = Protocol.parse_message(info.decode())
                content = content_lst[0]
            except (ValueError, IndexError):
                return False, 'Message arguments'
            admin_id = self.db.select(self.db.tables['projects']['admin_id'],
                                      database.Project(id_=user_client.u_id))[0][0]
            approved = None
            if user_client.u_id == admin_id:
                approved = content

            self.db.update(self.db.tables['projects'], database.Project(content=content, approved=approved),
                           database.Project(id_=user_client.p_id))
            return True, ''

        def pull_project(user_client: Client, info: bytes) -> Tuple[bool, str]:
            select_res = self.db.select(self.db.tables['projects']['content'], database.Project(id_=user_client.p_id))
            try:
                content = select_res[0]
            except IndexError:
                return False, "ERROR"

            return True, content

        def listen_conn(user_client: Client, info: bytes) -> Tuple[bool, str]:
            client_ip = user_client.conn.getpeername()[0]
            for s in self.clients:
                if s.conn.getpeername()[0] == client_ip:
                    self.clients.remove(user_client.conn)
                    s.update_conn = user_client.conn
                    return True, ''
            return False, "Could not find client"

        commands = {
            'RSAKEY': rsa_key,
            'SIGNUP': signup,
            'LOGIN': login,
            'LOGOUT': logout,
            'PULLINFO': pull_info,
            'CONNECTPROJ': connect_proj,
            'CREATE': create,
            'ADDUSERS': add_users,
            'REMOVEUSER': remove_user,
            'PUSHPROJECT': push_project,
            'PULLPROJECT': pull_project,
            'LISTEN': listen_conn
        }

        if data == b'SECURITY':
            response_parts = send_rsa_keys()
        else:
            k, cmd, message = Protocol.parse_data(data, self.private_key, self.n)
            logging.info(f"{cmd}, {message}")
            response_parts = []
            client = self.clients.get_client(current_socket)
            key_par = (client.rsa_key, client.rsa_n,)

            try:
                if k:
                    code, r_message = commands[cmd](client, message)
                    key_par = (client.rsa_key, client.rsa_n,)

                    logging.info(f"{code}, {r_message}")

                    response_parts = Protocol.build_response(cmd, code, Protocol.build_message(r_message),
                                                             *key_par)
                else:
                    response_parts = Protocol.build_response(cmd, False, message.decode(), *key_par)
            except KeyError as e:
                response_parts = Protocol.build_response(cmd, False, "Command doesn't exist", *key_par)

        for part in response_parts:
            self.messages_to_send.put((current_socket, part))  # command that does not exists


if __name__ == '__main__':
    server = MainServer()
    server.activate()
