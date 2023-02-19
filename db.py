import sqlite3
from sqlite3 import Error
from dataclasses import dataclass, fields
import typing

@dataclass
class Item:
    def get(self):
        vals = {}
        for field_name in typing.get_type_hints(self).items():
            val = getattr(self, field_name)
            vals[field_name] = val
        return vals

@dataclass
class User(Item):
    id: int
    name: str
    password: str
    email: str

@dataclass
class Project:
    id: int
    name: str
    project: str

@dataclass
class Video:
    id: int
    path: str

@dataclass
class ProjectVideo:
    project_id: int
    video_id: int

@dataclass
class ProjectUser:
    project_id: int
    user_id: int


class DataBase():
    def __init__(self, db_path):
        self.db_path = db_path
        self.conn = None
    
    def connect(self):
        """Creates a database connection to the SQLite database specified by db_file

        Args:
            db_file (str): database file path
            return: Connection object or None
        """
        conn = None
        try:
            self.conn = sqlite3.connect(self.db_path)
        except Error as e:
            print(e)

class Table():
    def __init__(self, name, conn) -> None:
        self.name = name
        self.conn = conn

    def insert(self, item):
        dct = item.get()
        sql = f'''INSERT INTO {self.name}({", ".join([str(key) for key in dct.keys()])}
        VALUES ({", ".join([str(val) for val in dct.values()])}'''

    
    def create(create_table_sql):
        
        """Creates a table from create_table_sql statement

        Args:
            conn (sqlite.connect): Connection obejct
            create_table_sql (str): a CREATE TABLE statement
        """
        try:
            c = conn.cursor()
            c.execute(create_table_sql)
        except Error as e:
            print(e)

class UsersTable(Table):
    def __init__(self, conn):
        self.__super__("users", conn)
        

def append_to_table(conn, tabel_name, args_lst):
    sql = f"""INSERT INTO {table} ()"""

database = r"mydb.db"

create_tables = {}
create_tables["sql_create_users_table"] = """
CREATE TABLE if not EXISTS users (
    id integer PRIMARY KEY,
    name text NOT NULL,
    password text NOT NULL,
    email text NOT NULL
); """
create_tables["sql_create_project_table"] = """
CREATE TABLE if not EXISTS projects (
    id integer PRIMARY KEY,
    name text NOT NULL,
    project text
); """
create_tables["sql_create_videos_table"] = """
CREATE TABLE if not EXISTS videos (
    id integer PRIMARY KEY,
    path text NOT NULL
); """
create_tables["sql_create_project_videos_table"] = """
CREATE TABLE if not EXISTS project_videos (
    project_id integer PRIMARY KEY,
    video_id integer
); """
create_tables["sql_create_project_users_table"] = """
CREATE TABLE if not EXISTS project_users (
    project_id integer PRIMARY KEY,
    user_id integer
); """

# Create a database connection
conn = create_connection(database)
 # create tables

if conn is not None:
    for table in create_tables:
        create_table(conn, create_tables[table])

else:
    print("Error! cannot create the database connection")