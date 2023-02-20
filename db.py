import sqlite3
from sqlite3 import Error
from dataclasses import dataclass
import typing

class DotDict(dict):
    """dot.notation access to dictionary attributes"""
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__
    
@dataclass
class Item:
    def get(self):
        vals = {}
        for field_name in typing.get_type_hints(self).items():
            print(field_name)
            val = getattr(self, field_name[0])
            vals[field_name[0]] = val
        return vals

@dataclass
class User(Item):
    id: int
    name: str
    password: str
    email: str

@dataclass
class Project(Item):
    id: int
    name: str
    project: str

@dataclass
class Video(Item):
    id: int
    path: str

@dataclass
class ProjectVideo(Item):
    project_id: int
    video_id: int

@dataclass
class ProjectUser(Item):
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
        sql = f'''INSERT INTO {self.name}({", ".join([str(key) for key in dct.keys()])})
        VALUES ({", ".join([str(val) for val in dct.values()])})'''
        print(sql)
        try:
            self.conn.execute(sql)
        except sqlite3.IntegrityError as e:
            print("error!!", e)
        self.conn.commit()
    
    def create(self, create_table_sql):
        
        """Creates a table from create_table_sql statement

        Args:
            conn (sqlite.connect): Connection object
            create_table_sql (str): a CREATE TABLE statement
        """
        try:
            self.conn.execute(create_table_sql)
        except Error as e:
            print(e)

db_path = r"mydb1.db"
database = DataBase(db_path)
database.connect()

create_tables = DotDict()
create_tables.users = """
CREATE TABLE if not EXISTS users (
    id integer PRIMARY KEY,
    name text NOT NULL,
    password text NOT NULL,
    email text NOT NULL
); """
create_tables.projects = """
CREATE TABLE if not EXISTS projects (
    id integer PRIMARY KEY,
    name text NOT NULL,
    project text
); """
create_tables.videos = """
CREATE TABLE if not EXISTS videos (
    id integer PRIMARY KEY,
    path text NOT NULL
); """
create_tables.project_videos = """
CREATE TABLE if not EXISTS project_videos (
    project_id integer,
    video_id integer
); """
create_tables.project_users = """
CREATE TABLE if not EXISTS project_users (
    project_id integer,
    user_id integer
); """

tables = DotDict()

for create in create_tables:
    tables[create] = Table(create, database.conn)
    tables[create].create(create_tables[create])
    
tables.project_users.insert(ProjectUser(1,2))
tables.project_users.insert(ProjectUser(1,3))
tables.project_users.insert(ProjectUser(1,4))
tables.project_users.insert(ProjectUser(2,4))
tables.project_users.insert(ProjectUser(3,4))
tables.project_users.insert(ProjectUser(4,4))
tables.users.insert(User(2, "1", "123", "2"))